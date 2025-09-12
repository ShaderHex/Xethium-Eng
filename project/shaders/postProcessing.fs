#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform float exposure = 1.0;

void main() {
    vec3 color = texture(texture0, fragTexCoord).rgb;

    color = vec3(1.0) - exp(-color * exposure);

    fragColor = vec4(color, 1.0);
}
