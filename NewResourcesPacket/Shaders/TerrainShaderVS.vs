precision mediump float;
attribute vec3 a_posL,a_color;
attribute vec2 a_uv, a_uv2;

uniform mat4 uMVP;
uniform mat4 u_model;
uniform vec3 u_height;
uniform sampler2D u_texture_0;
precision mediump float;
uniform vec2 u_uvOffset;

varying vec2 v_uv, v_uv2;
varying vec3 v_worldPos;

attribute vec3 u_normal;
varying vec3 v_worldNormal;

void main()
{
	v_uv = a_uv + u_uvOffset;
	v_uv2 = a_uv2 + u_uvOffset;

	vec4 c_blend = texture2D(u_texture_0, v_uv2);
	vec4 new_pos = vec4(a_posL, 1.0);

	new_pos.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	v_worldPos = (u_model * vec4(a_posL, 1.0)).xyz;
	v_worldNormal = normalize(mat3(u_model) * u_normal);

	gl_Position = uMVP * new_pos;
}