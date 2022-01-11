#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
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
            for(auto entity : world.getEntities()){
                our::MeshRendererComponent* mesh = entity->getComponent<our::MeshRendererComponent>();
            }
        }
        // We initialize the camera controller system since it needs a pointer to the app
        /*cameraController.enter(getApp());*/
    }

    void onDraw(double deltaTime) override {
        // And finally we use the renderer system to draw the scene
        if (getApp()->getKeyboard().isPressed(GLFW_KEY_SPACE)) {
            auto app_config = getApp()->getConfig();
            std::string gameState = app_config["game-scene"].get<std::string>();
            getApp()->changeState(gameState);
        }
        auto size = getApp()->getFrameBufferSize();
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    void onDestroy() override {
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};