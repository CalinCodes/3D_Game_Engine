precision mediump float;
varying vec3 v_color;
varying vec2 v_uv, v_uv2;
uniform sampler2D u_texture_0; // BlendMap
uniform sampler2D u_texture_1; // Rock
uniform sampler2D u_texture_2; // Dirt
uniform sampler2D u_texture_3; // Grass
uniform vec2 u_uvOffset;
void main()
{
    vec2 uv_with_offset = v_uv + u_uvOffset;
	vec4 c_blend = texture2D(u_texture_0, v_uv2);
    vec4 c_rock = texture2D(u_texture_1, uv_with_offset);
    vec4 c_dirt = texture2D(u_texture_2, uv_with_offset);
    vec4 c_grass = texture2D(u_texture_3, uv_with_offset);
    vec4 c_final = c_dirt * c_blend.r + c_rock * c_blend.g + c_grass * c_blend.g;

    gl_FragColor = vec4(c_final.rgb, 1.0);
}
