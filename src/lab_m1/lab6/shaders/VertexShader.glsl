#version 330

// Input
// TODO(student): Get vertex attributes from each location

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;


layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;


// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_texture;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_normal = v_normal;


    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(frag_position, 1.0);
}
