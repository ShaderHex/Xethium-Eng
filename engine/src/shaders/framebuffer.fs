#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
    // vec3 col = texture(screenTexture, 1.0).rgb;
    FragColor = texture(screenTexture, TexCoords);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}