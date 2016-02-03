#ifndef VRGOGGLES_H
#define VRGOGGLES_H

#include <QString>
/**
 * @brief The VrGoggles class is a storage for all virtual realty goggles parameters
 */
class VrGoggles
{
public:
    VrGoggles();
    /**
     * @brief VrGoggles
     * @param name
     * @param interpupillaryDistance
     * @param verticalDistanceToLensCenter
     * @param lensDiameter
     * @param screenToLensDistance
     * @param eyeToLensDistance
     * @param coefficients Distortion coefficients
     */
    VrGoggles(const QString &name, float interpupillaryDistance, float verticalDistanceToLensCenter, float lensDiameter, float screenToLensDistance,
              float eyeToLensDistance, float coefficients[4]);

    /**
     * @brief interpupillaryDistance
     * @return Distance between eyes in m
     */
    float interpupillaryDistance() const;
    /**
     * @brief verticalDistanceToLensCenter
     * @return Vertical distance to lens center in m.
     */
    float verticalDistanceToLensCenter() const;
    /**
     * @brief lensDiameter
     * @return Diameter of lenses in m
     */
    float lensDiameter() const;
    /**
     * @brief screenToLensDistance
     * @return Screen to lens distance in m.
     */
    float screenToLensDistance() const;
    /**
     * @brief eyeToLensDistance
     * @return
     */
    float eyeToLensDistance() const;

    /**
     * @brief distortInverse Apply goggles's lens inverse distortion
     * @param radius
     * @return Distorted radius.
     */
    float distortInverse(float radius) const;
    /**
     * @brief distort Apply goggle's lens distotion on given radius
     * @param radius
     * @return Distorted radius
     */
    float distort(float radius)const;
    /**
     * @brief distortionFactor
     * @param radius
     * @return Factor of distortion
     */
    float distortionFactor(float radius)const;
    /**
     * @brief coefficients
     * @return Four distortion paramares
     */
    const float *coefficients() const;
    /**
     * @brief name
     * @return Name of the goggles
     */
    QString name() const;

private:
    float abs(float val) const;

    float m_interpupillaryDistance;
    float m_verticalDistanceToLensCenter;
    float m_lensDiameter;
    float m_screenToLensDistance;
    float m_eyeToLensDistance;
    float m_coefficients[4];
    QString m_name;
};

#endif // VRGOGGLES_H
