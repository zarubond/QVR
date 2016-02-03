#ifndef VRSETUP_H
#define VRSETUP_H

#include <QMatrix4x4>
#include <QtMath>

#include "vrgoggles.h"

/**
 * @brief The VrProjection class holds properties of rendering system
 */
class VrProjection
{
public:
    VrProjection();
    ~VrProjection();

    class Viewport{
    public:
        float x;
        float y;
        float width;
        float height;
        float eyeX;
        float eyeY;
    };

    class FieldOfView{

    public:
        FieldOfView(): mLeft(0),mRight(0),mBottom(0),mTop(0){}

        float left() const;
        void setLeft(float left);

        float right() const;
        void setRight(float right);

        float bottom() const;
        void setBottom(float bottom);

        float top() const;
        void setTop(float top);

        QMatrix4x4 toPerspectiveMatrix(float near, float far) const;

    private:
        float mLeft;
        float mRight;
        float mBottom;
        float mTop;
    };

    const FieldOfView &leftFOV() const;
    const FieldOfView &rightFOV() const;
    /**
     * @brief viewport
     * @param fov
     * @param xOffsetM
     * @return
     */
    Viewport viewport(const FieldOfView& fov, float xOffsetM) const;
    /**
     * @brief updateFov
     * @param screen_size_m Size of screen in m
     * @param screen_size Size of screen in px
     */
    void resize(const QSizeF &view_size_m, const QSize &view_size);
    /**
     * @brief canvasSize The canvas is virtual space which should be used for rendering of image for glasses in order to maintain
     * detail all over the rendered goggles.
     * @return Size of virtual canvas in px.
     */
    QSize canvasSize() const;
    /**
     * @brief viewSize
     * @return size of actual screen in px
     */
    QSize viewSize() const;
    /**
     * @brief viewSizeM
     * @return size of actual screen in m
     */
    QSizeF viewSizeM() const;
    /**
     * @brief goggles
     * @return
     */
    const VrGoggles *goggles() const;
    /**
     * @brief setParamas The class will take care of params
     * @param params
     */
    void setGoggles(const VrGoggles& goggles);
    /**
     * @brief dpm
     * @return Display point per meter
     */
    float dpm() const;

    float unitToM() const;
    /**
     * @brief rightViewProjection Apply right eye offset on given rotation
     * @param rotation
     * @return
     */
    QMatrix4x4 rightViewProjection(const QQuaternion & rotation) const;
    /**
     * @brief leftViewProjection Apply left eye offset on giver rotataion
     * @param rotation
     * @return
     */
    QMatrix4x4 leftViewProjection(const QQuaternion & rotation) const;

private:
    FieldOfView leftEyeFov,rightEyeFov;
    VrGoggles m_goggles;
    float m_dpm;
    QSize view_size;
    QSizeF view_size_m;
    QSize canvas_size;
};

#endif // VRSETUP_H
