#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

    // An enum that defines the type of the light 
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component {
    public:
        LightType lightType;
        glm::vec3 position = { 0, 0, 0 };
        glm::vec3 direction = { 0, 0, 0 };
        glm::vec3 color = { 0, 0, 0 };
        glm::vec3 attenuation = { 0, 0, 0 };
        glm::vec3 ambient = { 0, 0, 0 };
        glm::vec3 diffuse = { 0, 0, 0 };
        glm::vec3 specular = { 0, 0, 0 };
        glm::vec2 coneAngles = { 0, 0 };

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        void deserialize(const nlohmann::json& data) override;
    };

}