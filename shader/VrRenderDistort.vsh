attribute lowp vec2 aPosition;
attribute highp float aVignette;
attribute mediump vec2 aTextureCoord;
varying   highp vec2 f_texcoord;
varying   highp float vVignette;

void main() {

    f_texcoord = aTextureCoord;
    vVignette = aVignette;

    gl_Position = vec4(aPosition, 0.0, 1.0);
}
