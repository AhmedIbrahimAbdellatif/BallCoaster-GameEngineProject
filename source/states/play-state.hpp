#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-player-controller.hpp>
#include <systems/movement.hpp>
#include <systems/obstacle-collision.hpp>
#include <asset-loader.hpp>
#include <components/mesh-renderer.hpp>
#include <stdlib.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::PlayerControllerSystem playerController;
    our::MovementSystem movementSystem;
    our::ObstacleCollisionSystem obstacleCollisionSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
            for(auto entity : world.getEntities()){
                our::MeshRendererComponent* mesh = entity->getComponent<our::MeshRendererComponent>();
                if(mesh != nullptr && mesh->isObstacle()) {
                    float obstacleRadius = mesh->radius;
                    glm::vec3 obstaclePosition = mesh->fixedPosition;
                    obstacleCollisionSystem.addObstacle(obstacleRadius, obstaclePosition);
                }
            }
        }
        // We initialize the camera controller system since it needs a pointer to the app
        playerController.enter(getApp());
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        bool stopPlaying = playerController.update(&world, (float)deltaTime, &obstacleCollisionSystem, &movementSystem);
        auto& config = getApp()->getConfig()["scene"];
        if (stopPlaying) {
            if (playerController.isWin()) {
                // announce winning
                if(config.contains("win")){
                    world.deserialize(config["win"]);
                }
            }
            else {
                // announce losing
                if(config.contains("lose")){
                    world.deserialize(config["lose"]);
                }
            }
        }
        int score = playerController.getScore();
        if(score == 0){
            if(config.contains("zero")){ world.deserialize(config["zero"]);}
        }
        else if(score == 1){
            if(config.contains("one")){ world.deserialize(config["one"]);}
        }
        else if(score == 2){
            if(config.contains("two")){ world.deserialize(config["two"]);}
        }
        else if(score == 3){
            if(config.contains("three")){ world.deserialize(config["three"]);}
        }
        else if(score == 4){
            if(config.contains("four")){ world.deserialize(config["four"]);}
        }
        else if(score == 5){
            if(config.contains("five")){ world.deserialize(config["five"]);}
        }
        // And finally we use the renderer system to draw the scene
        auto size = getApp()->getFrameBufferSize();
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    void onDestroy() override {
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};