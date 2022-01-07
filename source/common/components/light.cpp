#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "../asset-loader.hpp"

void our::LightComponent::deserialize(const nlohmann::json& data)
{
    if (!data.is_object()) return;
    shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());

    std::string lightTypeStr = data.value("lightType", "directional");
    if (lightTypeStr == "point") {
        lightType = LightType::POINT;
    }
    else if (lightTypeStr == "spot") {
        lightType = LightType::SPOT;
    }
    else {
        lightType = LightType::DIRECTIONAL;
    }
    
    position = data.value("position", position);
    direction = data.value("direction", direction);
    color = data.value("color", color);
    attenuation = data.value("attenuation", attenuation);
    coneAngles = glm::radians(data.value("coneAngles", coneAngles));
    ambient = data.value("ambient",ambient);
    diffuse = data.value("diffuse",diffuse);
    specular = data.value("specular",specular);
}

void our::LightComponent::setup()
{
    shader->use();
}
