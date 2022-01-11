#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-player-controller.hpp"
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
    class PlayerControllerSystem {
        Application* app; // The application in which the state runs
        int score = 0;
    
    private:
        bool stopGame = false;
        void stopMoving() {
            stopGame = true;
        }
        bool win = true;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app) {
            this->app = app;
        }

        int getScore() {
            return score;
        }

        // This should be called every frame to update the player entity 
        bool update(World* world, float deltaTime, our::ObstacleCollisionSystem* obstacleCollisionSystem, 
                    our::MovementSystem* movementSystem) {

            MeshRendererComponent* playerEntity = nullptr;
            FreePlayerControllerComponent *playerController = nullptr;
            findPlayerAndController(world, playerEntity, playerController);
            if(!(playerEntity && playerController)) return false;

            Entity* playerOwnerEntity = playerEntity->getOwner();
            glm::vec3& playerPosition = playerOwnerEntity->localTransform.position;

            if (isEndGame(obstacleCollisionSystem, playerEntity->radius, playerPosition))
            {
                if (obstacleCollisionSystem->isCollision(playerEntity->radius, playerPosition)) {
                    win = false;
                }
                stopMoving();
                movementSystem->endGame();
            }
            else {
                updateScore(playerPosition);
                
                glm::vec3 right = getRightDirection(playerOwnerEntity);
                updatePosition(playerController, playerPosition, right, deltaTime);
            }           

            return stopGame;
        }

        glm::vec3 getRightDirection(our::Entity*& playerOwnerEntity)
        {
            glm::mat4 matrix = playerOwnerEntity->localTransform.toMat4(); // player model matrix relative to parent
            return glm::vec3(matrix * glm::vec4(1, 0, 0, 0)); // compute right relative to parent
        }

        void updatePosition(our::FreePlayerControllerComponent* playerController, glm::vec3& position, glm::vec3& right, float deltaTime)
        {
            glm::vec3 current_sensitivity = playerController->positionSensitivity;
            // increase speed
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) {
                current_sensitivity *= playerController->speedupFactor;
            }

            // move right
            if (app->getKeyboard().isPressed(GLFW_KEY_D) && !stopGame) {
                glm::vec3 newPosition = position + right * (deltaTime * current_sensitivity.x);
                position = (newPosition.x <= 9) ? newPosition : position;
            }

            // move left
            if (app->getKeyboard().isPressed(GLFW_KEY_A) && !stopGame) {
                glm::vec3 newPosition = position - right * (deltaTime * current_sensitivity.x);
                position = (newPosition.x >= -9) ? newPosition : position;
            }
        }

        void updateScore(glm::vec3& position)
        {
            // update score according to distance
            if (position.z < -9)  { score = 1; }
            if (position.z < -18) { score = 2; }
            if (position.z < -27) { score = 3; }
            if (position.z < -36) { score = 4; }
            if (position.z < -45) { score = 5; }
        }

        void findPlayerAndController(World* world, MeshRendererComponent* &playerToSet, FreePlayerControllerComponent* &controllerToSet) {
            for (auto entity : world->getEntities()) {
                playerToSet = entity->getComponent<MeshRendererComponent>();
                if (playerToSet) {
                    playerToSet = playerToSet->isPlayer() ? playerToSet : nullptr;
                }
                controllerToSet = entity->getComponent<FreePlayerControllerComponent>();
                if (playerToSet && controllerToSet) break; // we have to find both to break
            }
        }

        bool isEndGame(our::ObstacleCollisionSystem* &obstacleCollisionSystem, float playerRadius, glm::vec3 playerPosition) {
            return obstacleCollisionSystem->isCollision(playerRadius, playerPosition) || playerPosition.z <= FINISH_LINE;
        }

        bool isWin() {
            return win;
        }
    };

}
