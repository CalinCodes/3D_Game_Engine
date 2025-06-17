precision mediump float;

uniform sampler2D u_texture_0;
uniform samplerCube u_texture_1; // Skybox texture
uniform vec3 u_cameraPos;

varying vec3 v_worldPos;
varying vec3 v_worldNormal;
varying vec2 v_uv;

uniform float u_reflection;

void main() {
    vec3 viewDir = normalize(u_cameraPos - v_worldPos);
    vec3 reflectDir = reflect(viewDir, normalize(v_worldNormal));
    vec4 c_reflexie = textureCube(u_texture_1, reflectDir);
    vec4 c_proprie = texture2D(u_texture_0, v_uv);
    float alpha = c_proprie.a;
    vec4 c_final = c_reflexie * u_reflection + c_proprie * (1.0 - u_reflection);
    c_final.a = alpha;

    if (c_final.a<0.1) discard;

    gl_FragColor = c_final;
}