#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include<iostream>
#include <glm/gtx/euler_angles.hpp>

namespace our {

    void printMat4(glm::mat4 mat)
    {
        for (int i = 0;i < 4;++i)
        {
            for (int j = 0;j < 4;++j)
            {
                std::cout << mat[i][j] << " ";
            }
            std::cout  << "\n";
        }
    }
    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: Write this function
        glm::mat4 transformationMat = glm::translate(glm::mat4(1.0f), position) *
            glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) *
            glm::scale(glm::mat4(1.0f), scale);

        return  transformationMat;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}