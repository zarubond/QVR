#include "vrrendergoggles.h"
#include <QDebug>

#define OPENGL_OFFSET(i) ((char *)NULL + (i))

#define DEG_TO_RAD 0.0174532925
#define RAD_TO_DEG 57.2957795

VrRenderGoggles::VrRenderGoggles():
    inited(false)
{
}

VrRenderGoggles::~VrRenderGoggles()
{
}

bool VrRenderGoggles::init()
{
    inited=true;
    initializeOpenGLFunctions();

    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsh);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsh);
    if(!program.link())
    {
        qDebug()<<"Shader link error:"<<program.log();
        return false;
    }

    if((u_texture = program.uniformLocation("uTextureSampler")) == -1) return false;
    if((a_position = program.attributeLocation("aPosition")) == -1) return false;
    if((a_texture_coord = program.attributeLocation("aTextureCoord")) == -1) return false;

    program.bind();
    program.setUniformValue(u_texture, 0);
    program.release();

    return true;
}

void VrRenderGoggles::updateProjection(const VrProjection *config)
{
#ifdef Q_OS_SAILFISH
    mesh.createProjection(config, true);
#else
    mesh.createProjection(config, false);
#endif
    vao.destroy();
}

void VrRenderGoggles::createVAO()
{
    vao.create();
    vao.bind();
    mesh.arrayBuffer.bind();

    program.enableAttributeArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 16, OPENGL_OFFSET(0*4));

    program.enableAttributeArray(a_texture_coord);
    glVertexAttribPointer(a_texture_coord, 2, GL_FLOAT, GL_FALSE, 16, OPENGL_OFFSET(2*4));

    mesh.elementBuffer.bind();

    vao.release();
}

void VrRenderGoggles::render(GLuint texture)
{
    if(!inited) init();

    if(!vao.isCreated())  createVAO();

    program.bind();
    glBindTexture(GL_TEXTURE_2D, texture);

    vao.bind();
    glDrawElements(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_SHORT, OPENGL_OFFSET(0));//9126
    vao.release();

    program.release();
}

const char * VrRenderGoggles::fsh = "\
varying highp vec2 f_texcoord; \n \
uniform sampler2D uTextureSampler; \n \
\n \
void main() \n \
{ \n \
    highp vec2 uv=vec2(f_texcoord.x, 1.0 - f_texcoord.y); \n \
    //highp vec2 uv=vec2(f_texcoord.x, f_texcoord.y); \n \
    gl_FragColor = texture2D(uTextureSampler, uv); \n \
} \n \
";

const char * VrRenderGoggles::vsh = "\
attribute lowp vec2 aPosition; \
attribute highp float aVignette; \
attribute mediump vec2 aTextureCoord;\
varying   highp vec2 f_texcoord; \
varying   highp float vVignette; \
\
void main() \
{ \
\
    f_texcoord = aTextureCoord; \
    vVignette = aVignette; \
\
    gl_Position = vec4(aPosition, 0.0, 1.0); \
} \
";
