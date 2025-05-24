precision mediump float;
attribute vec3 a_posL;
uniform mat4 uMVP;
varying vec3 v_coord;

void main() {
    v_coord = a_posL;
    gl_Position = uMVP * vec4(a_posL, 1.0);
}