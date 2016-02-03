#include "vrrendersplit2d.h"

#include <QVector4D>
#define OPENGL_OFFSET(i) ((char *)NULL + (i))

VrRenderSplit2D::VrRenderSplit2D():
    projection(NULL), resized(false), inited(false), texture_size(0,0)
{
}

VrRenderSplit2D::~VrRenderSplit2D()
{
}

void VrRenderSplit2D::render(GLuint texture)
{
    if(!inited) init();

    if(resized)
    {
        resizeGL();
        resized=false;
    }

    program.bind();

    if(!vao.isCreated())
    {
        vao.create();
        vao.bind();
        quad.bind();

        program.enableAttributeArray(a_vertex);
        glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),0);

        program.enableAttributeArray(a_coord);
        glVertexAttribPointer(a_coord, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), OPENGL_OFFSET(3*sizeof(float)));
    }
    else
        vao.bind();

    program.setUniformValue(u_matrix, ortho);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 12);
    vao.release();

    program.release();
}

void VrRenderSplit2D::updateProjection(const VrProjection *projection)
{
    this->projection=projection;

    QSize canvas_size=projection->canvasSize();
    float d = canvas_size.width()*projection->goggles()->interpupillaryDistance()/2.0;

    texture_size.setHeight(canvas_size.height());
    texture_size.setWidth(canvas_size.width()/2.0+2.0*d);
    resized=true;
    ortho.setToIdentity();
    ortho.ortho(0, projection->canvasSize().width(), 0, projection->canvasSize().height(), 0.0, 1.0);
}

QSize VrRenderSplit2D::textureSize()
{
    return texture_size;
}

bool VrRenderSplit2D::init()
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

    a_vertex = program.attributeLocation("vertex");
    a_coord  = program.attributeLocation("coord");
    u_source = program.uniformLocation("source");
    u_matrix = program.uniformLocation("matrix");

    resizeGL();

    program.bind();
    program.setUniformValue(u_source,0);
    program.release();

    return true;
}

void VrRenderSplit2D::resizeGL()
{
    QSize canvas_size=projection->canvasSize();
    float d = projection->goggles()->interpupillaryDistance()/2.0*canvas_size.width();

    const float height=texture_size.height();
    const float middle=canvas_size.width()/2.0;

    float quad_data[]={
         0.0f - d*2.0f          , 0.0f    , 0.0f,    0.0f, 0.0f,
         middle                 , 0.0f    , 0.0f,    1.0f, 0.0f,
         0.0f - d*2.0f          , height , 0.0f,          0.0f, 1.0f,

         0.0f - d*2.0f          , height , 0.0f,          0.0f, 1.0f,
         middle                 , height , 0.0f,          1.0f, 1.0f,
         middle                 , 0.0f    , 0.0f,    1.0f, 0.0f,

         middle                 , 0.0f    , 0.0f,    0.0f, 0.0f,
         middle+middle+d*2.0f   , 0.0f    , 0.0f,    1.0f, 0.0f,
         middle                 , height , 0.0f,          0.0f, 1.0f,

         middle                 , height , 0.0f,          0.0f, 1.0f,
         middle+middle+d*2.0f   , height , 0.0f,          1.0f, 1.0f,
         middle+middle+d*2.0f   , 0.0f    , 0.0f,    1.0f, 0.0f
    };

    if(quad.isCreated())
        quad.destroy();

    quad.create();
    quad.bind();
    quad.allocate(quad_data, sizeof(quad_data));
    quad.release();

    vao.destroy();
}

const char *VrRenderSplit2D::vsh =
    "attribute highp vec3 vertex;\n"
    "attribute lowp vec2 coord;\n"
    "uniform highp mat4 matrix;\n"
    "varying lowp vec2 f_texcoord;\n"
    "void main() {\n"
    "   f_texcoord = coord;\n"
    "   gl_Position = matrix * vec4(vertex,1.0);\n"
    "}\n";

const char *VrRenderSplit2D::fsh =
    "varying lowp vec2 f_texcoord;\n"
    "uniform sampler2D source;\n"
    "void main() {\n"
    "   lowp vec4 color=texture2D(source, f_texcoord);\n"
    "   gl_FragColor = color;\n"
    "}\n";
