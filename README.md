
# Concept Questions

* Describe how you used the matrix stack to model your scene
    In the matrix stack, we had a for loop iterating through the size of m_info, which held our list of planets.  We first pushed, creating a copy of the top of the stack. From there, we ran rotateY for the rotational position of each planet around the sun, which changes following updateTime.  After that, there was a X translation applied to establish the distance from the sun, an X rotation to affix the texture to appear properly on the planet (North America at the front!), rotate Y to establish tilt, and then rotate Z to establish the day position.  Lastly, the radius of the planet was scaled in, the textures were pulled from the planet class, the sphere was drawn, and the matrix was popped off for the next planet.  

* Describe how you implemented pan and zoom
Pan and Zoom were both added in the keyPressEvent and utilized the m_view QMatrix4x4 methods.  For the scaling, a counter was added to keep track of the scale, and the scale was applied by m_view.scale(), with a factor of 2.0 for doubling and 0.5 for halving.  The Panning was implemented with the m_scaling in account to translate a specific distance across the viewing plane.  As such, it was implemented as m_view.translate() in the x or y direction by 1/20 of the viewing plane (20 uses will leave the original viewing frame).  

* How did you implement a moon around a planet?
    The moon was added as another planet in solarData (could have also been added on separately in parsePlanets but we chose not to).  It was then implemented separately INSIDE the matrix stack after the FIRST X TRANSLATION of the planet around the sun.  As such, the moon implementation would orbit the sun.  The rest of the implementation is just copied from the planet implementations.  

# Quick Lab Survey

None of your answers to the questions below  will have an impact on your grade. This is to help provide feedback and improve course quality


* Approximately, how many hours did you take to complete this lab?
  *    11  Hours

* How difficult did you find this lab?
  (1-5, with 5 being very difficult and 1 being very easy)
    3.5

* Describe the biggest challenge you faced on this lab.
Understanding how to implement the UpdateTime method as well as how to correctly order the matrixStack (particularly regarding rotations).  
