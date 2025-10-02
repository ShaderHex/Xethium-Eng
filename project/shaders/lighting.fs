#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

#define MAX_LIGHTS 4
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

struct Light {
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
    int castsShadows;
    sampler2D shadowMap;  // This should be a depth texture
    mat4 lightMatrix;
};

uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

out vec4 finalColor;

float CalculateShadow(int lightIndex, vec3 fragPos) {
    if (lights[lightIndex].castsShadows == 0) 
        return 0.0;

    // Transform fragment position to light space
    vec4 lightSpacePos = lights[lightIndex].lightMatrix * vec4(fragPos, 1.0);
    
    // Perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    
    // Transform to [0,1] range for texture sampling
    projCoords = projCoords * 0.5 + 0.5;
    
    // Check if fragment is outside the shadow map
    if (projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // Sample from the DEPTH texture
    float closestDepth = texture(lights[lightIndex].shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    // Add bias to prevent shadow acne
    float bias = 0.005;
    
    // Check if current fragment is in shadow
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord) * fragColor * colDiffuse;
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);

    vec3 lightAccum = vec3(0.0);
    vec3 specularAccum = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (lights[i].type == -1) continue;

        vec3 lightDir = vec3(0.0);
        float attenuation = 1.0;
        float shadow = 0.0;

        if (lights[i].type == LIGHT_DIRECTIONAL) {
            lightDir = -normalize(lights[i].target - lights[i].position);
            shadow = CalculateShadow(i, fragPosition);
        }
        else if (lights[i].type == LIGHT_POINT) {
            lightDir = normalize(lights[i].position - fragPosition);
            float distance = length(lights[i].position - fragPosition);
            attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        }

        float shadowFactor = 1.0 - shadow;
        float NdotL = max(dot(normal, lightDir), 0.0) * shadowFactor * attenuation;

        lightAccum += lights[i].color.rgb * NdotL;

        if (NdotL > 0.0) {
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0) * shadowFactor * attenuation;
            specularAccum += lights[i].color.rgb * spec;
        }
    }

    vec3 litColor = texelColor.rgb * (ambient.rgb + lightAccum) + specularAccum;
    finalColor = vec4(litColor, texelColor.a);
}