#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>

namespace our {
    void printMat4_2(glm::mat4 mat)
    {
        for (int i = 0;i < 4;++i)
        {
            for (int j = 0;j < 4;++j)
            {
                std::cout << mat[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();

        glm::vec4 eye_cam(0.0, 0.0, 0.0, 1.0); // position is a point
        glm::vec4 center_cam(0.0, 0.0 , -1.0, 1.0); // orientation // point
        glm::vec4 up_cam(0.0, 1.0, 0.0, 0.0); // up is a vector

        glm::vec3 eye_world = glm::vec3(M * eye_cam);
        glm::vec3 center_world = glm::vec3(M * center_cam);
        glm::vec3 up_world = glm::vec3(M * up_cam);

        glm::mat4 viewMat = glm::lookAt(eye_world, center_world, up_world);

        return viewMat;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
       
        float aspectRatio = (float)viewportSize.x / viewportSize.y;
        if (cameraType == our::CameraType::PERSPECTIVE)
        {            
            return glm::perspective<float>(fovY, aspectRatio, near, far);
        }
        else if (cameraType == our::CameraType::ORTHOGRAPHIC)
        {
            // left, right, bottom, top, near, far
            return glm::ortho(-orthoHeight*aspectRatio/2, orthoHeight*aspectRatio/2, -orthoHeight / 2, orthoHeight / 2, near, far);
        }
    }
}