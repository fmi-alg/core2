#version 330 core
in vec3 color_rgb;
out vec4 color_rgba;

void main() {
    color_rgba = vec4(color_rgb, 1.0);
}
