# CS480_FinalProject
Explorative Program of the Solar System

Created a Explorative Program of the Solar System with accurate depictions of the planets, such as axis tilt, rotation, and moons. Coded with object oritentation in mind, using OpenGL libraries. 

![alt text](https://github.com/jmontesa1/CS480_FinalProject/blob/main/CS480UML.png?raw=true)


EXPLORATION MODE CONTROLS
'W' - Move forward
'A' - Move left
'S' - Move right
'D' - Move backward
'Left Shift' - (While moving) Speeds up the ship, Boost
'Left Control - (While moving) Limits speed of ship to a slow pace
'Spacebar' - Completely stop ship
'Mouse' - Change direciton of the ship, rotate ship
'Scroll'- Scroll up, zooms in. Scroll down, zooms out


OBSERVATION MODE CONTROLS
'M' - Press 'M' to enter observation mode
'Mouse' - Rotate camera, look at
'Scroll' Scroll up, zooms in. Scroll down, zooms out

WHILE IN OBSERVATION MODE:
'1' - Mercury
'2' - Venus
'3' - Earth
'4' - Mars
'5' - Jupiter
'6' - Saturn
'7' - Uranus
'8' - Neptune



DEPENDENCIES
External Libraries: 

Open Asset Import Library (ASSIMP)  is one of the key external libraries that we used for our project. From our previous assignments, we used ASSIMP to effectively load in external models or meshes as .obj files. These .obj files will turn into the object model that we see inside of our program. The .obj files that we are importing from will be 3-D models. We used ASSIMP to load meshes such as the starship, saturn rings, comets, and asteroids.  We ended up using <assimp/Importer.hpp>, <assimp/scene.h>, and <assimp/postprocess.h> to load in our object files.

OpenGL Mathematics (GLM) is one of the most important external libraries that we used for our project. Another library from our previous assignments, it allowed us to implement OPENGL math to our system, which enabled us to alter how the solar system behaves. We used GLM:: operations to create transformations that we sent out to our model matrix. We used GLM::translate to create our elliptical orbits in our solar system. We used GLM::rotate to create realistic rotations of planets and sun in our solar system. Finally, we used GLM::scale to edit the size of our planets and solar objects so that they would be scaled to what they would appear like in real life. GLM allowed us to create a realistic simulation of the solar system, which we were able to edit and send into our model matrix. We used <glm/glm.hpp>, <glm/gtc/matrix_transform.hpp>, <glm/gtc/type_ptr.hpp>, and <glm/gtx/rotate_vector.hpp> to manipulate our transformations. We also used <glm/gtc/random.hpp> to use the linear rand command which allowed us to randomly render in our asteroids.

OpenGL Extension Wrangler Library (GLEW) is a library that we have used since the beginning of our projects. We utilized GLEW to load external libraries into our project. It allowed us to see which OpenGL extensions are supported on our platform. For glew we used <GL/glew.h>. It is a basic GL library.

Simple OpenGL Image Library (SOIL) was used often in our project, dating back to previous projects as well. This library allowed us to import images from online that were either jpg or png files. These images were later converted to either diffuse textures or normal textures in our objects. We used SOIL_load_OGL_texture to load in our texture based on the file name we gave it, then give it to the corresponding texture type of either diffuse or normal.

GLFW was a library that we used to edit our inputs, our keyboard and mouse movements. We utilized GLFWto get a keyboard input so that we could close our window, move our starship around front, forward, back, up, down, left, and right. We also utilized GLFW so that we could obtain our cursor location and move our screen based on the movement of our mouse, which we would update constantly. We used <GLFW/glfw2.h>.
