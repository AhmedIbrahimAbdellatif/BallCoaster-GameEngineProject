#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/mesh-renderer.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <systems/obstacle-collision.hpp>
#include <systems/movement.hpp>

#define FINISH_LINE -50

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class PlayerControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        int score = 0;
    
    private:
        bool stopGame = false;
        void stopMoving() {
            stopGame = true;
        }
        bool win = true;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        int getScore(){
            return score;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        bool update(World* world, float deltaTime, our::ObstacleCollisionSystem* obstacleCollisionSystem, 
                    our::MovementSystem* movementSystem) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent* camera = nullptr;
            MeshRendererComponent* mesh = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for(auto entity : world->getEntities()){
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(camera && controller) break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(camera && controller)) return false;

            for(auto entity : world->getEntities()){
                mesh = entity->getComponent<MeshRendererComponent>();
                if (mesh) {
                    mesh = mesh->isPlayer() ? mesh : nullptr;
                }
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(mesh && controller) break;
            }
            if(!(mesh && controller)) return false;

            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            //Entity* entity = camera->getOwner();
            Entity* entity = mesh->getOwner();

            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;
            glm::vec3& rotation = entity->localTransform.rotation;

            if (obstacleCollisionSystem->isCollision(mesh->radius, position)) {
                win = false;
                stopMoving();
                movementSystem->endGame();
            }

            if (position.z <= FINISH_LINE) {
                stopMoving();
                movementSystem->endGame();
            }

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;

            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;
            
            if(position.z < -9) { score = 1;}
            if(position.z < -18){ score = 2;}
            if(position.z < -27){ score = 3;}
            if(position.z < -36){ score = 4;}
            if(position.z < -45){ score = 5;}

            if(app->getKeyboard().isPressed(GLFW_KEY_D) && !stopGame) {
                glm::vec3 newPosition = position + right * (deltaTime * current_sensitivity.x);
                position = (newPosition.x <= 9)? newPosition : position;
            }
            if(app->getKeyboard().isPressed(GLFW_KEY_A) && !stopGame) {
                glm::vec3 newPosition = position - right * (deltaTime * current_sensitivity.x);
                position = (newPosition.x >= -9)? newPosition : position;
            }

            return stopGame;
        }

        bool isWin() {
            return win;
        }
    };

}
