#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos; 
in vec3 LightPos;
in vec2 TextCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emision;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse,TextCoords).rgb;
  	
    //diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse,TextCoords).rgb;
    
    //specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 specular = light.specular * spec * texture(material.specular,TextCoords).rgb;  
        
    //emision 
    vec3 emision = vec3(0.0);
     
    if (texture(material.specular,TextCoords).rgb == vec3(0.0)){
        emision = texture(material.emision,TextCoords).rgb;
    }

    vec3 result = ambient + diffuse + specular + emision;
    FragColor = vec4(result, 1.0);
}