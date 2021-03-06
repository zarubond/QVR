#include "itemrenderer.h"
#include "item.h"

ItemRenderer::ItemRenderer(const Item *item):
    VrItemRenderer(item), inited(false)
{

}

void ItemRenderer::renderScene(int width, int height)
{
    const QQuaternion & rotation = sensor.reading()->rotation;

    QQuaternion rot;

    rot.setX(-rotation.y());
    rot.setY(-rotation.x());
    rot.setZ(rotation.z());
    rot.setScalar(rotation.scalar());
    rot*=QQuaternion::fromAxisAndAngle(1.0, 0.0, 0.0, 90);


    QMatrix4x4 right_vp = projection()->rightViewProjection(rot);
    QMatrix4x4 left_vp = projection()->leftViewProjection(rot);

    if(!inited)
    {
        initGL();
        inited = true;
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width/2.0, height);
    renderEye(left_vp);
    glViewport(width/2.0, 0, width/2.0, height);
    renderEye(right_vp);
}

void ItemRenderer::updateProjection(const VrProjection *projection)
{

}

void ItemRenderer::renderEye(const QMatrix4x4 &vp)
{
    QMatrix4x4 mvp(vp);
    mvp.translate(-5, 0, 0);
    cube.render(mvp);
}

void ItemRenderer::initGL()
{
    cube.initGL();
}
