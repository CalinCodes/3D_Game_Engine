precision mediump float;

const int MAX_LIGHTS = 10;

varying vec2 v_uv;
varying vec3 v_worldPos;
varying vec3 v_worldNormal;

uniform sampler2D u_texture;
uniform vec3 u_cameraPos;

uniform vec3 u_ambientColor;
uniform float u_ambientRatio;

uniform int u_numLights;
uniform vec3 u_lightPositions[MAX_LIGHTS];
uniform vec3 u_lightDirections[MAX_LIGHTS];
uniform vec3 u_lightDiffuse[MAX_LIGHTS];
uniform vec3 u_lightSpecular[MAX_LIGHTS];
uniform float u_lightSpecPower[MAX_LIGHTS];
uniform int u_lightTypes[MAX_LIGHTS];
uniform float u_lightSpotCutoff[MAX_LIGHTS];
uniform float u_lightSpotExponent[MAX_LIGHTS];

uniform float u_kspec;
uniform float u_kdiff;

void main()
{
    vec4 c_obj = texture2D(u_texture, v_uv);
    if (c_obj.a < 0.1) discard;

    vec3 color = vec3(0.0);

    vec3 Camb = u_ambientColor * u_ambientRatio * c_obj.rgb;

    vec3 N = normalize(v_worldNormal);
    vec3 E = normalize(u_cameraPos - v_worldPos);

    for (int i = 0; i < MAX_LIGHTS; ++i) {
        if (i >= u_numLights) break;

        vec3 L;

        if (u_lightTypes[i] == 0) {
            L = normalize(u_lightPositions[i] - v_worldPos);
        } else if (u_lightTypes[i] == 1) {
            L = normalize(-u_lightDirections[i]);
        } else if (u_lightTypes[i] == 2) {
            L = normalize(u_lightPositions[i] - v_worldPos);
            vec3 D = normalize(u_lightDirections[i]);
            float spotEffect = dot(-D, L);
            if (spotEffect < cos(radians(u_lightSpotCutoff[i]))) continue;
            float intensity = pow(spotEffect, u_lightSpotExponent[i]);
            L *= intensity;
        }

        float diff = max(dot(N, L), 0.0);
        vec3 Cdiff = u_lightDiffuse[i] * diff * c_obj.rgb;

        vec3 R = reflect(-L, N);
        float spec = pow(max(dot(R, E), 0.0), u_lightSpecPower[i]);
        vec3 Cspec = u_lightSpecular[i] * spec;

        color += u_kdiff * Cdiff + u_kspec * Cspec;
    }

    vec3 Cfinal = Camb + color;

    gl_FragColor = vec4(Cfinal, c_obj.a);
}