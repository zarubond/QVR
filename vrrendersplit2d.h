#ifndef VRRENDERGOOGLES2D_H
#define VRRENDERGOOGLES2D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "vrprojection.h"
/**
 * @brief The VrRenderSplit2D class help you render giver texture side by side. The is used to help desing UI in VR
 */
class VrRenderSplit2D: public QOpenGLFunctions
{
public:
    VrRenderSplit2D();
    ~VrRenderSplit2D();
    /**
     * @brief render Render the texture side by side.
     * @param texture
     */
    void render(GLuint texture);

    /**
     * @brief updateProjection Update projection parameters.
     * @param projection
     */
    void updateProjection(const VrProjection *projection);
    QSize textureSize();

private:
    bool init();
    void resizeGL();

    int a_vertex, a_coord, u_source, u_matrix;
    QOpenGLShaderProgram program;
    QOpenGLBuffer quad;
    QOpenGLVertexArrayObject vao;

    const VrProjection *projection;
    static const char *vsh, *fsh;
    QMatrix4x4 ortho;
    bool resized;
    bool inited;
    QSize texture_size;
};

#endif // VRRENDERGOOGLES2D_H
