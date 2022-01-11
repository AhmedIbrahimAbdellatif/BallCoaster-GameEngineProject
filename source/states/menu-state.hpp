#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-player-controller.hpp>
#include <systems/movement.hpp>
#include <systems/obstacle-collision.hpp>
#include <asset-loader.hpp>
#include <components/mesh-renderer.hpp>

// This state shows how to use the ECS framework and deserialization.
class MenuState: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::PlayerControllerSystem cameraController;
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
        if(config.contains("menu")){
            world.deserialize(config["menu"]);
        }
    }

    void onDraw(double deltaTime) override {
        // check if the user pressed space bar, start game
        if (getApp()->getKeyboard().isPressed(GLFW_KEY_SPACE)) {
            goToPlayState();
        }
        // And finally we use the renderer system to draw the scene
        auto size = getApp()->getFrameBufferSize();
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    void goToPlayState()
    {
        auto app_config = getApp()->getConfig();
        std::string gameState = app_config["game-scene"].get<std::string>();
        getApp()->changeState(gameState);
    }

    void onDestroy() override {
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};