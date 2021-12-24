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

        glm::mat4 scaleMat = glm::mat4(1.0f);
        scaleMat[0][0] = scale[0];
        scaleMat[1][1] = scale[1];
        scaleMat[2][2] = scale[2];
        std::cout << "scale:" << std::endl;
        printMat4(scaleMat);
        std::cout << std::endl;

        glm::mat4 rotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z); 
        std::cout << "rotation:" << std::endl;
        printMat4(rotationMat);
        std::cout << std::endl;

        glm::mat4 translationMat = glm::mat4(1.0f);
        translationMat[0][3] = position[0];
        translationMat[1][3] = position[1];
        translationMat[2][3] = position[2];
        std::cout << "translation:" << std::endl;
        printMat4(translationMat);
        std::cout << std::endl;


        glm::mat4 transformationMat = translationMat * rotationMat * scaleMat;
        std::cout << "total:" << std::endl;
        printMat4(transformationMat);
        std::cout << std::endl;

        return  transformationMat;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}