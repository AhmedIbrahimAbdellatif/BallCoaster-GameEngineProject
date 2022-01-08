#include "material.hpp"

#define TEXTURE_UNIT_0 0
#define TEXTURE_UNIT_1 1
#define TEXTURE_UNIT_2 2
#define TEXTURE_UNIT_3 3
#define TEXTURE_UNIT_4 4
#define TEXTURE_UNIT_5 5

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: Write this function
        Material::setup(); // tinted material is the child of material class
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: Write this function
        TintedMaterial::setup(); // textured material is the child of tinted material class
        shader->set("alphaThreshold", alphaThreshold);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        sampler->bind(TEXTURE_UNIT_0);
        shader->set("tex", TEXTURE_UNIT_0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup() const
    {
        Material::setup(); // parent's setup()

        shader->set("material.albedo_tint", albedo_tint);
        shader->set("material.specular_tint", specular_tint);
        shader->set("material.roughness_range", roughness_range);
        shader->set("material.emissive_tint", emissive_tint);

        shader->set("sky_light.top_color", {0.5, 0.5, 1});
        shader->set("sky_light.middle_color", { 1, 1, 1});
        shader->set("sky_light.bottom_color", { 0.5, 1, 0.5 });

        glActiveTexture(GL_TEXTURE1);
        albedo_map->bind();
        sampler->bind(TEXTURE_UNIT_1);
        shader->set("material.albedo_map", TEXTURE_UNIT_1);
        
        glActiveTexture(GL_TEXTURE2);
        specular_map->bind();
        sampler->bind(TEXTURE_UNIT_2);
        shader->set("material.specular_map", TEXTURE_UNIT_2);
        
        glActiveTexture(GL_TEXTURE3);
        ambient_occlusion_map->bind();
        sampler->bind(TEXTURE_UNIT_3);
        shader->set("material.ambient_occlusion_map", TEXTURE_UNIT_3);
        
        glActiveTexture(GL_TEXTURE4);
        roughness_map->bind();
        sampler->bind(TEXTURE_UNIT_4);
        shader->set("material.roughness_map", TEXTURE_UNIT_4);
        
        glActiveTexture(GL_TEXTURE5);
        emissive_map->bind();
        sampler->bind(TEXTURE_UNIT_5);
        shader->set("material.emissive_map", TEXTURE_UNIT_5);
    }

    void LitMaterial::deserialize(const nlohmann::json& data) {
        Material::deserialize(data);
        if (!data.is_object()) return;
        
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

        std::string test = data.value("albedo_map", "white");
        albedo_map = AssetLoader<Texture2D>::getTexture(data.value("albedo_map", "white"), glm::vec4(255, 255, 255, 255));
        albedo_tint = data.value<glm::vec3>("albedo_tint", { 1.0f, 1.0f, 1.0f });
        
        specular_map = AssetLoader<Texture2D>::getTexture(data.value<std::string>("specular_map", "black"), glm::vec4(0, 0, 0, 255));
        specular_tint = data.value<glm::vec3>("specular_tint", { 1.0f, 1.0f, 1.0f });

        roughness_map = AssetLoader<Texture2D>::getTexture(data.value<std::string>("roughness_map", "white"), glm::vec4(255, 255, 255, 255));
        roughness_range = data.value<glm::vec2>("roughness_scale", { 0.0f, 1.0f });

        ambient_occlusion_map = AssetLoader<Texture2D>::getTexture(data.value<std::string>("ambient_occlusion_map", "white"), glm::vec4(255, 255, 255, 255));

        emissive_map = AssetLoader<Texture2D>::getTexture(data.value<std::string>("emissive_map", "black"), glm::vec4(0, 0, 0, 255));
        emissive_tint = data.value<glm::vec3>("emissive_tint", { 1.0f, 0.0f, 1.0f });
    }
}