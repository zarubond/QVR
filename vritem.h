#ifndef GOGGLESITEM_H
#define GOGGLESITEM_H

#include <QQuickItem>
#include "vritemrenderer.h"
#include "vrprojection.h"

class VrItem : public QQuickItem
{
    Q_OBJECT
public:
    explicit VrItem(QQuickItem *parent = 0);
    /**
     * @brief createRenderer This method is called from render thread
     * @return
     */
    virtual VrItemRenderer *createRenderer() const =0;
    /**
     * @brief physicalSize
     * @return  Physical size of the item in meters
     */
    QSizeF physicalSize() const;
    /**
     * @brief config
     * @return
     */
    const VrProjection *projection() const;
    void setGoggles(const VrGoggles &goggles);
    /**
     * @brief synchronize Synchronize the rendering thread and main thread.
     */
    void synchronize();
    /**
     * @brief desynchronize Desynchronize the rendering thread and main thread.
     */
    void desynchronize();
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    virtual void updateProjection(const VrProjection *){}
private slots:
    void windowChanged();
    void setupRenderer();
private:
    void updateSize();
    VrProjection m_config;
    VrItemRenderer * renderer;
};

#endif // GOGGLESITEM_H
