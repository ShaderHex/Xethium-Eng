#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform bool useTexture;

void main() {
    if (useTexture) {
        FragColor = texture(ourTexture, TexCoord);
    } else {
        FragColor = vec4(objectColor, 1.0f);
    }
}
