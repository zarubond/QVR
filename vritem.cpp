#include "vritem.h"

#include <QScreen>
#include <QThread>

VrItem::VrItem(QQuickItem *parent):
    QQuickItem(parent), renderer(NULL)
{
    //setFlag(ItemHasContents);
    connect(this, &QQuickItem::windowChanged, this, &VrItem::windowChanged);
}

void VrItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    qDebug()<<newGeometry;
    if(window()!=NULL)
        updateSize();
}

void VrItem::updateSize()
{
    QSize s_px = QSize(width(), height());
    QSizeF s_m = physicalSize();

#ifdef Q_OS_SAILFISH // Sailfish does not support display rotation, so I have to force it in the program
    int t=s_px.width();
    s_px.setWidth(s_px.height());
    s_px.setHeight(t);
#endif

    m_config.resize(s_m, s_px);
    updateProjection(&m_config);
}

QSizeF VrItem::physicalSize() const
{
    QSizeF physical(0, 0);
#ifdef Q_OS_SAILFISH
    //THIS IS HACK FOR NEXUS 5 with Sailfish OS, because screen()->physicalDotsPerInch() returns always 100
    //also have to swap width/height because sailfish does not have display rotation
    physical.setWidth(float(height())/float(445.0));
    physical.setHeight(float(width())/float(445.0));

    physical.rwidth()*=0.0254;//inch to m
    physical.rheight()*=0.0254;
#else
    if(window()!=NULL)
    {
        physical.setWidth(float(width()) / float(window()->screen()->physicalDotsPerInchX()));
        physical.setHeight(float(height()) / float(window()->screen()->physicalDotsPerInchY()));

        physical.rwidth() *= 0.0254;//inch to m
        physical.rheight() *= 0.0254;
    }
#endif
    return physical;
}

const VrProjection *VrItem::projection() const
{
    return &m_config;
}

void VrItem::setGoggles(const VrGoggles &goggles)
{
    m_config.setGoggles(goggles);
    updateProjection(&m_config);
}

void VrItem::synchronize()
{
    if(renderer!=NULL)
        renderer->mutex.lock();
}

void VrItem::desynchronize()
{
    if(renderer!=NULL)
        renderer->mutex.unlock();
}

void VrItem::windowChanged()
{
    if(window()!=NULL)
    {
        connect(window(), &QQuickWindow::beforeRendering, this, &VrItem::setupRenderer, Qt::DirectConnection);
        updateSize();
    }
}

void VrItem::setupRenderer()
{
    disconnect(this, SLOT(setupRenderer()));
    if(renderer==NULL)
    {
        renderer=createRenderer();
        //r->updateProjection(&m_config);
    }
}
