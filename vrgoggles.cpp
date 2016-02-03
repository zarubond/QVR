#include "vrgoggles.h"

#include <QDebug>

VrGoggles::VrGoggles()
{
    //default values for google cardboard
    m_interpupillaryDistance        = 0.06F;
    m_verticalDistanceToLensCenter  = 0.035F;
    m_lensDiameter                  = 0.025F;
    m_screenToLensDistance          = 0.037F;
    m_eyeToLensDistance             = 0.011F;

    m_coefficients[0] = 1.0f;
    m_coefficients[1] = 250.0f;
    m_coefficients[2] = 50000.0f;
    m_coefficients[3] = 0.0f;
}

VrGoggles::VrGoggles(const QString &name, float interpupillaryDistance, float verticalDistanceToLensCenter, float lensDiameter, float screenToLensDistance,
                     float eyeToLensDistance, float coefficients[4])
{
    m_interpupillaryDistance        = interpupillaryDistance;
    m_verticalDistanceToLensCenter  = verticalDistanceToLensCenter;
    m_lensDiameter                  = lensDiameter;
    m_screenToLensDistance          = screenToLensDistance;
    m_eyeToLensDistance             = eyeToLensDistance;

    m_coefficients[0] = coefficients[0];
    m_coefficients[1] = coefficients[1];
    m_coefficients[2] = coefficients[2];
    m_coefficients[3] = coefficients[3];

    m_name=name;
}

float VrGoggles::interpupillaryDistance() const
{
    return m_interpupillaryDistance;
}

float VrGoggles::verticalDistanceToLensCenter() const
{
    return m_verticalDistanceToLensCenter;
}

float VrGoggles::lensDiameter() const
{
    return m_lensDiameter;
}

float VrGoggles::screenToLensDistance() const
{
    return m_screenToLensDistance;
}

float VrGoggles::eyeToLensDistance() const
{
    return m_eyeToLensDistance;
}

const float *VrGoggles::coefficients() const
{
    return m_coefficients;
}

QString VrGoggles::name() const
{
    return m_name;
}

float VrGoggles::distortionFactor(float radius) const
{
    const float r2 = radius * radius;
    return m_coefficients[0] + m_coefficients[1] * r2 + m_coefficients[2] * r2 * r2 + m_coefficients[3] *r2 * r2 * r2;
}

float VrGoggles::distort(float radius) const
{
    return radius * distortionFactor(radius);
}

//http://peterabeles.com/blog/?p=73
float VrGoggles::distortInverse(float radius) const
{
    //secant methond
    float r0 = radius / 0.9F;
    float r1 = radius * 0.9F;

    float dr0 = radius - distort(r0);
    while (abs(r1 - r0) > 0.0001f)
    {
        float dr1 = radius - distort(r1);
        float r2 = r1 - dr1 * ((r1 - r0) / (dr1 - dr0));
        r0 = r1;
        r1 = r2;
        dr0 = dr1;
    }

    return r1;
}

float VrGoggles::abs(float val) const
{
    if(val<0) return -val;
    return val;
}
