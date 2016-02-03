#include "vrdistortionmesh.h"

#include <QDebug>

VrDistortionMesh::VrDistortionMesh():
    arrayBuffer(QOpenGLBuffer::VertexBuffer), elementBuffer(QOpenGLBuffer::IndexBuffer), rows(40), cols(40)
{
}


void VrDistortionMesh::createProjection(const VrProjection *config, bool rotate)
{
    float * vertexData = new float[rows * cols * 4 * 2];

    nIndices = (rows - 1) * (cols - 1) * 6 * 2;
    unsigned short * indexData = new unsigned short[nIndices];

    QSizeF screen_size = config->viewSizeM();

    float mBorderSizeMeters = 0.0030F;
    VrProjection::Viewport leftViewport = config->viewport(config->leftFOV(), 0.0);
    VrProjection::Viewport rightViewport = config->viewport(config->rightFOV(), leftViewport.width);

    float screenWidthM = screen_size.width();
    float screenHeightM = screen_size.height();
    float xEyeOffsetMScreen = screenWidthM / 2.0F  - config->goggles()->interpupillaryDistance() / 2.0F;
    float yEyeOffsetMScreen = config->goggles()->verticalDistanceToLensCenter() - mBorderSizeMeters;

    float textureWidthM = leftViewport.width + rightViewport.width;
    float textureHeightM = std::max(leftViewport.height, rightViewport.height);

    create(config->goggles(), leftViewport, vertexData, indexData,
            screenWidthM, screenHeightM, xEyeOffsetMScreen, yEyeOffsetMScreen, textureWidthM, textureHeightM, rotate);

    xEyeOffsetMScreen = screenWidthM - xEyeOffsetMScreen;

    create(config->goggles(), rightViewport, &vertexData[rows * cols * 4], &indexData[(rows - 1) * (cols - 1) * 6],
            screenWidthM, screenHeightM, xEyeOffsetMScreen, yEyeOffsetMScreen, textureWidthM, textureHeightM, rotate);

    for(int i = 0; i < (rows - 1) * (cols - 1) * 6; i++)       
        indexData[(rows - 1) * (cols - 1) * 6 + i] += rows*cols;

    int vertexLength = rows * cols * 4 * 2;
    int indexLength = nIndices;

    if(arrayBuffer.isCreated())
        arrayBuffer.destroy();

    arrayBuffer.create();
    arrayBuffer.bind();
    arrayBuffer.allocate(vertexData, vertexLength* sizeof(float));
    arrayBuffer.release();

    if(elementBuffer.isCreated())
        elementBuffer.destroy();

    elementBuffer.create();
    elementBuffer.bind();
    elementBuffer.allocate(indexData, indexLength * sizeof(unsigned short));
    elementBuffer.release();

    delete [] vertexData;
    delete [] indexData;
}

void VrDistortionMesh::create(const VrGoggles *distortion, const VrProjection::Viewport &viewport, float *vertexData, unsigned short *indexData, float screenWidthM, float screenHeightM, float xEyeOffsetMScreen, float yEyeOffsetMScreen, float textureWidthM, float textureHeightM, bool rotate)
{
    const float xEyeOffsetMTexture = viewport.eyeX;
    const float yEyeOffsetMTexture = viewport.eyeY;
    const float viewportXMTexture = viewport.x;
    const float viewportYMTexture = viewport.y;
    const float viewportWidthMTexture = viewport.width;
    const float viewportHeightMTexture = viewport.height;

    int vertexLength = 0;

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            float uTexture = float(col) / float(cols-1.0f) * (viewportWidthMTexture / textureWidthM) + viewportXMTexture / textureWidthM;
            float vTexture = float(row) / float(rows-1.0f) * (viewportHeightMTexture / textureHeightM) + viewportYMTexture / textureHeightM;

            float xTextureEye = uTexture * textureWidthM - xEyeOffsetMTexture;
            float yTextureEye = vTexture * textureHeightM - yEyeOffsetMTexture;

            float rTexture = (float) sqrt(xTextureEye * xTextureEye + yTextureEye * yTextureEye);
            float textureToScreen = rTexture > 0.0f ? distortion->distortInverse(rTexture) / rTexture : 1.0f;

            float uScreen = (xTextureEye * textureToScreen + xEyeOffsetMScreen) / screenWidthM;
            float vScreen = (yTextureEye * textureToScreen + yEyeOffsetMScreen) / screenHeightM;

            if(rotate)
            {
                vertexData[(vertexLength + 0)] = (2.0f * vScreen - 1.0f);
                vertexData[(vertexLength + 1)] = (2.0f * uScreen - 1.0f);
            }
            else
            {
                vertexData[(vertexLength + 0)] = (2.0f * uScreen - 1.0f);
                vertexData[(vertexLength + 1)] = (2.0f * vScreen - 1.0f);
            }
            vertexData[(vertexLength + 2)] = uTexture;
            vertexData[(vertexLength + 3)] = vTexture;
            vertexLength += 4;
        }
    }

    int indexOffset = 0;

    for(int row=0; row< rows-1; row++)
    {
        for(int col=0; col< cols-1; col++)
        {
            indexData[indexOffset++]= (row+0)*cols + col +0;
            indexData[indexOffset++]= (row+1)*cols + col +0;
            indexData[indexOffset++]= (row+0)*cols + col +1;

            indexData[indexOffset++]= (row+1)*cols + col +0;
            indexData[indexOffset++]= (row+1)*cols + col +1;
            indexData[indexOffset++]= (row+0)*cols + col +1;
        }
    }
}
