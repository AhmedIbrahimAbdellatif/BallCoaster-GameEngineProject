#pragma once
#include <stdlib.h>
#include <vector>
#include <glm/glm.hpp>

#define OBSTACLES_COUNT 9
namespace our
{

    class ObstacleCollisionSystem {
        

    public:
        std::vector<float> obstacleRadius;
        std::vector<glm::vec3> obstaclePosition;

        // store obstacle position and dimensions to be able to detect collision with player
        void addObstacle(float radius, glm::vec3 position) {
            obstacleRadius.push_back(radius);
            obstaclePosition.push_back(position);
        }

        bool isCollision(float playerRadius, glm::vec3 playerPosition) {
            // loop over all obstacles to detect collision with any of them
            for (int i = 0; i < OBSTACLES_COUNT; i++) {
                // we use the sphere collision as in our game design both the player and the obstacles are spheres
                // sphere collision is detected if the summation of the radii <= distance between both centers
                float dist = glm::distance(playerPosition, obstaclePosition[i]);
                if (dist <= playerRadius + obstacleRadius[i]) {
                    return true; // return true if the player collided with any of the obstacles
                }
            }
            return false;
        }
    };

}