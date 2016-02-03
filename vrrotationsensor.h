#ifndef ROTATIONSENSOR_H
#define ROTATIONSENSOR_H

#include <QObject>
#include <QQuaternion>
#include <QRotationSensor>
#include <QGyroscope>
#include <QElapsedTimer>

#include <cmath>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <jni.h>
#endif
/**
 * @brief The RotationEvent class
 */
class RotationEvent
{
public:
    /**
     * @brief rotation Ratation from the sensor
     */
    QQuaternion rotation;
    /**
     * @brief euler
     * @return An euler representation of the rotation.
     */
    QVector3D euler() const
    {
        float x, y, z;
        float angle = 2 * acos(rotation.scalar());
        double s = sqrt(1-rotation.scalar()*rotation.scalar()); // assuming quaternion normalised then w is less than 1, so term always positive.
        if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
          // if s close to zero then direction of axis not important
          x = rotation.x(); // if it is important that axis is normalised then replace with x=1; y=z=0;
          y = rotation.y();
          z = rotation.z();
        } else {
          x = rotation.x() / s; // normalise axis
          y = rotation.y() / s;
          z = rotation.z() / s;
        }
        return QVector3D(x*angle, y*angle, z*angle);
    }
};
/**
 * @brief The VrRotationSensor class which represents a rotation sensor
 */
class VrRotationSensor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief VrRotationSensor
     * @param enable_compass Enable correction of rotation to the magnetic field of earth
     * @param parent
     */
    explicit VrRotationSensor(bool enable_compass=false, QObject *parent = 0);
    ~VrRotationSensor();
    /**
     * @brief start Start the sensor
     */
    void start();
    /**
     * @brief stop Stop the sensor
     */
    void stop();
    /**
     * @brief reading
     * @return Last reaging value.
     */
    const RotationEvent * reading();

signals:
    /**
     * @brief rotationChanged Inform about new reading avilability
     */
    void rotationChanged(const RotationEvent *);

public slots:

protected:
    bool filter(QRotationReading * reading);
    bool filter(QGyroscopeReading * reading);

private:

    class Rotation: public QRotationFilter
    {
    public:
        Rotation(VrRotationSensor * sensor): sensor(sensor)
        {
            rotation.addFilter(this);
        }

        void start()
        {
            rotation.start();
        }

        void stop()
        {
            rotation.stop();
        }

    protected:
        bool filter(QRotationReading * reading) override
        {
            return sensor->filter(reading);
        }

    private:
        VrRotationSensor * sensor;
        QRotationSensor rotation;
    };

    class Gyroscope: public QGyroscopeFilter
    {
    public:
        Gyroscope(VrRotationSensor * sensor): sensor(sensor)
        {
            gyroscope.addFilter(this);
        }

        void start()
        {
            gyroscope.start();
        }

        void stop()
        {
            gyroscope.stop();
        }

    protected:
        bool filter(QGyroscopeReading * reading) override
        {
            return sensor->filter(reading);
        }

    private:
        VrRotationSensor * sensor;
        QGyroscope gyroscope;
    };

    bool enable_compass;
    static VrRotationSensor * self_ptr;
    RotationEvent event;
#ifdef Q_OS_ANDROID
    void initJNI();
    static void onRotationChanged(JNIEnv *, jobject, jfloat w, jfloat a, jfloat b, jfloat c);
    QAndroidJniObject * java_backend;
#endif

    Rotation rotation;
    Gyroscope gyroscope;
    QQuaternion quaternionGyroscope, deltaQuaternion;
    QQuaternion quaternionRotationVector;
    bool positionInitialised;
    quint64 timestamp;
    double gyroscopeRotationVelocity;
};

#endif // ROTATIONSENSOR_H
