#include "vrdevices.h"
/*
const VrGogglesDevices::Device VrGogglesDevices::cardboard={
    0.06f,  //interpupillaryDistance
    0.035f, //verticalDistanceToLensCenter
    0.025f, //lensDiameter
    0.037f, //screenToLensDistance
    0.011f, //eyeToLensDistance
    0.06f,  //visibleViewportSize

    1.0f,     //coefficients[0]
    250.0f,   //coefficients[1]
    50000.0f, //coefficients[2]
    0.0f      //coefficients[3]
};

const VrGogglesDevices::Device VrGogglesDevices::colorcross={
    0.065f, //interpupillaryDistance
    0.035f, //verticalDistanceToLensCenter
    0.034f, //lensDiameter;
    0.075f, //screenToLensDistance
    0.011f, //eyeToLensDistance
    0.06f,  //visibleViewportSize

    1.0f,   //coefficients[0]
    128,    //coefficients[1]
    256,    //coefficients[2]
    0.0f    //coefficients[3]
};*/


VrDevices::VrDevices()
{
    data.insert(cardboard.name(), &cardboard);
    data.insert(colorcross.name(), &colorcross);
}

const VrGoggles *VrDevices::device(const QString &name) const
{
    return data.value(name);
}

QList<QString> VrDevices::devices() const
{
    return data.keys();
}

const VrGoggles VrDevices::cardboard("cardboard",
                               0.06f,  //interpupillaryDistance
                               0.035f, //verticalDistanceToLensCenter
                               0.025f, //lensDiameter
                               0.037f, //screenToLensDistance
                               0.011f, //eyeToLensDistance
                     (float[]){1.0f,     //coefficients[0]
                               250.0f,   //coefficients[1]
                               50000.0f, //coefficients[2]
                               0.0f      //coefficients[3]
                               });

const VrGoggles VrDevices::colorcross("colorcross",
            0.065f, //interpupillaryDistance
            0.035f, //verticalDistanceToLensCenter
            0.034f, //lensDiameter;
            0.075f, //screenToLensDistance
            0.025f, //eyeToLensDistance

(float[]){  1.0f,   //coefficients[0]
            128,    //coefficients[1]
            256,    //coefficients[2]
            0.0f    //coefficients[3]
        });
