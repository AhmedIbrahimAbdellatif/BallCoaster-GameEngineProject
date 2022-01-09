#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"

namespace our {
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());
        material = AssetLoader<Material>::get(data["material"].get<std::string>());
        player = data.value<bool>("player", false);
        obstacle = data.value<bool>("obstacle", false);
        radius = data.value<float>("radius", 1.0f);
        fixedPosition.x = data.value<float>("positionX", 1.0f);
        fixedPosition.y = data.value<float>("positionY", 1.0f);
        fixedPosition.z = data.value<float>("positionZ", 1.0f);
    }
}