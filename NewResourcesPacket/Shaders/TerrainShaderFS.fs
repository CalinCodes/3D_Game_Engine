precision mediump float;
varying vec3 v_color;
varying vec3 v_worldPos;

varying vec2 v_uv, v_uv2;
uniform vec2 u_uvOffset;
uniform mat4 u_model;

uniform sampler2D u_texture_0; // BlendMap
uniform sampler2D u_texture_1; // Rock
uniform sampler2D u_texture_2; // Dirt
uniform sampler2D u_texture_3; // Grass

uniform vec3 u_cameraPos;
uniform vec3 u_fogColor;
uniform float u_smallRadius;
uniform float u_largeRadius;
void main()
{
    vec2 uv_with_offset = v_uv + u_uvOffset;
	vec4 c_blend = texture2D(u_texture_0, v_uv2);
    vec4 c_rock = texture2D(u_texture_1, uv_with_offset);
    vec4 c_dirt = texture2D(u_texture_2, uv_with_offset);
    vec4 c_grass = texture2D(u_texture_3, uv_with_offset);

    vec4 c_final = c_dirt * c_blend.r + c_rock * c_blend.g + c_grass * c_blend.g;

    float d = distance(u_cameraPos, v_worldPos);
    float alpha = clamp((d - u_smallRadius) / (u_largeRadius - u_smallRadius), 0.0, 1.0);
    vec3 foggedColor = mix(c_final.rgb, u_fogColor, alpha);
    
    gl_FragColor = vec4(foggedColor.rgb, 1.0);
    //alpha = (d - u_smallRadius)/(u_largeRadius - u_smallRadius);
    //gl_FragColor = vec4(alpha, 0.0, 0.0, 1.0);
}
