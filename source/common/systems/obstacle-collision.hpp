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

        void addObstacle(float radius, glm::vec3 position) {
            obstacleRadius.push_back(radius);
            obstaclePosition.push_back(position);
        }

        bool isCollision(float playerRadius, glm::vec3 playerPosition) {
            for (int i = 0; i < OBSTACLES_COUNT; i++) {
                float dist = glm::distance(playerPosition, obstaclePosition[i]);
                if (dist <= playerRadius + obstacleRadius[i]) {
                    return true;
                }
            }
            return false;
        }
    };

}