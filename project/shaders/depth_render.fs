#version 330
in vec2 fragTexCoord;
uniform sampler2D depthTexture;
uniform bool flipY;

const float nearPlane = 0.1;
const float farPlane = 100.0;

out vec4 finalColor;

float LinearizeDepth(float depth)
{
    return (2.0*nearPlane)/(farPlane + nearPlane - depth*(farPlane - nearPlane));
}

void main()
{
    vec2 uv = fragTexCoord;
    if(flipY) uv.y = 1.0 - uv.y;

    float depth = texture(depthTexture, uv).r;
    float linearDepth = LinearizeDepth(depth);

    // Normalize to [0,1] for display
    float normalized = (linearDepth - nearPlane) / (farPlane - nearPlane);
    finalColor = vec4(vec3(normalized), 1.0);
}
