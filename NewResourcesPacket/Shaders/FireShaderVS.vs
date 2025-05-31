attribute vec3 a_posL;
attribute vec2 a_uv;

//uniform mat4 u_model;
uniform mat4 uMVP;

varying vec2 v_uv;
//varying vec3 v_worldPos;

void main() {
    v_uv = a_uv;

    //v_worldPos = (u_model * vec4(a_posL, 1.0)).xyz;

    gl_Position = uMVP * vec4(a_posL, 1.0);
}