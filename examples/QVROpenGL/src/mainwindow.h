#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include "../../vrrendergoggles.h"
#include "../../vrrotationsensor.h"
#include "cube.h"

class MainWindow : public QWindow
{
    Q_OBJECT

public:
    MainWindow(QWindow *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *) override;
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
private:
    void render();
    void renderEye(const QMatrix4x4 &vp);
    void initGL();
    void resizeBuffer();
    VrRenderGoggles goggles;
    VrProjection projection;
    VrRotationSensor sensor;
    QOpenGLFramebufferObject * fbo;
    QOpenGLContext *context;
    bool inited;
    QSizeF physicalSize() const;
    Cube cube;
};

#endif // MAINWINDOW_H
