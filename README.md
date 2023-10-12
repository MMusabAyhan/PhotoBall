# PhotoBall

## General Description

<a target="blank"><img align="center" src="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/PicklePhotoBall.png?raw=true" height="300" /></a>
<a href="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/Pickle.gif?raw=true" target="blank"><img align="center" src="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/Pickle.gif?raw=true" height="300" /></a>

PhotoBall is an application that forms an image out of thousands of balls. User can select the image s/he prefers and choose the simulation style via the
interface. After selections are made, user can click the Start Simulation button and ta-da!

PhotoBall uses OpenGL for renderer and a simple 2D physics engine made by John Buffer for physics simulation (A physic engine based on verlet integration). For UI, it utilizes ImGui ,and for building, it uses premake.
Some other libraries are also used for other features such as Math(glm) and loading an image into a buffer, etc.

Unfortunately, PhotoBall is only supported in Windows.

## How To Build

1- Clone the repository recursively into your local directory. ( git clone --recursive https://github.com/MMusabAyhan/PhotoBall.git )

2- Execute the batch file named "Win-Premake" inside the "scripts" folder. It should create all necessary libs, project files and the solution for the application.

3- Now, you are all set. You can open the solution file and check out the PhotoBall.

## Controls

In case of an image formed in the wrong orientation, a simple control is added for the camera. These controls only valid when the simulation is in progress.

Q - Rotates camera 90° Clockwise

E - Rotates camera 90° Anti-Clockwise

WSAD - Moves camera around

Mouse Wheel - Zooms In/Out

Space - Resets View

## Credits

This project was inspired by the youtuber named Pezza`s work (John Buffer) and The Cherno. Many thanks to both of them.

