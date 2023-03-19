#version 330

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform vec3 car_position; // aici
uniform vec3 minimap;


layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;


out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_texture;

void main()
{
    vec3 word_position2 = (Model * vec4(v_position, 1)).xyz;
    //vec3 word_position_car2 = (Model * vec4(car_position, 1)).xyz;

    vec3 result = car_position - word_position2;
    float module = length(result) * length(result);

    word_position2.y = word_position2.y - module * 0.01f;

    frag_color      = object_color;

    if(minimap.x == 0) {
        gl_Position = Projection * View *  vec4(word_position2, 1.f);
    } else {
        gl_Position = Projection * View * Model * vec4(v_position, 1);
    }
    
   //gl_Position = Projection * View * Model * vec4(v_position, 1);
}
