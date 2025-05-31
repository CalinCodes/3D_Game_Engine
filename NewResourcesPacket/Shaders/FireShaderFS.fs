precision mediump float;

uniform sampler2D u_texture_0;   // fire_mask.tga (inverted if needed)
uniform sampler2D u_texture_1;      // fire3.tga
uniform sampler2D u_texture_2;      // DisplacementMap.tga

uniform float u_Time;
uniform float u_DispMax;


varying vec2 v_uv;

void main() {
    vec2 disp = texture2D(u_texture_2, vec2(v_uv.x, v_uv.y + u_Time)).rg;
    vec2 offset = (disp * 2.0 - 1.0) * u_DispMax;
    vec2 v_uv_displaced = v_uv + offset;
    vec4 c_fire = texture2D(u_texture_1, v_uv_displaced);
    vec4 c_alpha = texture2D(u_texture_0, v_uv);
    c_fire.a *= c_alpha.r;
    if (c_fire.a < 0.05) {
        discard;
    }

    gl_FragColor = c_fire;
    //gl_FragColor = vec4(1.0, 0.5, 0.5, 1.0);
}