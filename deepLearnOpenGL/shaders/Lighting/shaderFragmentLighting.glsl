#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos; 
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;

    sampler2D emision;
    float shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct BasicPhongLighting {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos; 


vec3 CalDirLight(DirLight ligth, vec3 norm, vec3 viewDir);
vec3 CalPointLight(PointLight ligth, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

//Generic function for calc the attenuation value for lights with a limited radius for lighting
float CalAttenuation(vec3 lightPosition, vec3 fragPos, float lightConstant, float lightLinear, float lightQuadratic);

//Generic function for calculating the PhongLighting 
BasicPhongLighting CalBasicPhongLighting(BasicPhongLighting light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 norm = normalize(Normal);
    
    vec3 result = vec3(0.0);
    //Calculating dirLight, like we have the sun above;
    
    result = CalDirLight(dirLight, norm, viewDir);
    

    //Calculating 4 differents PointLights, like torchs or light bulbs
    
    //if (pointLights[0]){
    //  for (unsigned int i = 0; i < NR_POINT_LIGHTS; i++){
    //      result += CalPointLight(pointLights[i],norm,FragPos,viewDir);
    //    }
    //}


    //Calc spotLight

    //if (spotLight.position){
    
    //    result += CalSpotLight(spotLight,norm,FragPos,viewDir);

    //}

    
    FragColor = vec4(result, 1.0);
    
}


vec3 CalDirLight(DirLight light, vec3 norm, vec3 viewDir){
    BasicPhongLighting phongLight = BasicPhongLighting(vec3(0.0),light.direction,
                                                        light.ambient,light.diffuse,
                                                        light.specular);

    BasicPhongLighting calPhongLight = CalBasicPhongLighting(phongLight,norm,vec3(0.0), viewDir);
    
    
    vec3 ambient = calPhongLight.ambient;
    vec3 diffuse = calPhongLight.diffuse;
    vec3 specular = calPhongLight.specular;
    
    return (ambient + diffuse + specular);
}

vec3 CalPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir){
    BasicPhongLighting phongLight = BasicPhongLighting(light.position,vec3(0.0),
                                                        light.ambient,light.diffuse,
                                                        light.specular);
    
    
    BasicPhongLighting calPhongLight = CalBasicPhongLighting(phongLight,norm,fragPos,viewDir);

    float attenuation = CalAttenuation(light.position, fragPos, light.constant, light.linear, light.quadratic);
    
    vec3 ambient = calPhongLight.ambient;
    vec3 diffuse = calPhongLight.diffuse;
    vec3 specular = calPhongLight.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

}

vec3 CalSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir){
    BasicPhongLighting phongLight = BasicPhongLighting(light.position, vec3(0.0),
                                                        light.ambient, light.diffuse,
                                                        light.specular);

    BasicPhongLighting calPhongLight = CalBasicPhongLighting(phongLight,norm,fragPos,viewDir);
    
    vec3 ambient = calPhongLight.ambient;
    vec3 diffuse = calPhongLight.diffuse;
    vec3 specular = calPhongLight.specular;

    float attenuation = CalAttenuation(light.position, fragPos, light.constant,
                                        light.linear, light.quadratic);

    vec3 lightDir = normalize(viewPos - fragPos);
    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

BasicPhongLighting CalBasicPhongLighting(BasicPhongLighting light, vec3 norm, vec3 fragPos , vec3 viewDir){
    vec3 lightDir = vec3(0.0);
    
    if (light.position != vec3(0.0)){
        lightDir = normalize(light.position - fragPos);
    }
    else {
        lightDir = normalize(-light.direction);
    }

    float diff = max(dot(norm,lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.texture_diffuse1,TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1,TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1,TexCoords).rgb;

    if (material.texture_diffuse2){
        ambient += light.ambient * texture(material.texture_diffuse2,TexCoords).rgb;
        diffuse += light.diffuse * diff * texture(material.texture_diffuse2,TexCoords).rgb;
    }
    if (material.texture_diffuse3){
        ambient += light.ambient * texture(material.texture_diffuse3,TexCoords).rgb;
        diffuse += light.diffuse * diff * texture(material.texture_diffuse3,TexCoords).rgb;
    }
    if (material.texture_specular2){
        specular += light.specular * spec * texture(material.texture_specular2,TexCoords).rgb;
    }

    return BasicPhongLighting(vec3(0.0),vec3(0.0),ambient,diffuse,specular);

}

float CalAttenuation(vec3 lightPosition, vec3 fragPos, float lightConstant, float lightLinear, float lightQuadratic){
    float distance = length(lightPosition - fragPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
       
    return attenuation;
}