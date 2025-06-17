precision mediump float;

attribute vec3 a_posL;
attribute vec3 u_normal;
attribute vec2 a_uv;

uniform mat4 uMVP;
uniform mat4 u_model;

varying vec3 v_worldPos;
varying vec3 v_worldNormal;
varying vec2 v_uv;

void main()
{
	v_uv = a_uv;
    vec4 worldPos = u_model * vec4(a_posL, 1.0);
	v_worldPos = worldPos.xyz;

    v_worldNormal = normalize(mat3(u_model) * u_normal);

	gl_Position = uMVP * vec4(a_posL, 1.0);
}
   