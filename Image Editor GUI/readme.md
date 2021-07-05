# Image Editor

## What is it??
This project is a steganography project I've done as my last school assignment. 
I've worked on uses the Win32 API to create the GUI for the application and I've used a local University
Library to interface with BMP files.

## How to use it??
The instructions can be found on the `Help` button on the top left of the UI. Although it's really easy to
use since the UI is very clear.

<img src="https://github.com/FrancescoMaca/FrancescoMaca/blob/main/images/ImageEditor_Main.png" height=350> <img src="https://github.com/FrancescoMaca/FrancescoMaca/blob/main/images/ImageEditor_Find.png" height=350>

## What is steganography??
Steganography is a method used to hide data in images. Normally with steganography you could hide whatever
data you want but in my project you can only hide images in other images.  

## How does my project's logic work??
The way my code works is by overwriting an _x_ amount of LSB (Least Significant Bit) at the end of each
pixel color's byte. At the start it checks if the image that will be used to hide the data is big enough
to store it, if its not you'll need to increase the **bit depth** or either use a bigger image.
The higher the bit depth is, the easier it will be to see that there is data hidden in a image. I advise
to use a depth of 1 or 2 at maximum and to **_always_** encrypt your data before hiding it.

## Bugs

🟢 _File size missing._

🟠 _Real-time indicator to check if the source can fit in the destination at a given depth._


## Version 1.0.0
