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
        //TODO: Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        glm::vec4 eye(0.0, 0.0, 0.0, 1.0); // position is a point
        glm::vec4 center(0.0, 0.0 , -1.0, 1.0); // orientation // point
        glm::vec4 up(0.0, 1.0, 0.0, 0.0); // up is a vector

        glm::vec4 eye_new = glm::vec4(M * eye);
        glm::vec4 center_new = glm::vec4(M * center);
        glm::vec4 up_new = glm::vec4(M * up);

        glm::mat4 veiwMat = glm::lookAt(glm::vec3(eye_new.x, eye_new.y, eye_new.z), glm::vec3(center_new.x, center_new.y, center_new.z), 
                                        glm::vec3(up_new.x, up_new.y, up_new.z));

        return veiwMat;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective

        float aspectRatio = (float)viewportSize.x / viewportSize.y;
        if (cameraType == our::CameraType::PERSPECTIVE)
        {            
            return glm::perspective<float>(fovY, aspectRatio, near, far);
        }
        else if (cameraType == our::CameraType::ORTHOGRAPHIC)
        {
            return glm::ortho(-orthoHeight*aspectRatio/2, orthoHeight*aspectRatio/2, -orthoHeight / 2, orthoHeight / 2, near, far);
        }
    }
}