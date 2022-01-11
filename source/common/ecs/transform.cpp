#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include<iostream>
#include <glm/gtx/euler_angles.hpp>

namespace our {
    // This function computes and returns a matrix that represents this transform
    glm::mat4 Transform::toMat4() const {
        // prepare transformation matrices for translation, rotation, scaling
        glm::highp_mat4 translation(glm::translate(glm::mat4(1.0f), position));
        glm::highp_mat4 rotation(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z));
        glm::highp_mat4 scaling(glm::scale(glm::mat4(1.0f), scale));

        // combine these transformations through matrix multiplication in the proper order
        return  translation * rotation * scaling;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}