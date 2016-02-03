#ifndef VRDISTORTIONMESH_H
#define VRDISTORTIONMESH_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <cmath>
#include <QDebug>

#include "vrgoggles.h"
#include "vrprojection.h"
/**
 * @brief The VrDistortionMesh class dummu object ot create mesh of pincushion distortion.
 */
class VrDistortionMesh
{
public:
    VrDistortionMesh();

    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer elementBuffer;
    int nIndices;
    /**
     * @brief createProjection Create a mesh and upload it into OpenGL buffers
     * @param config Configuration of projeciton
     * @param rotete Swap the X and Y axis, in case that you OS doens not support screen rotation (Sailfish OS)
     */
    void createProjection(const VrProjection *config, bool rotate=false);
private:

    void create(const VrGoggles * distortion, const VrProjection::Viewport &viewport, float *vertexData, unsigned short *indexData, float screenWidthM, float screenHeightM, float xEyeOffsetMScreen, float yEyeOffsetMScreen, float textureWidthM, float textureHeightM, bool rotate);
    const int rows, cols;
};

#endif // VRDISTORTIONMESH_H
