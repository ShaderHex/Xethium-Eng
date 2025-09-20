#version 330 core

in vec3 fragPosition;

uniform samplerCube environmentMap;
uniform bool vflipped;

out vec4 finalColor;

vec3 toneMapACES(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{
    vec3 dir = fragPosition;
    if (vflipped) dir.y = -dir.y;

    vec3 color = texture(environmentMap, dir).rgb;

    float horizonFade = pow(1.0 - clamp(dir.y, 0.0, 1.0), 2.0);
    color *= 0.8 + 0.2*horizonFade;

    color = toneMapACES(color);
    color = pow(color, vec3(1.0/2.2));

    finalColor = vec4(color, 1.0);
}
