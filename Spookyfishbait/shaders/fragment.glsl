#version 330
// Specify the inputs to the fragment shader
in vec3 positionAttr;
in vec3 normalAttr;
in vec2 textureAttr;

// Specify the Uniforms of the vertex shaders
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 material;
uniform sampler2D samplerUniform;
uniform int nLights;
uniform vec3 lightPositions[25];
uniform vec4 lightColors[25];

// Specify the output of the fragment shader
// Usually a vec4 describing a color
out vec4 fColor;

vec3 normalize(vec3 vector) {
    return vector / length(vector);
}

void main()
{
    // Color of object at point
    vec4 materialColor = texture(samplerUniform,textureAttr);

    // Set normal, light, reflect and view vectors
    vec3 N = normalize(normalAttr);
    //vec3 V = -positionAttr;
    vec4 V4 = inverse(modelMatrix) * inverse(viewMatrix) * inverse(projectionMatrix) * vec4(0.0, 0.0, -1.0, 0.0);
    vec3 V = V4.xyz;
    V /= length(V);

    for (int i=0; i<nLights; i++) {
        // Light and reflection vector
        vec3 L = normalize(lightPositions[i] - positionAttr);
        vec3 R = 2 * dot(L, N) * N - L;

        // Phong shading
        // ambient
        fColor = lightColors[i] * materialColor * material[0];
        // diffuse
        fColor += max(dot(L, N), 0.0) * lightColors[i] * materialColor * material[1];
        // specular
        fColor += pow(max(dot(R,V), 0.0), material[3]) * lightColors[i] * material[2];
    }

}
