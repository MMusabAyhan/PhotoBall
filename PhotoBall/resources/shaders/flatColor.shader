#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
};

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = vec4(1.f,1.f,1.f,1.f);
}