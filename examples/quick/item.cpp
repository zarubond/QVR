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

void Item::setGui(QQuickItem *gui)
{
    if (m_gui == gui)
        return;

    m_gui = gui;
    //emit guiChanged(gui);
}

