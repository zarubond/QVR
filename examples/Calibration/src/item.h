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
protected:
    void updateProjection(const VrProjection * projection);
    void keyPressEvent(QKeyEvent *event);
public slots:
    void setGui(QQuickItem * gui);
signals:
    void guiChanged(QQuickItem * gui);
private:
    QQuickItem * m_gui;
    void resizeGui(const VrProjection *projection);
};

#endif // ITEM_H
