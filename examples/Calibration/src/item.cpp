#include "item.h"

Item::Item()
{

}

ItemRenderer *Item::createRenderer() const
{
    return new ItemRenderer(this);
}

QQuickItem *Item::gui() const
{
    return m_gui;
}

void Item::updateProjection(const VrProjection *projection)
{
    resizeGui(projection);
}

void Item::keyPressEvent(QKeyEvent *event)
{
    if(event->key() ==  Qt::Key_Up){
        float coef[4];
        coef[0] = projection()->goggles()->coefficients()[0];
        coef[1] = projection()->goggles()->coefficients()[1]+1;
        coef[2] = projection()->goggles()->coefficients()[2];
        coef[3] = projection()->goggles()->coefficients()[3];
        VrGoggles goggles(projection()->goggles()->name(), projection()->goggles()->interpupillaryDistance(), projection()->goggles()->verticalDistanceToLensCenter(),
                          projection()->goggles()->lensDiameter(), projection()->goggles()->screenToLensDistance(), projection()->goggles()->eyeToLensDistance(),
            coef);
        setGoggles(goggles);
    }
    else if(event->key() == Qt::Key_Down){
        float coef[4];
        coef[0] = projection()->goggles()->coefficients()[0];
        coef[1] = projection()->goggles()->coefficients()[1]-1;
        coef[2] = projection()->goggles()->coefficients()[2];
        coef[3] = projection()->goggles()->coefficients()[3];
        VrGoggles goggles(projection()->goggles()->name(), projection()->goggles()->interpupillaryDistance(), projection()->goggles()->verticalDistanceToLensCenter(),
                          projection()->goggles()->lensDiameter(), projection()->goggles()->screenToLensDistance(), projection()->goggles()->eyeToLensDistance(),
            coef);
        setGoggles(goggles);
    }
    else if(event->key() == Qt::Key_Right) {
        float coef[4];
        coef[0] = projection()->goggles()->coefficients()[0];
        coef[1] = projection()->goggles()->coefficients()[1];
        coef[2] = projection()->goggles()->coefficients()[2]+1;
        coef[3] = projection()->goggles()->coefficients()[3];
        VrGoggles goggles(projection()->goggles()->name(), projection()->goggles()->interpupillaryDistance(), projection()->goggles()->verticalDistanceToLensCenter(),
                          projection()->goggles()->lensDiameter(), projection()->goggles()->screenToLensDistance(), projection()->goggles()->eyeToLensDistance(),
            coef);
        setGoggles(goggles);
    }
    else if(event->key() == Qt::Key_Left) {
        float coef[4];
        coef[0] = projection()->goggles()->coefficients()[0];
        coef[1] = projection()->goggles()->coefficients()[1];
        coef[2] = projection()->goggles()->coefficients()[2]-1;
        coef[3] = projection()->goggles()->coefficients()[3];
        VrGoggles goggles(projection()->goggles()->name(), projection()->goggles()->interpupillaryDistance(), projection()->goggles()->verticalDistanceToLensCenter(),
                          projection()->goggles()->lensDiameter(), projection()->goggles()->screenToLensDistance(), projection()->goggles()->eyeToLensDistance(),
            coef);
        setGoggles(goggles);
    }
}

void Item::setGui(QQuickItem *gui)
{
    if (m_gui == gui)
        return;

    m_gui = gui;

    if(m_gui!=NULL)
    {
        //to enable rendering to QSGTextureProvider
        QObject * layer = qvariant_cast<QObject *>(m_gui->property("layer"));
        layer->setProperty("enabled", true);
        layer->setProperty("type",GL_RGBA);
        if(width()!=0 && height()!=0)
            resizeGui(projection());
    }

    emit guiChanged(gui);
}

void Item::resizeGui(const VrProjection *projection)
{
    if(m_gui!=NULL && width()!=0 && height()!=0)
    {
        QSize canvas_size=projection->canvasSize();
        float d = canvas_size.width()*projection->goggles()->interpupillaryDistance()/2.0;
        QSize size;
        size.setHeight(canvas_size.height());
        size.setWidth(canvas_size.width()/2.0+2.0*d);
        m_gui->setSize(size);
    }
}
