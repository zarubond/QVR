#include "vrrenderfullscreen.h"

#include <QDebug>
#include <QGuiApplication>

#define OPENGL_OFFSET(i) ((char *)NULL + (i))

VrRenderFullscreen::VrRenderFullscreen():
    canvas_buffer(QOpenGLBuffer::VertexBuffer), a_coord(0), u_tex_color(0), inited(false)
{
}

VrRenderFullscreen::~VrRenderFullscreen()
{

}

bool VrRenderFullscreen::init()
{
    inited=true;
    initializeOpenGLFunctions();

    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsh);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsh);
    if(!program.link())
    {
        qDebug()<<"Shader link error: "<<program.log();
        return false;
    }

    if((a_coord = program.attributeLocation("coord")) == -1)
        return false;

    if((u_tex_color = program.uniformLocation("tex_color")) == -1)
        return false;

    program.bind();
    program.setUniformValue(u_tex_color, 0);
    program.release();

    static const GLfloat quad_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    canvas_buffer.create();
    canvas_buffer.bind();
    canvas_buffer.allocate(sizeof(quad_data));
    canvas_buffer.write(0, quad_data, sizeof(quad_data));
    canvas_buffer.release();

    return true;
}

void VrRenderFullscreen::updateProjection(const VrProjection *)
{
}

void VrRenderFullscreen::createVAO()
{
    vao.create();
    vao.bind();
    canvas_buffer.bind();

    program.enableAttributeArray(a_coord);
    glVertexAttribPointer(a_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

    vao.release();
}

void VrRenderFullscreen::render(GLuint texture)
{
    if(!inited) init();

    program.bind();
    glBindTexture(GL_TEXTURE_2D, texture);

    if(!vao.isCreated())
        createVAO();

    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.release();

    program.release();
}

const char *VrRenderFullscreen::vsh=" \
attribute highp vec3 coord; \
varying highp vec2 f_texcoord; \
\
void main(void) \
{ \
    f_texcoord = (coord.xy + vec2(1.0, 1.0)) / 2.0; \
    gl_Position =  vec4(coord, 1.0); \
} \
";
const char *VrRenderFullscreen::fsh="\
uniform sampler2D tex_color; \
varying highp vec2 f_texcoord; \
\
void main(void) \
{ \
    highp vec2 uv=vec2(f_texcoord.x, 1.0 - f_texcoord.y); \
    gl_FragData[0] = texture2D(tex_color, uv); \
} \
";

