precision mediump float;

const int MAX_LIGHTS = 10;

varying vec2 v_uv;                // UV coordinates
varying vec3 v_worldPos;          // World position of the fragment
varying vec3 v_worldNormal;       // World normal of the fragment

uniform sampler2D u_texture;      // Object texture
uniform vec3 u_cameraPos;         // Camera position

uniform vec3 u_ambientColor;      // Ambient light color
uniform float u_ambientRatio;     // Ambient light ratio

uniform int u_numLights;          // Number of lights
uniform vec3 u_lightPositions[MAX_LIGHTS];  // Light positions
uniform vec3 u_lightDirections[MAX_LIGHTS]; // Light directions
uniform vec3 u_lightDiffuse[MAX_LIGHTS];    // Diffuse light colors
uniform vec3 u_lightSpecular[MAX_LIGHTS];   // Specular light colors
uniform float u_lightSpecPower[MAX_LIGHTS]; // Specular power
uniform int u_lightTypes[MAX_LIGHTS];       // Light types (0 = point, 1 = directional, 2 = spot)
uniform float u_lightSpotCutoff[MAX_LIGHTS]; // Spotlight cutoff angles
uniform float u_lightSpotExponent[MAX_LIGHTS]; // Spotlight intensity falloff

uniform float u_kspec;            // Specular coefficient
uniform float u_kdiff;            // Diffuse coefficient

void main()
{
    // Sample the texture color
    vec4 c_obj = texture2D(u_texture, v_uv);
    if (c_obj.a < 0.1) discard; // Discard transparent fragments

    // Initialize the final color
    vec3 color = vec3(0.0);

    // Ambient lighting
    vec3 Camb = u_ambientColor * u_ambientRatio * c_obj.rgb;

    // Normalized normal and view direction
    //vec3 N = normalize(v_worldNormal);
    vec3 N = normalize(v_worldNormal);
    //N = vec3(0.0, 0.0, 1.0);
    vec3 E = normalize(u_cameraPos - v_worldPos);

    // Loop through all lights
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        if (i >= u_numLights) break;

        vec3 L; // Direction to the light

        // Determine light direction based on type
        if (u_lightTypes[i] == 0) { // Point light
            L = normalize(u_lightPositions[i] - v_worldPos);
        } else if (u_lightTypes[i] == 1) { // Directional light
            L = normalize(-u_lightDirections[i]);
        } else if (u_lightTypes[i] == 2) { // Spotlight
            L = normalize(u_lightPositions[i] - v_worldPos);
            vec3 D = normalize(u_lightDirections[i]);
            float spotEffect = dot(-D, L);
            if (spotEffect < cos(radians(u_lightSpotCutoff[i]))) continue; // Outside spotlight cone
            float intensity = pow(spotEffect, u_lightSpotExponent[i]);
            L *= intensity; // Apply spotlight intensity falloff
            //gl_FragColor = vec4(intensity, 0.0, 0.0, c_obj.a);
        }

        // Diffuse lighting
        float diff = max(dot(N, L), 0.0);
        vec3 Cdiff = u_lightDiffuse[i] * diff * c_obj.rgb;

        // Specular lighting
        vec3 R = reflect(-L, N);
        float spec = pow(max(dot(R, E), 0.0), u_lightSpecPower[i]);
        vec3 Cspec = u_lightSpecular[i] * spec;

        // Accumulate diffuse and specular components
        color += u_kdiff * Cdiff + u_kspec * Cspec;
    }

    // Combine ambient, diffuse, and specular components
    vec3 Cfinal = Camb + color;

    // Output the final color
    gl_FragColor = vec4(Cfinal, c_obj.a);
}