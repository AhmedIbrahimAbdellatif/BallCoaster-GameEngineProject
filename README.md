# Computer Graphics Project

## Introduction

> This is the project of the computer graphics academic course `CMPN205` in Cairo University - Faculty of Engineering - Credit Hours System - Communication and Computer program
>
> This project is about implementing a game engine, then using it to create a simple game of our design.		

***

## Learning Outcomes:

*The goals of this project are to learn how to:* 

1. Use buffers to store mesh data. 
2. Use vertex array objects to render meshes. 
3. Change an object's position, rotation and scale via matrices. 
4. Use depth testing, face culling, blending and color/depth masks. 
5. Use textures to draw images. 
6. Use samplers to configure the texture sampling process. 
7. Combine shaders, pipeline states and uniform data into a Material object. 
8. Describe and process a scene using the Entity-Component-System ECS framework. 
9. Implement a forward renderer with support for transparent objects. 
10. Implement a lighting model and support multiple lights of different types. 
11. Use the engine to implement a simple game.

> *The aim of this project is not to focus on game design or game visuals, the main aim is to absorb the previously mentioned concepts and be able to implement them in code to create a simple game built on a game engine of our implementation*

***

## Used Technologies

<img src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white">	<img src="https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl">

***

## Game States

### Menu State:

![](.\doc\menu-state.png)

*The game starts with a menu state, just still scene. When the user presses space bar the state changes from menu state to play state*

### Play State:

![](.\doc\game-state.png)

*Our game is very simple, the ball and the camera move forward in a linear movement, the player presses `D` to move right and `A` to move left and can increase the speed by pressing `Left-Shift`*

*For the player to win he has to dodge the obstacles until he reaches the finish line*

***

