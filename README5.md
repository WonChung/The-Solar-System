
# Concept Questions

* Describe how implemented right and pitch in your camera class. Do you rebuild the LookAt matrix from scratch or do you multiply the current LookAt matrix by a translation/rotation? If the latter, is the multiplication on the left or right.
To create the lookAt matrix, we worked from scratch.  We utilized the eye, at, and up vertices to form two separate matrices: a rotation matrix and a translation matrix.  We then multiplied them together with matrix multiplication and set lookAt = R * T, so the translation was performed first, while the rotation was performed second.  

* Describe the additional effect (besides adding the camera class and perspective matrix) you implemented.
We implemented a 'bouncing' forward and back vertical motion based off of a counter(increments with the timer) and a cosine sinusoid.  Due to the cosine, the forward motion would have a (smaller) component of vertical sinusoidal motion.  

* Given a 4x4 LookAt matrix could you recover any of the original parameters, eye, at, and up just by inspecting the matrix? If you can extract a parameter, describe which parameter you can extract and how. If you cannot recover a parameter, briefly describe why?  
Based on examination of the printmatrix, you can extract the -eye from the 4th column by pure Translation single-axis movements.  However, if a more complex transformation is performed, then the -eye becomes distorted by the rotation and then harder to see, but still gets close.  



# Quick Lab Survey

None of your answers to the questions below  will have an impact on your grade. This is to help provide feedback and improve course quality


* Approximately, how many hours did you take to complete this lab?
  *    10 Hours

* How difficult did you find this lab?
  (1-5, with 5 being very difficult and 1 being very easy)
3.5

* Describe the biggest challenge you faced on this lab.
We had trouble conceptualizing how to pan, zoom, and rotate the camera. Specifically, we had trouble understanding matrices.
