# The Solar System
Using OpenGL, We made a 3D representation of the solar system including all the planets (with the addition of Pluto and the Moon for Earth). The GUI was made in Qt Creator.

## Running the Program
Make a build directory ("mkdir build") and cd into that directory ("cd build"). Type "cmake .." and once the compiling is done, type "make && ./solar".

## Concept Questions

* Describe how you used the matrix stack to model your scene

In the matrix stack, we had a for loop iterating through the size of m_info, which held our list of planets.  We first pushed, creating a copy of the top of the stack. From there, we ran rotateY for the rotational position of each planet around the sun, which changes following updateTime.  After that, there was a X translation applied to establish the distance from the sun, an X rotation to affix the texture to appear properly on the planet (North America at the front!), rotate Y to establish tilt, and then rotate Z to establish the day position.  Lastly, the radius of the planet was scaled in, the textures were pulled from the planet class, the sphere was drawn, and the matrix was popped off for the next planet.  

* Describe how you implemented pan and zoom

Pan and Zoom were both added in the keyPressEvent and utilized the m_view QMatrix4x4 methods.  For the scaling, a counter was added to keep track of the scale, and the scale was applied by m_view.scale(), with a factor of 2.0 for doubling and 0.5 for halving.  The Panning was implemented with the m_scaling in account to translate a specific distance across the viewing plane.  As such, it was implemented as m_view.translate() in the x or y direction by 1/20 of the viewing plane (20 uses will leave the original viewing frame).  

* How did you implement a moon around a planet?
    
The moon was added as another planet in solarData (could have also been added on separately in parsePlanets but we chose not to).  It was then implemented separately INSIDE the matrix stack after the FIRST X TRANSLATION of the planet around the sun.  As such, the moon implementation would orbit the sun.  The rest of the implementation is just copied from the planet implementations.  

* Describe how implemented right and pitch in your camera class. Do you rebuild the LookAt matrix from scratch or do you multiply the current LookAt matrix by a translation/rotation? If the latter, is the multiplication on the left or right.

To create the lookAt matrix, we worked from scratch.  We utilized the eye, at, and up vertices to form two separate matrices: a rotation matrix and a translation matrix.  We then multiplied them together with matrix multiplication and set lookAt = R * T, so the translation was performed first, while the rotation was performed second.  

* Describe the additional effect (besides adding the camera class and perspective matrix) you implemented.

We implemented a 'bouncing' forward and back vertical motion based off of a counter(increments with the timer) and a cosine sinusoid.  Due to the cosine, the forward motion would have a (smaller) component of vertical sinusoidal motion.  

* Given a 4x4 LookAt matrix could you recover any of the original parameters, eye, at, and up just by inspecting the matrix? If you can extract a parameter, describe which parameter you can extract and how. If you cannot recover a parameter, briefly describe why?  

Based on examination of the printmatrix, you can extract the -eye from the 4th column by pure Translation single-axis movements.  However, if a more complex transformation is performed, then the -eye becomes distorted by the rotation and then harder to see, but still gets close.  

## Acknowledgments
This program was for a computer science class taught by Andrew Danner at Swarthmore College. He provided starting code.
