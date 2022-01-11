#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>
#include "../shader/shader.hpp"

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
        glm::vec3 direction = { 0, -1, 0 };
        glm::vec3 color = { 255, 255, 255 };
        glm::vec3 attenuation = { 0, 0, 0 }; // for spot and point light types
        glm::vec3 ambient = { 0, 0, 0 };
        glm::vec3 diffuse = { 0, 0, 0 };
        glm::vec3 specular = { 0, 0, 0 };
        glm::vec2 coneAngles = { 0, 0 }; // for spot light

        static std::string getID() { return "Light"; }

        void deserialize(const nlohmann::json& data) override;
    };

}