#version 410

uniform vec4 vColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 camera;
//uniform mat4 projection;

in vec4 vPosition;
in vec2 vTexture;

out vec4 color;
out vec2 texCoord;

void main()
{
    //gl_Position = view*model*vPosition;
    gl_Position = view*camera*model*vPosition;
    color = vColor;
    texCoord = vTexture;
}
