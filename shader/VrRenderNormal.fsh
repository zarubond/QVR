uniform sampler2D tex_color;
varying highp vec2 f_texcoord;

void main(void)
{
    highp vec2 uv=vec2(f_texcoord.x, 1.0 - f_texcoord.y);
    gl_FragData[0] = texture2D(tex_color, uv);
}
