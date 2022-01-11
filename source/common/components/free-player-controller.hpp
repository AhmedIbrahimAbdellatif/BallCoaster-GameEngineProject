#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    // This component denotes that the FreePlayerControllerSystem will move the owning entity using user inputs.
    class FreePlayerControllerComponent : public Component {
    public:
        
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; // The unit per second of player movement if WASD is pressed
        float speedupFactor = 5.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.

        static std::string getID() { return "Free Player Controller"; }

        // Reads sensitivity & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}