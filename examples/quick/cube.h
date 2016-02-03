#ifndef CUBE_H
#define CUBE_H

#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLVersionFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Cube: public QOpenGLFunctions
{
public:
    Cube();

    void render(const QMatrix4x4 &mvp);
    void initGL();
private:
    void setupVertexAttribs();

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLVertexArrayObject *m_vao;
    int u_matrix;
};

#endif // CUBE_H
