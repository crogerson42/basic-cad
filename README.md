## Implementing a Basic CAD Framework
_Created by Christian Rogerson for CS5310 Computer Graphics with Professor Mike Shah_

<img src="https://github.com/user-attachments/assets/144b8007-d722-49c3-a890-22f5eaba6ea7" alt="BasiCAD" width="500"/>

### YouTube Demonstration: 

**https://youtu.be/TCiZyULI-5w**

### Description
This program implements a very simple CAD system in C++ using OpenGL.  It was created as a final project, using knowledge acquired over the course of a graduate-level class on 3D graphics.  Currently implemented features include:

* Movable camera
* Preliminary GUI buttons
* Keyboard controls (detailed below)

* Creating new cubic extrudes
* Lengthening/shortening extrusions
* Moving extrusions in six dimensions
* Loading custom-formatted geometry files
    ** _Geometry files contain descriptions of planes, sketches, and extrusions_
* Hiding/showing reference planes and wireframe mode

As a graphics application, it's better seen than described, so enjoy the short video demonstration linked above or feel free to run it yourself!

This project was built upon files created over the course of the semester (camera, etc.), including with permission several files created by Professor Mike Shah, which are cited as such in document headers.  Additionally, this project uses the GLAD and GLM libraries.

### Controls
* Space - camera/mouse mode
* WASD - move camera
* Tab - cycle objects
* ~ - cycle arrowkey edit mode
* ↑↓→← - edit geometry
* N - create new extrude
### UI Buttons
* Save file
* Load file
* Wireframe
* Toggle planes
* Create new extrude
### Compilation
Run Python script "build.py", i.e.:
*py build.py*
This will generate "project.exe" which can be executed with the arugments detailed below.
### Recommended Arguments:
./project ./geometry/robot.geo
* // loads a model of a simple anthropomorphic robot (see screenshot)

./project ./geometry/test.geo
* // loads a simple extrude of a concave polygon
