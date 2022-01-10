#version 330 core
//#include "./light-common.glsl"

in Varyings {
   vec4 color;
   vec2 tex_coord;
   vec3 world;
   vec3 view;
   vec3 normal;
} fsin;

#define TYPE_DIRECTIONAL    0
#define TYPE_POINT          1
#define TYPE_SPOT           2

struct Light {
    int type;
    vec3 color;
    vec3 position, direction;
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
    float inner_angle, outer_angle;
};


struct TexturedMaterial {
   sampler2D albedo_map;
   vec3 albedo_tint;
   sampler2D specular_map;
   vec3 specular_tint;
   sampler2D ambient_occlusion_map;
   sampler2D roughness_map;
   vec2 roughness_range;
   sampler2D emissive_map;
   vec3 emissive_tint;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 emissive;
    float shininess;
};

#define MAX_LIGHT_COUNT 16
uniform TexturedMaterial material;
uniform int light_count = 1;
uniform Light lights[MAX_LIGHT_COUNT];

out vec4 frag_color;

Material sample_material(TexturedMaterial tex_mat, vec2 tex_coord){
   Material mat;
   mat.diffuse = tex_mat.albedo_tint * texture(tex_mat.albedo_map, tex_coord).rgb;
   mat.specular = tex_mat.specular_tint * texture(tex_mat.specular_map, tex_coord).rgb;
   mat.emissive = tex_mat.emissive_tint * texture(tex_mat.emissive_map, tex_coord).rgb;
   mat.ambient = mat.diffuse * texture(tex_mat.ambient_occlusion_map, tex_coord).r;
   float roughness = mix(tex_mat.roughness_range.x, tex_mat.roughness_range.y,
       texture(tex_mat.roughness_map, tex_coord).r);
   mat.shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
   return mat;
}

void main() {
    Material sampled = sample_material(material, fsin.tex_coord);
    vec3 normal = normalize(fsin.normal);
    vec3 view = normalize(fsin.view);

    // simulating light reflections
    vec3 ambient = sampled.ambient;
    
    // effect of light reflections and material emission
    vec3 accumulated_light = sampled.emissive + ambient;

    int count = min(light_count, MAX_LIGHT_COUNT);
    
    // Second step: frag color updated due to multiple light sources
    for(int i = 0; i < count; i++){
        Light light = lights[i];

        vec3 light_direction;
        float attenuation = 1;
        
        if(light.type == TYPE_DIRECTIONAL){
            light_direction = light.direction;
        }
        else {
             light_direction = fsin.world - light.position;
             float distance = length(light_direction);
             light_direction /= distance;
             attenuation *= 1.0f / (light.attenuation_constant +
                            light.attenuation_linear * distance +
                            light.attenuation_quadratic * distance * distance);
             if(light.type == TYPE_SPOT){
                float angle = acos(dot(light.direction, light_direction));
                attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
             }
        }
        
        float lambert = max(0.0f, dot(normal, -light_direction));
        vec3 diffuse = sampled.diffuse * light.color * lambert;

        vec3 reflected = reflect(light_direction, normal);
        float phong = pow(max(0.0f, dot(view, reflected)), sampled.shininess);
        vec3 specular = sampled.specular * light.color * phong;
        
        accumulated_light += (diffuse + specular) * attenuation;
    }

    frag_color = fsin.color * vec4(accumulated_light, texture(material.albedo_map, fsin.tex_coord).a);
}