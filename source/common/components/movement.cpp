#include "movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    // in our game we move both the camera and the ball (i.e. player) forward in a straight line with the same speed
    // we also move the ball with some angular velocity to simulate ball rotation
    void MovementComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // we have two types of continuous movements either in a straight line or rotating in place
        linearVelocity = data.value("linearVelocity", linearVelocity);
        angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));
    }
}