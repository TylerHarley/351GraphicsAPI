# 351GraphicsAPI

Graphics library written by Tyler Harley, Margaux LeBlanc, and Bruce Maxwell for the Computer Graphics course at Colby College.
With this library, you can create 2D or 3D scenes with basic polygons or 3D models. Scenes can also be lit and shaded with
Gouraud and Phong shading. All images are output as PPM files. Animations can be created by creating individual frames and splicing them together into a GIF.

How it works: all image source files (src) files set up camera, models, and lighting and write out to a PPM file(s) using the library files. Library is written to emulate some features of OpenGL, especially in the model hierarchy structure. Images are
traditionally written to the images folder.
