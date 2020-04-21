#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 inputColor;
out vec3 color_rgb;

void main() {
    color_rgb = inputColor;
    gl_Position = vec4(position, 0.0, 1.0);
}
