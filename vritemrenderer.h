#ifndef GOGGLESITEMRENDERER_H
#define GOGGLESITEMRENDERER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#define QT_OPENGL_ES_3
#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include "./vrprojection.h"
#include "./vrrendergoggles.h"
#include "./vrrenderfullscreen.h"

class VrItem;
/**
 * @brief The VrItemRenderer class id pure virtual class which you have to override in otder to render 3d
 */
class VrItemRenderer: public QObject
{
    Q_OBJECT
public:
    VrItemRenderer(const VrItem * item);
    virtual ~VrItemRenderer();
    /**
     * @brief renderScene In this method
     * @param width
     * @param height
     */
    virtual void renderScene(int width, int height)=0;
    /**
     * @brief update Force update of the graphics
     */
    void update();
    /**
     * @brief projection
     * @return Parameterd of the rendering system.
     */
    const VrProjection *projection() const;
    /**
     * @brief setDistortion
     * @param enable Trun off/on rendering with distortion
     */
    void setDistortion(bool enable);
    /**
     * @brief distortion
     * @return Is distortion enabled.
     */
    bool distortion();
protected:
    virtual void updateProjection(const VrProjection *projection);
    virtual void synchronize();
private slots:

    void windowChanged(QQuickWindow * window);
    void afterRendering();
    void beforeSynchronizing();
    void visibleChanged();
    void selfDestruct();
private:
    friend class VrItem;
    void createFramebuffer(int width, int height);
    void deleteFramebuffer();

    VrRenderGoggles goggles_render;
    VrRenderFullscreen fullscreen_render;
    VrProjection config_copy;
    QMutex mutex;
    const VrItem * item;
    GLuint texture, framebuffer, depth_buffer;
    bool m_distortion;
    bool visible;
};

#endif // GOGGLESITEMRENDERER_H
