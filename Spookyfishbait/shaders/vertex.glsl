#version 330
// Specify the input locations of attributes
layout(location=0) in vec3 positionAttrIn;      // vertex position
layout(location=1) in vec3 normalAttrIn;        // vertex normal
layout(location=2) in vec2 textureAttrIn;       // vertex texture

// Specify the Uniforms of the vertex shaders
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalsMatrix;


// Specify the outputs of the vertex shader
// These will be the input for the fragment shader
out vec3 positionAttr;
out vec3 normalAttr;
out vec2 textureAttr;

void main()
{
    positionAttr = positionAttrIn;
    normalAttr = normalAttrIn;
    textureAttr = textureAttrIn;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(positionAttrIn,1.0);
}
