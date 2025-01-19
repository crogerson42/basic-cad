## Implementing a Basic CAD Framework
Created by Christian Rogerson for CS5310 Computer Graphics with Professor Mike Shah

<img src="https://github.com/user-attachments/assets/144b8007-d722-49c3-a890-22f5eaba6ea7" alt="BasiCAD" width="500"/>

### YouTube Demonstration: 

**https://youtu.be/TCiZyULI-5w**

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
