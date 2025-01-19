## Implementing a Basic CAD Framework

## YouTube/Dropbox/Drive Link: 

**https://youtu.be/TCiZyULI-5w**

## Screenshots
<img src="https://github.com/user-attachments/assets/144b8007-d722-49c3-a890-22f5eaba6ea7" alt="BasiCAD" width="500"/>

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
### Recommended Arguments:
./project ./geometry/robot.geo
* // loads a model of a simple anthropomorphic robot (see screenshot)

./project ./geometry/test.geo
* // loads a simple extrude of a concave polygon

## Details
* Name:
  * Christian Rogerson
* How many hours did it take you to complete this final project?
  * ~60 hours
* Did you collaborate or share ideas with any other students/TAs/Professors?
  * N/A
* Did you use any external resources? 
  * Note it is fair to utilize some tutorials--cite them here. You **must** add your own personal touch however!
  * https://www.geometrictools.com/ - helpful resource on polygon tesselation algorithms
  * Assorted files from the course, particularly the structure of the model loader as well as Texture.hpp/cpp and Image.hpp/.cpp, were very helpful with structuring the program
* (Optional) What was the most interesting part of the Final Assignment? How could the instructor improve the final project?
  * Probably 55 hours was spent setting up the datastructures used in this project and resolving OpenGL issues, and 5 hours were spent implementing the UI interactivity features.  This was not a result of lack of attention to the UI, but rather a referrendum on how easily implementing user functions was once the fundamental datastructures allowing those functions (editable geometry, etc.) were set up.  A significant challenge I faced was caused by C++ creating object copies, which then cleared OpenGL buffers upon their destruction while the original objects still referenced them.  This was solved by clearing buffers not in the destructor, but in a FreeBuffers() function called when the mainloop terminates.  This worked fine because the only objects that have associated GPU data are stored in global lists, which are traversed as part of the PreDraw() and Draw() calls, so the main/global scope have no difficulty keeping track of them.

### Rubric

**Make sure you read the banned project ideas/APIs in the Readme**

<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(5%) Project Check-in</td>
     <td align="left">Did you check-in with the professor or TA during office hours regarding your project at least 2 weeks before the deadline?</td>
    </tr>
  </tbody>
</table>


<table>
  <tbody>
    <tr>
      <th>Points</th>
      <th align="center">Description</th>
    </tr>
    <tr>
      <td>(25%) Project Completion</td>
     <td align="left"><ul><li>(10%) Does the project compile and run.</li><li>Is it polished without any bugs (No weird visual artifacts).</li><li>(15%)Did you make a video (ideally 2-3 minutes in length following guidelines given in writeup: introduce your project, show your project, show something technical in code that you can explain.)?</li><li>(5%)Did you add a screenshot of your project to the repository?</li></ul></td>
    </tr>
    <tr>
      <td>(40%) Technical</td>
      <td align="left"><ul><li>Was the implementation of the project challenging?</li><li>Even if you followed a tutoral, it should not be trivial, and have some personal touch to it.</li><li>Did you have to organize/process a sufficient amount of data?</li><li>Was it clear you consulted some outside resources that go above and beyond the scope of this class</li></ul></td>
    </tr>
    <tr>
      <td>(30%) Creativity</td>
      <td align="left"><ul><li>How visually appealing is the scene?<ul><li>Note: There should be some 'wow' factor--instructors discretion is used here relative to previous projects.</li></ul></li><li>How original is the project<ul><li>Again, did you enhance a tutorial and do something unique or just go by the book?</li></ul></li></ul></td>
    </tr>
  </tbody>
</table>
