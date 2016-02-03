#include "vrrotationsensor.h"


#include <QDebug>
#include <QtMath>

//http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/
//http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
QQuaternion euler2quaternion(double heading, double attitude, double bank) {
   // Assuming the angles are in radians.
   double c1 = cos(heading/2.0);
   double s1 = sin(heading/2.0);
   double c2 = cos(attitude/2.0);
   double s2 = sin(attitude/2.0);
   double c3 = cos(bank/2.0);
   double s3 = sin(bank/2.0);
   double c1c2 = c1*c2;
   double s1s2 = s1*s2;
   double w =c1c2*c3 - s1s2*s3;
   double x =c1c2*s3 + s1s2*c3;
   double y =s1*c2*c3 + c1*s2*s3;
   double z =c1*s2*c3 - s1*c2*s3;

   return QQuaternion(w, z, x, y);
}

VrRotationSensor::VrRotationSensor(bool enable_compass, QObject *parent) :
    QObject(parent), enable_compass(enable_compass), rotation(this), gyroscope(this)
{
    self_ptr=this;
    timestamp=0;
    enable_compass=false;
    event.rotation=QQuaternion::fromAxisAndAngle(1.0, 0.0 ,0.0, 90);
#ifdef Q_OS_ANDROID
    if(enable_compass)
        initJNI();
#else
    enable_compass=false;
#endif
}

VrRotationSensor::~VrRotationSensor()
{
    if(!enable_compass)
    {
        //sensor.removeFilter(this);
        //sensor.stop();
        stop();
    }
}

void VrRotationSensor::start()
{
#ifdef Q_OS_ANDROID
    enable_compass=false;
    if(enable_compass)
    {
        java_backend->callMethod<void>("start", "()V");
    }
    else
    {
        gyroscope.start();
        rotation.start();
    }
#else
    gyroscope.start();
    rotation.start();
#endif
    timestamp = 0;
    positionInitialised=false;
}

void VrRotationSensor::stop()
{
#ifdef Q_OS_ANDROID
    if(enable_compass)
        java_backend->callMethod<void>("stop", "()V");
    else
    {
        gyroscope.stop();
        rotation.stop();
    }
#else
    gyroscope.stop();
    rotation.stop();
#endif
}

const RotationEvent *VrRotationSensor::reading()
{
    return &event;
}

bool VrRotationSensor::filter(QRotationReading *reading)
{
   quaternionRotationVector=euler2quaternion(qDegreesToRadians(reading->z()),
                                    qDegreesToRadians(reading->x()),
                                    qDegreesToRadians(reading->y()));
   quaternionRotationVector.normalize();

   if (!positionInitialised) {
        // Override
        quaternionGyroscope=quaternionRotationVector;
        positionInitialised = true;
    }

    return true;
}

bool VrRotationSensor::filter(QGyroscopeReading *reading)
{
    const double EPSILON = 0.1f;

    double dT = (reading->timestamp()- timestamp)/1000000.0; //microsecond to second
    timestamp = reading->timestamp();

    // Axis of the rotation sample, not normalized yet.
    double axisX = reading->x()*0.0174532925;
    double axisY = reading->y()*0.0174532925;
    double axisZ = reading->z()*0.0174532925;

    // Calculate the angular speed of the sample
    gyroscopeRotationVelocity = sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);

    // Normalize the rotation vector if it's big enough to get the axis
    if (gyroscopeRotationVelocity > EPSILON) {
        axisX /= gyroscopeRotationVelocity;
        axisY /= gyroscopeRotationVelocity;
        axisZ /= gyroscopeRotationVelocity;
    }

    // Integrate around this axis with the angular speed by the timestep
    // in order to get a delta rotation from this sample over the timestep
    // We will convert this axis-angle representation of the delta rotation
    // into a quaternion before turning it into the rotation matrix.
    double thetaOverTwo = gyroscopeRotationVelocity * dT / 2.0f;
    double sinThetaOverTwo = sin(thetaOverTwo);
    double cosThetaOverTwo = cos(thetaOverTwo);
    deltaQuaternion.setX(sinThetaOverTwo * axisX);
    deltaQuaternion.setY(sinThetaOverTwo * axisY);
    deltaQuaternion.setZ(sinThetaOverTwo * axisZ);
    deltaQuaternion.setScalar(cosThetaOverTwo);


    const double OUTLIER_THRESHOLD = 0.85f;
    const double INDIRECT_INTERPOLATION_WEIGHT = 0.01f;

    // Move current gyro orientation
    quaternionGyroscope *= deltaQuaternion;

    // Calculate dot-product to calculate whether the two orientation sensors have diverged
    // (if the dot-product is closer to 0 than to 1), because it should be close to 1 if both are the same.
#ifdef Q_OS_SAILFISH
    float dotProd = 0.0;
#else
    float dotProd = QQuaternion::dotProduct(quaternionGyroscope, quaternionRotationVector);
#endif
    //if the difference between angles is to big just use the rotation vector
    if (fabs(dotProd) < OUTLIER_THRESHOLD)
    {
        quaternionGyroscope = quaternionRotationVector;
    }
    else if (fabs(dotProd) < 0.999) // if it smaller then try to get there slowly
    {
        quaternionGyroscope = QQuaternion::slerp( quaternionGyroscope, quaternionRotationVector, dT);
    }
    else
    {
        // Both are nearly saying the same. Perform normal fusion.
        quaternionGyroscope = QQuaternion::slerp( quaternionGyroscope, quaternionRotationVector, INDIRECT_INTERPOLATION_WEIGHT * gyroscopeRotationVelocity);
    }

    event.rotation=quaternionGyroscope;
    emit rotationChanged(&event);

    return true;
}


#ifdef Q_OS_ANDROID
void VrRotationSensor::initJNI()
{
    JNINativeMethod methods[] {
        {"rotationChanged", "(FFFF)V", reinterpret_cast<void *>(VrRotationSensor::onRotationChanged)}};

    if (QAndroidJniObject::isClassAvailable("com/zarubond/lookingglass/RotationSensor"))
    {
        java_backend=new QAndroidJniObject("com/zarubond/lookingglass/RotationSensor","()V");
        if(java_backend->isValid())
        {
            QAndroidJniEnvironment env;
            jclass objectClass = env->GetObjectClass(java_backend->object<jobject>());
            env->RegisterNatives(objectClass, methods,  sizeof(methods) / sizeof(methods[0]));
            env->DeleteLocalRef(objectClass);
        }
    }
}

void VrRotationSensor::onRotationChanged(JNIEnv *, jobject, jfloat w, jfloat a, jfloat b, jfloat c)
{
   // self_ptr->event.rotation=QQuaternion(w,a,b,c);
   // qDebug()<<"A:"<<self_ptr->event.rotation;
   // qDebug()<<QQuaternion(w,a,b,c);
  //  emit self_ptr->rotationChanged(&self_ptr->event);
}
#endif

VrRotationSensor * VrRotationSensor::self_ptr=NULL;
