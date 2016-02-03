#include "mainwindow.h"

#include <QScreen>

MainWindow::MainWindow(QWindow *parent)
    : QWindow(parent), inited(false), context(NULL), fbo(NULL)
{
    setSurfaceType(QWindow::OpenGLSurface);
    setWidth(800);
    setHeight(480);
    sensor.start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    projection.resize(physicalSize(), size());
    if(context!=NULL)
    {
        context->makeCurrent(this);
        goggles.updateProjection(&projection);
        resizeBuffer();
        context->doneCurrent();
    }
}

void MainWindow::render()
{
    if (!isExposed())
        return;

    const QQuaternion & rotation = sensor.reading()->rotation;
    QQuaternion rot(rotation.scalar(), -rotation.y(), -rotation.x(), rotation.z());
    rot*=QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, 90);


    QMatrix4x4 right_vp = projection.rightViewProjection(rot);
    QMatrix4x4 left_vp = projection.leftViewProjection(rot);

    if (!context) {
        context = new QOpenGLContext(this);
        context->setFormat(requestedFormat());
        context->create();
    }

    if(!inited)
    {
        context->makeCurrent(this);
        initGL();
        context->doneCurrent();
        inited=true;
    }

    context->makeCurrent(this);

    fbo->bind();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0.0, 0.0, fbo->width()/2.0, fbo->height());
    renderEye(left_vp);
    glViewport(fbo->width()/2.0, 0.0, fbo->width()/2.0, fbo->height());
    renderEye(right_vp);
    fbo->release();

    glViewport(0.0, 0.0, width(), height());
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    goggles.render(fbo->texture());

    context->swapBuffers(this);


    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

QSizeF MainWindow::physicalSize() const
{
#ifdef Q_OS_SAILFISH
    //THIS IS HACK FOR NEXUS 5 on Sailfish OS, because screen()->physicalDotsPerInch() returns always 100
    //also have to swap width/height because sailfish does not have display rotation
    QSizeF physical(float(height())/float(445.0),
                    float(width())/float(445.0));

    physical.rwidth()*=0.0254;//inch to m
    physical.rheight()*=0.0254;
#else
    QSizeF physical(float(width())/float(screen()->physicalDotsPerInchX()),
                    float(height())/float(screen()->physicalDotsPerInchY()));

    physical.rwidth()*=0.0254;//inch to m
    physical.rheight()*=0.0254;
#endif
    return physical;
}

void MainWindow::renderEye(const QMatrix4x4 &vp)
{
    QMatrix4x4 mvp(vp);
    mvp.translate(-5, 0, 0);
    cube.render(mvp);

    mvp=vp;
    mvp.translate(5, 0, 0);
    cube.render(mvp);

    mvp=vp;
    mvp.translate(0, 0, -5);
    cube.render(mvp);

    mvp=vp;
    mvp.translate(0, 0, 5);
    cube.render(mvp);
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render();
        return true;
    default:
        return QWindow::event(event);
    }
}

void MainWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

void MainWindow::initGL()
{
    cube.initGL();
    resizeBuffer();
    goggles.updateProjection(&projection);
#ifdef Q_OS_ANDROID
    showFullScreen();
#endif
}

void MainWindow::resizeBuffer()
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::Depth);
    format.setTextureTarget(GL_TEXTURE_2D);

    if(fbo!=NULL)
        delete fbo;
    fbo = new QOpenGLFramebufferObject(projection.canvasSize(), format);
    fbo->bindDefault();
}
