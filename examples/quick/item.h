#ifndef ITEM_H
#define ITEM_H

#include "../../vritem.h"
#include "itemrenderer.h"

class Item : public VrItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem * gui READ gui WRITE setGui NOTIFY guiChanged)
public:
    Item();
    ItemRenderer * createRenderer() const override;
    QQuickItem * gui() const;
public slots:
    void setGui(QQuickItem * gui);
signals:
    void guiChanged(QQuickItem * gui);
private:
    QQuickItem * m_gui;
};

#endif // ITEM_H
