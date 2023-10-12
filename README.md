# PhotoBall

<a target="blank"><img align="center" src="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/PicklePhotoBall.png?raw=true" height="300" /></a>
<a href="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/Pickle.gif?raw=true" target="blank"><img align="center" src="https://github.com/MMusabAyhan/PhotoBall/blob/main/screenshots/Pickle.gif?raw=true" height="300" /></a>

PhotoBall is an application that forms an image out of thousands of balls. User can select the image s/he prefers and choose the simulation style via the
interface. After selections are made, user can click the Start Simulation button and ta-da!

PhotoBall uses OpenGL for renderer and a simple 2D physics engine made by John Buffer for physics simulation (A physic engine based on verlet integration). For UI, it utilizes ImGui ,and for building, the app uses premake.
Some other libraries are also used for other features such as loading an image into a buffer, etc.

## How To Build

1- Clone the repository recursively into your local directory. ( git clone --recursive https://github.com/MMusabAyhan/PhotoBall.git )

2- Execute the batch file named "Win-Premake" inside the "scripts" folder. It should create all necessary libs and project files for the solution.

3- You are all set. Now, you can open the solution file and check out the PhotoBall.

## Controls

In case of an image formed in the wrong orientation, a simple control is added for the camera. These controls only valid when the simulation is in progress.

Q - Turn 90 degree Left

E - Turn 90 degree Right

WSAD - To move the camera around

Mouse Wheel - Zoom In/Out

Space - Reset View

## Credits

This project was inspired by the youtuber named Pezza`s work (John Buffer) and The Cherno. Many thanks to both of them.

