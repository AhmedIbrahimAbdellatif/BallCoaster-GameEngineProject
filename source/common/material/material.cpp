#include "material.hpp"

#define FIRST_TEXTURE_UNIT 0

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
        texture->bind();
        sampler->bind(FIRST_TEXTURE_UNIT);
        shader->set("tex", FIRST_TEXTURE_UNIT);
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
        TexturedMaterial::setup(); // parent's setup()

        // setting samplers in texturedMaterial uniform
        albedoMap->bind(1);
        shader->set("texturedMaterial.albedo_map", 1);
        specularMap->bind(2);
        shader->set("texturedMaterial.specular_map", 2);
        ambientOcclusionMap->bind(3);
        shader->set("texturedMaterial.ambient_occlusion_map", 3);
        roughnessMap->bind(4);
        shader->set("texturedMaterial.roughness_map", 4);
        emissiveMap->bind(5);
        shader->set("texturedMaterial.emissive_map", 5);

        // setting vectors in texturedMaterial uniform
        shader->set("texturedMaterial.albedo_tint", albedo);
        shader->set("texturedMaterial.specular_tint", specular);
        shader->set("texturedMaterial.roughness_range", roughnessRange);
        shader->set("texturedMaterial.emissive_tint", emissive);
    }

    void LitMaterial::deserialize(const nlohmann::json& data) {
        TintedMaterial::deserialize(data);
        if (!data.is_object()) return;
        
        // loading samplers
        albedoMap = AssetLoader<Sampler>::get(data.value("albedoMap", ""));
        specularMap = AssetLoader<Sampler>::get(data.value("specularMap", ""));
        ambientOcclusionMap = AssetLoader<Sampler>::get(data.value("ambientOcclusionMap", ""));
        roughnessMap = AssetLoader<Sampler>::get(data.value("roughnessMap", ""));
        emissiveMap = AssetLoader<Sampler>::get(data.value("emissiveMap", ""));

        // loading vectors
        albedo = data.value("albedo", albedo);
        specular = data.value("specular", specular);
        roughnessRange = data.value("roughness", roughnessRange);
        emissive = data.value("emissive", emissive);
    }
}