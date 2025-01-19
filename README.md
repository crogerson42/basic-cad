## Implementing a Basic CAD Framework
_Created by Christian Rogerson for CS5310 Computer Graphics with Professor Mike Shah_

<img src="https://github.com/user-attachments/assets/144b8007-d722-49c3-a890-22f5eaba6ea7" alt="BasiCAD" width="500"/>

### YouTube Demonstration: 

**https://youtu.be/TCiZyULI-5w**

### Description
This program implements a very simple CAD system in C++ using OpenGL.  It was created as a final project, using knowledge acquired over the course of a graduate-level class on 3D graphics.  Currently implemented features include:

* Interface:
  * Movable camera
  * Preliminary GUI buttons
  * Keyboard controls (detailed below)
  * Hiding/showing reference planes and wireframe mode
* Geometry:
  * Creating new cubic extrudes
  * Lengthening/shortening extrusions
  * Moving extrusions in six dimensions
  * Loading custom-formatted geometry files (which contain descriptions of planes, sketches, and extrusions_)

These features form a very basic core of CAD operations that "do something cool" in OpenGL.  A significant portion of this project was spent creating the datastructures and objects needed to represent geometry both mathematically and visually (planes, sketches, and extrusions), as well as capturing the hierarchical dependencies between them.  These representations are neither fully utilized nor demonstrated in the current implementation, but provide a flexible framework on which additional features can easily be built.  Additional features planned following the initial academic scope of this project include:

* (Now) Load and render geometry
* (Next) Create and modify planes, sketches, and extrusions with full user-input parameterization
* (Future) Complex operations (negative geometry, boolean operations, etc.)

As a graphics application, it's better seen than described, so enjoy the short video demonstration linked above or feel free to run it yourself!

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
### References:
This project was built upon files created over the course of the semester (camera, etc.), including with permission several files created by Professor Mike Shah, which are cited as such in document headers (Texture.hpp/.cpp, Image.hpp/.cpp, etc.).  This project also uses and includes the GLAD and GLM libraries.

Additionally, https://www.geometrictools.com/ was a very helpful resource on polygon tesselation algorithms.  A very basic approach is used in the current implementation of extrusions, but more advanced algorithms may be revisited in the future.
