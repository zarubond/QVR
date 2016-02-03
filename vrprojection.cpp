#include "vrprojection.h"

#define DEG_TO_RAD 0.0174532925
#define RAD_TO_DEG 57.2957795

#include <QDebug>

VrProjection::VrProjection(): m_dpm(0)
{
}

VrProjection::~VrProjection()
{
}

const VrProjection::FieldOfView & VrProjection::leftFOV() const
{
    return leftEyeFov;
}

const VrProjection::FieldOfView &VrProjection::rightFOV() const
{
    return rightEyeFov;
}

const VrGoggles *VrProjection::goggles() const
{
    return &m_goggles;
}

VrProjection::Viewport VrProjection::viewport(const FieldOfView& fov,float xOffsetM) const
{
    float eyeToScreenDistanceM = m_goggles.eyeToLensDistance() + m_goggles.screenToLensDistance();
    float leftM     = (float) tan(DEG_TO_RAD*(fov.left()))   * eyeToScreenDistanceM;
    float rightM    = (float) tan(DEG_TO_RAD*(fov.right()))  * eyeToScreenDistanceM;
    float bottomM   = (float) tan(DEG_TO_RAD*(fov.bottom())) * eyeToScreenDistanceM;
    float topM      = (float) tan(DEG_TO_RAD*(fov.top()))    * eyeToScreenDistanceM;

    Viewport vp;
    vp.x = xOffsetM;
    vp.y = 0.0F;
    vp.width = (leftM + rightM);
    vp.height = (bottomM + topM);
    vp.eyeX = (leftM + xOffsetM);
    vp.eyeY = bottomM;

    return vp;
}

void VrProjection::resize(const QSizeF & screen_size_m, const QSize & screen_size)
{
    this->view_size=screen_size;
    this->view_size_m=screen_size_m;

    float borderSizeMeters = 0.0030f;
    float ideal_fov_angle = (float) RAD_TO_DEG*(atan2(m_goggles.lensDiameter() / 2.0F, m_goggles.eyeToLensDistance()));

    float eye_to_screen = m_goggles.eyeToLensDistance() + m_goggles.screenToLensDistance();
    float outerDist     = (screen_size_m.width() - m_goggles.interpupillaryDistance()) / 2.0F;
    float innerDist     = m_goggles.interpupillaryDistance() / 2.0F;
    float bottomDist    = m_goggles.verticalDistanceToLensCenter() - borderSizeMeters;
    float topDist       = screen_size_m.height() + borderSizeMeters - m_goggles.verticalDistanceToLensCenter();

    float outerAngle    = (float) RAD_TO_DEG*(atan2(m_goggles.distort(outerDist), eye_to_screen));
    float innerAngle    = (float) RAD_TO_DEG*(atan2(m_goggles.distort(innerDist), eye_to_screen));
    float bottomAngle   = (float) RAD_TO_DEG*(atan2(m_goggles.distort(bottomDist), eye_to_screen));
    float topAngle      = (float) RAD_TO_DEG*(atan2(m_goggles.distort(topDist), eye_to_screen));

    leftEyeFov.setLeft(   std::min(outerAngle , ideal_fov_angle));
    leftEyeFov.setRight(  std::min(innerAngle , ideal_fov_angle));

    leftEyeFov.setBottom( std::min(bottomAngle, ideal_fov_angle));
    leftEyeFov.setTop(    std::min(topAngle   , ideal_fov_angle));

    rightEyeFov.setLeft(  std::min(innerAngle , ideal_fov_angle));
    rightEyeFov.setRight( std::min(outerAngle , ideal_fov_angle));

    rightEyeFov.setBottom(std::min(bottomAngle, ideal_fov_angle));
    rightEyeFov.setTop(   std::min(topAngle   , ideal_fov_angle));

    float xPxPerM = screen_size.width() / screen_size_m.width();
    float yPxPerM = screen_size.height() / screen_size_m.height();
    m_dpm = (xPxPerM+yPxPerM)/2.0;

    //canvas size

    Viewport leftViewport=viewport(leftEyeFov,0.0);
    Viewport rightViewport=viewport(rightEyeFov,leftViewport.width);

    float textureWidthM = leftViewport.width + rightViewport.width;
    float textureHeightM = std::max(leftViewport.height, rightViewport.height);

    int textureWidthPx = textureWidthM * m_dpm;
    int textureHeightPx = textureHeightM * m_dpm;
    canvas_size=QSize(textureWidthPx, textureHeightPx);
}

QSize VrProjection::canvasSize() const
{
    return canvas_size;
}

QSize VrProjection::viewSize() const
{
    return view_size;
}

QSizeF VrProjection::viewSizeM() const
{
    return view_size_m;
}

void VrProjection::setGoggles(const VrGoggles &goggles)
{
    m_goggles=goggles;
    resize(view_size_m,view_size);
}

float VrProjection::dpm() const
{
    return m_dpm;
}

float VrProjection::unitToM() const
{
    QMatrix4x4 s=leftEyeFov.toPerspectiveMatrix(0.1,2000);
    QVector4D vec(0.0, 1.0, 0.0, 1.0);
    QVector4D sc=s*vec;

    return (view_size_m.height()/2.0)/sc.y();
}

QMatrix4x4 VrProjection::rightViewProjection(const QQuaternion &rotation) const
{
    float halfInterpupillaryDistance = m_goggles.interpupillaryDistance()/2.0;

    QMatrix4x4 right=rightEyeFov.toPerspectiveMatrix(0.1,1000);
    right.rotate(rotation);

    QMatrix4x4 r;
    r.setToIdentity();
    r.translate(QVector3D(-halfInterpupillaryDistance,0.0,0.0));

    return right*r;
}

QMatrix4x4 VrProjection::leftViewProjection(const QQuaternion &rotation) const
{
    float halfInterpupillaryDistance = m_goggles.interpupillaryDistance()/2.0;

    QMatrix4x4 left=leftEyeFov.toPerspectiveMatrix(0.1,1000);

    left.rotate(rotation);

    QMatrix4x4 l;
    l.setToIdentity();
    l.translate(QVector3D(halfInterpupillaryDistance,0.0,0.0));

    return left*l;
}

float VrProjection::FieldOfView::left() const
{
    return mLeft;
}

void VrProjection::FieldOfView::setLeft(float left)
{
    mLeft = left;
}

float VrProjection::FieldOfView::right() const
{
    return mRight;
}

void VrProjection::FieldOfView::setRight(float right)
{
    mRight = right;
}
float VrProjection::FieldOfView::bottom() const
{
    return mBottom;
}

void VrProjection::FieldOfView::setBottom(float bottom)
{
    mBottom = bottom;
}

float VrProjection::FieldOfView::top() const
{
    return mTop;
}

void VrProjection::FieldOfView::setTop(float top)
{
    mTop = top;
}

QMatrix4x4 VrProjection::FieldOfView::toPerspectiveMatrix(float near, float far) const
{
    float l = (float) -tan(qDegreesToRadians(mLeft)) * near;
    float r = (float) tan(qDegreesToRadians(mRight)) * near;
    float b = (float) -tan(qDegreesToRadians(mBottom)) * near;
    float t = (float) tan(qDegreesToRadians(mTop)) * near;
    QMatrix4x4 matrix;
    matrix.frustum(l,r,b,t,near,far);
    return matrix;
}
