#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

#define MAX_LIGHTS 4
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

struct Light {
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 lightAccum = vec3(0.0);
    vec3 specularAccum = vec3(0.0);
    vec4 tint = colDiffuse * fragColor;

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        vec3 lightDir = vec3(0.0);

        if (lights[i].type == LIGHT_DIRECTIONAL)
        {
            lightDir = -normalize(lights[i].target - lights[i].position);
        }
        else if (lights[i].type == LIGHT_POINT)
        {
            lightDir = normalize(lights[i].position - fragPosition);
        }

        float NdotL = max(dot(normal, lightDir), 0.0);
        lightAccum += lights[i].color.rgb * NdotL;

        if (NdotL > 0.0)
        {
            float spec = pow(max(0.0, dot(viewD, reflect(-lightDir, normal))), 16.0);
            specularAccum += spec;
        }
    }

    // final color
    vec3 litColor = (tint.rgb + specularAccum) * lightAccum + (ambient.rgb / 5.0) * tint.rgb;
    finalColor = vec4(pow(litColor, vec3(1.0/2.2)), 1.0);
}
