#ifndef VRRENDERFULLSCREEN_H
#define VRRENDERFULLSCREEN_H

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include "vrprojection.h"
/**
 * @brief The VrRenderFullscreen class helps you render given texture all over you screen.
 */
class VrRenderFullscreen: public QOpenGLFunctions
{
public:
    VrRenderFullscreen();
    ~VrRenderFullscreen();
    /**
     * @brief render Render the texture on the screen.
     * @param texture
     */
    void render(GLuint texture);
    /**
     * @brief updateProjection Update rendering paramerers
     */
    void updateProjection(const VrProjection *);

private:
    bool init();
    void createVAO();

    QOpenGLShaderProgram program;
    QOpenGLBuffer canvas_buffer;
    QOpenGLVertexArrayObject vao;

    int a_coord;
    int u_tex_color;

    bool inited;
    static const char *vsh, *fsh;
};

#endif // VRRENDERFULLSCREEN_H
