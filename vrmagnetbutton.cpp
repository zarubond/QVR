#include "vrmagnetbutton.h"

VrMagnetButton::VrMagnetButton()
{
    connect(&sensor, SIGNAL(readingChanged()), this, SLOT(readingChanged()));
    pressed=false;
    stable=true;
}

VrMagnetButton::~VrMagnetButton()
{

}

bool VrMagnetButton::start()
{
    if(!sensor.isActive())
        sensor.start();
    return sensor.isActive();
}

bool VrMagnetButton::isPressed()
{
    return pressed;
}

void VrMagnetButton::readingChanged()
{
    const QMagnetometerReading * reading = sensor.reading();
    readingList.push_back(QVector3D(reading->x(), reading->y(), reading->z()));

    if(readingList.size()>40)
        readingList.pop_front();

    if(readingList.size()==40)
        update();
}

void VrMagnetButton::update()
{
    QVector3D base=readingList.back();
    float min=INT_MAX, max=INT_MIN;
    for(int i=0;i<20;i++)
    {
        QVector3D tmp=readingList[i]-base;
        float mag=tmp.length();
        if(mag<min)
            min=mag;
    }

    for(int i=20;i<40;i++)
    {
        QVector3D tmp=readingList[i]-base;
        float mag=tmp.length();
        if(mag>max)
            max=mag;
    }
    min*=1000;
    max*=1000;
    if(min<0.1 && max>0.6)
    {
        if(stable)
        {
            stable=false;
            emit buttonPressed();
        }
    }
    else
    {
        if(min<0.01 && max<0.01)
            stable=true;
    }
}

