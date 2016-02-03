#include "vritemrenderer.h"

#include "vritem.h"
#include <QThread>

#include <GLES3/gl3.h>

VrItemRenderer::VrItemRenderer(const VrItem *item):
    item(item), texture(0), framebuffer(0), depth_buffer(0), m_distortion(true)
{
    connect(item, &QQuickItem::destroyed, this, &VrItemRenderer::selfDestruct);
    connect(item, &QQuickItem::windowChanged,   this, &VrItemRenderer::windowChanged);
    connect(item, &QQuickItem::visibleChanged,  this, &VrItemRenderer::visibleChanged);

    visible=item->isVisible();

    if(item->window()!=NULL)
        windowChanged(item->window());
}

VrItemRenderer::~VrItemRenderer()
{
    deleteFramebuffer();
}

void VrItemRenderer::update()
{
    item->window()->update();
}

void VrItemRenderer::updateProjection(const VrProjection *projection)
{
    config_copy = *projection;
}

void VrItemRenderer::windowChanged(QQuickWindow *window)
{
    connect(window, &QQuickWindow::afterRendering, this, &VrItemRenderer::afterRendering, Qt::DirectConnection);
    connect(window, &QQuickWindow::beforeSynchronizing, this, &VrItemRenderer::beforeSynchronizing, Qt::DirectConnection);
}

void VrItemRenderer::afterRendering()
{
    if(!visible) return;

    QOpenGLFramebufferObject::bindDefault();//disable the gui_fbo which the QT inserted

    if(framebuffer!=0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, config_copy.canvasSize().width(), config_copy.canvasSize().height());
        mutex.lock();
        renderScene(config_copy.canvasSize().width(), config_copy.canvasSize().height());
        mutex.unlock();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef Q_OS_SAILFISH
        glViewport(0, 0, config_copy.viewSize().height(), config_copy.viewSize().width());
#else
        glViewport(0, 0, config_copy.viewSize().width(), config_copy.viewSize().height());
#endif

        glClearColor(item->window()->color().red(), item->window()->color().green(),
                     item->window()->color().blue(), item->window()->color().alpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(m_distortion)
            goggles_render.render(texture);
        else
            fullscreen_render.render(texture);
    }
    QOpenGLFramebufferObject::bindDefault();
    update();
}

void VrItemRenderer::beforeSynchronizing()
{
    if(item->projection()->viewSize() != config_copy.viewSize())
    {
        config_copy = *item->projection();
        QSize size = config_copy.canvasSize();

        createFramebuffer(size.width(), size.height());

        goggles_render.updateProjection(&config_copy);
        fullscreen_render.updateProjection(&config_copy);

        updateProjection(&config_copy);
    }
}

void VrItemRenderer::visibleChanged()
{
    visible = item->isVisible();
    if(visible)
        update();
}

void VrItemRenderer::selfDestruct()
{
    delete this;
}

void VrItemRenderer::createFramebuffer(int width, int height)
{
    deleteFramebuffer();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    //-------------------------
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    //-------------------------
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
    //-------------------------
    //Does the GPU support current FBO configuration?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE: break;
    case GL_FRAMEBUFFER_UNDEFINED:                      qDebug()<<"Framebuffer: GL_FRAMEBUFFER_UNDEFINED"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
    case GL_FRAMEBUFFER_UNSUPPORTED:                    qDebug()<<"Framebuffer: GL_FRAMEBUFFER_UNSUPPORTED"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:         qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
#ifndef GL_ES_VERSION_2_0
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:       qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         qDebug()<<"Framebuffer: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
#endif
    default:
        qDebug()<<"GL_FRAMEBUFFER ERROR:"<<status;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VrItemRenderer::deleteFramebuffer()
{
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depth_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);

    texture = 0;
    depth_buffer = 0;
    framebuffer = 0;
}

void VrItemRenderer::synchronize()
{
}

const VrProjection *VrItemRenderer::projection() const
{
    return &config_copy;
}

void VrItemRenderer::setDistortion(bool enable)
{
    m_distortion=enable;
}

bool VrItemRenderer::distortion()
{
    return m_distortion;
}
