#ifndef VRRENDERGOGGLES_H
#define VRRENDERGOGGLES_H

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "vrdistortionmesh.h"
#include "vrprojection.h"
/**
 * @brief The VrRenderGoggles class helps you render given texture for your goggles
 */
class VrRenderGoggles: public QOpenGLFunctions
{
public:
    VrRenderGoggles();
    ~VrRenderGoggles();
    /**
     * @brief render Render the given texture side by side for your goggles.
     * @param texture
     */
    void render(GLuint texture);
    /**
     * @brief updateProjection Update parameters of the projection.
     * @param projection
     */
    void updateProjection(const VrProjection *projection);

private:
    bool init();
    void createProjection(const VrProjection *config);
    void createVAO();

    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject vao;
    VrDistortionMesh mesh;

    int u_texture;
    int a_position, a_texture_coord;
    bool inited;
    static const char *fsh, *vsh;
};

#endif // VRRENDERGOGGLES_H
