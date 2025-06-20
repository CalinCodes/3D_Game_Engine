precision mediump float;

attribute vec3 a_posL;          // Local position of the vertex
attribute vec3 u_normal;        // Local normal of the vertex
attribute vec2 a_uv;            // UV coordinates

uniform mat4 uMVP;              // Model-View-Projection matrix
uniform mat4 u_model;           // Model matrix

varying vec2 v_uv;              // UV coordinates passed to the fragment shader
varying vec3 v_worldPos;        // World position passed to the fragment shader
varying vec3 v_worldNormal;

void main()
{
    v_uv = a_uv;

    vec4 worldPos = u_model * vec4(a_posL, 1.0);
    v_worldPos = worldPos.xyz;

    v_worldNormal = normalize(mat3(u_model) * u_normal);

    gl_Position = uMVP * vec4(a_posL, 1.0);
}