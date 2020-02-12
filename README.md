# Lozenge Larry's Road to Destiny
This is my group university project, which is a platformer game developed with OpenGL, SDL, Bullet and NGL
This application is using some of the graphic libraries created by our lecturer at Bournemouth University (NCCA)

I was mainly responsible for wrapping up the Bullet Physics Engine, collisions/collision shapes, controls, debugging, import/export geometry, implementation of game features.
                                                                                                                  
![](https://github.com/Elyg/OpenGL_Flocking/blob/master/flocking_demo.gif)

# Library dependancies

 - NGL
 - OpenGL
 - Bullet
 - SDL2
 - Boost
 - tiff
 - Qt5
 - qmake
 
### How to compile and run the application
```
git clone git@github.com:Elyg/Larry_Game.git
cd Larry_Game/
qmake FlockingSystem.pro
make
./Larry_Game
```
# Playing the game

The controls for the game are as follows:

* Movement: W - Forward, S - Backward, A - Left, D - Right, SPACE - Jump
* Camera: Press and drag with the left mouse button to move the camera, and use the scroll wheel to zoom in and out.
* Shapeshifting: F will make you sprout legs! Growing legs will stop your momentum and enable you to jump.
* Press ENTER to bring up the pause menu at any time.
* DEVELOPERS: Press CTRL+SHIFT+B to enter debug mode
* Pressing ESC will quit the game.

# Some References:
[https://pybullet.org/wordpress/](https://pybullet.org/wordpress/)

[https://www.khronos.org/registry/OpenGL-Refpages/](https://www.khronos.org/registry/OpenGL-Refpages/)

[https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)

[https://github.com/NCCA/NGL](https://github.com/NCCA/NGL)
```
You can find the doxygen generated documentation for this project in the /docs folder, 
just open the /docs/index.html with your preffered browser
```
