This is an example of support for the CH32V003 with the WeAct 2.13-inch Black-White-Red eInk display.
It uses the RREFont library to print fonts on the display.
The CH32V003 has very limited RAM, making it impossible to create a memory buffer for the entire display memory and paint fonts there.
Some memory optimization has been applied to render fonts on this chipset. Due to the specific memory layout of this display, it is necessary to store a whole display block of dimensions 250x8 at once.
Here is a simple approach that has been implemented and can be extended and polished to achieve a more mature solution.
Currently, it is possible to render text lines on the display and specify the colors of particular characters (black or red).
The position of a text line must be a multiple of 8 (due to the memory layout of the display).

Following resources and implementation are used to achieve the current result:

https://github.com/WeActStudio/WeActStudio.EpaperModule/tree/master

https://github.com/cbm80amiga/RREFont
