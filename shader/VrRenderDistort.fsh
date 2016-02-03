varying highp vec2 f_texcoord;
uniform sampler2D uTextureSampler;

void main() {
    highp vec2 uv=vec2(f_texcoord.x, 1.0 - f_texcoord.y);
    //highp vec2 uv=vec2(f_texcoord.x, f_texcoord.y);
    gl_FragColor = texture2D(uTextureSampler, uv);
}
