# Ray Tracing In One Weekend Series

Just my implementation of the book series by Peter Shirley et al.

The changes I've made to the original:

* Using stb to write images to PNGs instead of using the PPM format
    * Just a personal choice
    * Added automatically generated filenames for the images instead of manually changing the filename before compiling
* Added multithreading to improve rendering performance