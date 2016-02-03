#ifndef ITEMRENDERER_H
#define ITEMRENDERER_H

#include "../../vritemrenderer.h"
#include "../../vrrotationsensor.h"
#include "../../vrrendersplit2d.h"
#include "cube.h"

#include <QSGDynamicTexture>
#include <QSGTextureProvider>

class Item;

class ItemRenderer : public VrItemRenderer
{
    Q_OBJECT
public:
    ItemRenderer(const Item * item);
    void renderScene(int width, int height) override;
public slots:
    void setGUI(QQuickItem *item);
protected:
    void updateProjection(const VrProjection *projection) override;
private:
    void renderEye(const QMatrix4x4 & vp);
    void initGL();
    VrRotationSensor sensor;
    VrRenderSplit2D split;
    bool inited;
    Cube cube;
    const QQuickItem * gui;
};

#endif // ITEMRENDERER_H
