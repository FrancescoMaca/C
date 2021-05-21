This project is a steganography project I've done as my last school assignment. 
I've worked on uses the Win32 API to create the GUI for the application and I've used a local University
Library to interface with BMP files.

## *_What is steganography??_*
Steganography is a method used to hide data in images. Normally with steganography you could hide whatever
data you want but in my project you can only hide images in other images.  

## *_How does my project's logic work??_*
The way my code works is by overwriting an _x_ amount of LSB (Least Significant Bit) at the end of each
pixel color's byte. At the start it checks if the image that will be used to hide the data is big enough
to store it, if its not you'll need to increase the **bit depth** or either use a bigger image.
The higher the bit depth is, the easier it will be to see that there is data hidden in a image. I advise
to use a depth of 1 or 2 at maximum and to **_always_** encrypt your data before hiding it.
