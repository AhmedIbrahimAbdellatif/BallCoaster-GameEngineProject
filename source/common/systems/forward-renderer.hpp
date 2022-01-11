#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/light.hpp"

#include <glad/gl.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#define TRANSPOSE true

namespace our
{
    
    // The render command stores command that tells the renderer that it should draw
    // the given mesh at the given localToWorld matrix using the given material
    // The renderer will fill this struct using the mesh renderer components
    struct RenderCommand {
        glm::mat4 localToWorld;
        glm::vec3 center;
        Mesh* mesh;
        Material* material;
    };

    struct LightCommand {
        glm::mat4 localToWorld;
        LightComponent* light;
    };

    // A forward renderer is a renderer that draw the object final color directly to the framebuffer
    // In other words, the fragment shader in the material should output the color that we should see on the screen
    // This is different from more complex renderers that could draw intermediate data to a framebuffer before computing the final color
    // In this project, we only need to implement a forward renderer
    class ForwardRenderer {
        // These are two vectors in which we will store the opaque and the transparent commands.
        // We define them here (instead of being local to the "render" function) as an optimization to prevent reallocating them every frame
        std::vector<RenderCommand> opaqueCommands;
        std::vector<RenderCommand> transparentCommands;
        std::vector<LightCommand> lightCommands;
    public:
        // This function should be called every frame to draw the given world
        // Both viewportStart and viewportSize are using to define the area on the screen where we will draw the scene
        // viewportStart is the lower left corner of the viewport (in pixels)
        // viewportSize is the width & height of the viewport (in pixels). It is also used to compute the aspect ratio
        void render(World* world, glm::ivec2 viewportStart, glm::ivec2 viewportSize){
            // First of all, we search for a camera and for all the mesh renderers
            CameraComponent* camera = nullptr;
            opaqueCommands.clear();
            transparentCommands.clear();
            for(auto entity : world->getEntities()){
                // If we hadn't found a camera yet, we look for a camera in this entity
                if(!camera) camera = entity->getComponent<CameraComponent>();
                // If this entity has a mesh renderer component
                if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                    // We construct a command from it
                    RenderCommand command;
                    command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                    command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                    command.mesh = meshRenderer->mesh;
                    command.material = meshRenderer->material;
                    // if it is transparent, we add it to the transparent commands list
                    if(command.material->transparent){
                        transparentCommands.push_back(command);
                    } else {
                    // Otherwise, we add it to the opaque command list
                        opaqueCommands.push_back(command);
                    }
                }
                // If this entity has a light component
                if (auto light = entity->getComponent<LightComponent>(); light) {
                    LightCommand command;
                    command.light = light;
                    command.localToWorld = light->getOwner()->getLocalToWorldMatrix();
                    lightCommands.push_back(command);
                }
            }

            // If there is no camera, we return (we cannot render without a camera)
            if(camera == nullptr) return;

            glm::vec4 localFowardDirection(0.0, 0.0, -1.0, 0.0);
            glm::vec3 cameraForward = glm::vec3(camera->getOwner()->getLocalToWorldMatrix() * localFowardDirection);

            std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
                glm::vec4 firstCoord(glm::vec4(first.center, 1.0));
                glm::vec4 secondCoord(glm::vec4(second.center, 1.0));
                glm::vec4 firstSecondVec(secondCoord - firstCoord);

                return glm::dot(firstSecondVec, glm::vec4(cameraForward, 0)) < 0;
            });

            // get the camera ViewProjection matrix
            glm::mat4 VP = camera->getProjectionMatrix(viewportSize)* camera->getViewMatrix();

            // set the OpenGL viewport using viewportStart and viewportSize
            glViewport(viewportStart.x, viewportStart.y, viewportSize.x, viewportSize.y);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black
            glClearDepth(1.0f); // depth = 1

            // set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
            glColorMask(true, true, true, true);
            glDepthMask(true);

            // clear the color and depth buffers
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

            // opaque commands should be drawn before transparent ones (order is important)
            drawCommands(opaqueCommands, VP, camera, lightCommands);
            drawCommands(transparentCommands, VP, camera, lightCommands);
        }

        void drawCommands(std::vector<RenderCommand>& renderCommands, glm::mat4& VP, CameraComponent*& camera, std::vector<LightCommand>& lightCommands)
        {
            for (auto renderCommand : renderCommands)
            {
                renderCommand.material->setup();
                setVertexShaderUniforms(renderCommand, VP, camera);
                setFragmentShaderUniforms(lightCommands, renderCommand);
                renderCommand.mesh->draw();
            }
        }
        void setVertexShaderUniforms(our::RenderCommand& renderCommand, glm::mat4& VP, our::CameraComponent*& camera)
        {
            renderCommand.material->shader->set("object_to_world", renderCommand.localToWorld);
            renderCommand.material->shader->set("view_projection", VP);
            glm::vec4 eye(0.0, 0.0, 0.0, 1.0);
            glm::vec3 cameraPosition = glm::vec3(camera->getOwner()->getLocalToWorldMatrix() * eye);
            renderCommand.material->shader->set("camera_position", cameraPosition);
            renderCommand.material->shader->set("object_to_world_inv_transpose", glm::inverse(renderCommand.localToWorld), TRANSPOSE);
        }
        void setFragmentShaderUniforms(std::vector<our::LightCommand>& lightCommands, our::RenderCommand& renderCommand)
        {
            // we use single pass forward ligting, that's why we pass all lights in a single array to fragment shader
            int i = 0;
            for (auto lightCommand : lightCommands)
            {
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].type", static_cast<int>(lightCommand.light->lightType));
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].color", lightCommand.light->color);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].position", lightCommand.light->position);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].direction", glm::normalize(lightCommand.light->direction));
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].attenuation_constant", lightCommand.light->attenuation.x);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].attenuation_linear", lightCommand.light->attenuation.y);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].attenuation_quadratic", lightCommand.light->attenuation.z);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].inner_angle", lightCommand.light->coneAngles.x);
                renderCommand.material->shader->set("lights[" + std::to_string(i) + "].outer_angle", lightCommand.light->coneAngles.y);
                i++;
            }
            renderCommand.material->shader->set("light_count", i);
        }
        ;

    };

}