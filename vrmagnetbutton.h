#ifndef VRMAGNETBUTTON_H
#define VRMAGNETBUTTON_H

#include <QMagnetometer>
#include <QVector3D>
#include <QDebug>
/**
 * @brief The VrMagnetButton class works with magneter to enable magnet button on Google Cardboard
 */
class VrMagnetButton: public QObject
{
    Q_OBJECT
public:
    VrMagnetButton();
    ~VrMagnetButton();
    /**
     * @brief start
     * @return Start reaging of the magnetic buttion.
     */
    bool start();
    /**
     * @brief isPressed
     * @return State of the button
     */
    bool isPressed();
signals:
    /**
     * @brief buttonPressed Inform about pressing of the button.
     */
    void buttonPressed();
private slots:
    void readingChanged();
private:
    QMagnetometer sensor;
    QList<QVector3D> readingList;
    void update();
    bool pressed;
    bool stable;
};

#endif // VRMAGNETBUTTON_H
