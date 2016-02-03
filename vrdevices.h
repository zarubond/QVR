#ifndef VRDEVICES
#define VRDEVICES

#include "vrgoggles.h"
#include <QString>
#include <QList>
#include <QMap>

class VrDevices{
    VrDevices();//do no create instances
public:

    const VrGoggles *device(const QString & name) const;
    QList<QString> devices() const;

    const static VrGoggles cardboard;
    const static VrGoggles colorcross;
private:
    QMap<QString, const VrGoggles*> data;
};

#endif // VRDEVICES

