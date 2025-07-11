// ------------------ Uniforms
// Transforms
uniform mat4 m;
uniform vec3 cameraPosition;

// Material
uniform vec4 uniformColor;
uniform float materialShininess;
uniform float materialSpecularIntensity;

// Illumination
#define MAX_LIGHTS 10
uniform struct Light {
   bool enabled;
   vec3 position;
   vec3 dir;
   vec3 color;
   bool spot;
   bool specularEnabled;
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
} lights[MAX_LIGHTS];

// ------------------ From the vertex shader
in vec3 fragPosition;
in vec4 fragColor;
in vec3 fragNormal;

// ------------------ Resulting color
out vec4 fragmentColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera, bool specularEnabled)
{
    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.spot == false) 			// Directional light
	{
        surfaceToLight = -normalize(light.dir);
        attenuation = 1.0;
    }
	else 								// Point (spot) light
	{
        surfaceToLight = normalize(light.position - surfacePos);
        float distanceToLight = length(surfaceToLight);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

        // Cone restrictions (affects attenuation)
        float lightToSurfaceAngle = acos(dot(-surfaceToLight, normalize(light.dir)));
        if(lightToSurfaceAngle > light.coneAngle)
		{
            attenuation = 0.0;
        }
    }

    // Ambient component
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.color;

    // Diffuse component
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.color;

    // Specular component
	vec3 specular = vec3(0);
	if(specularEnabled)
	{
		float specularCoefficient = 0.0;
		if(diffuseCoefficient > 0.0)
			specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
		specular = specularCoefficient * vec3(materialSpecularIntensity) * light.color;
	}

    // Linear color (before gamma correction)
    return ambient + (attenuation * (diffuse + specular));
}

void main()
{
    //vec3 normal = normalize(transpose(inverse(mat3(m))) * fragNormal);
	vec3 normal = normalize(mat3(m) * fragNormal);  // is equivalent? this allow to work in virtual machines...
    vec3 surfacePos = vec3(m * vec4(fragPosition, 1));
    vec4 surfaceColor = uniformColor * fragColor; //texture(materialTex, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    // Combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		if(lights[i].enabled)
		{
			linearColor += ApplyLight(lights[i], surfaceColor.rgb, normal, surfacePos, surfaceToCamera, lights[i].specularEnabled);
		}
    }

    // Gamma correction
	//fragmentColor = vec4(linearColor, 1);
    vec3 gamma = vec3(1.0/2.2);
    fragmentColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
