#ifndef ITEMRENDERER_H
#define ITEMRENDERER_H

#include "../../vritemrenderer.h"
#include "../../vrrotationsensor.h"
#include "cube.h"

class Item;

class ItemRenderer : public VrItemRenderer
{
    Q_OBJECT
public:
    ItemRenderer(const Item * item);
    void renderScene(int width, int height) override;

protected:
    void updateProjection(const VrProjection *projection) override;
private:
    void renderEye(const QMatrix4x4 & vp);
    void initGL();
    VrRotationSensor sensor;
    bool inited;
    Cube cube;

};

#endif // ITEMRENDERER_H
