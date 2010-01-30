Introduction
---------------
This is the base code library written for the Xiphos 1.0 Board designed by 
Darron Baida for his BSEE senior project at Cal Poly (completed June 2009).
The original version of the code was written by Patrick J. McCarty in May 2009.
Additional contributions have been made by:
	- <no one yet>

Motivation for PolyBot 2.0 Board
-----------------------------------
This code library was inspired by the 
PolyBot 1.0 and 1.1 Boards designed by Cal Poly Computer Science professor 
Dr. John Seng. After using Dr. Seng's original boards for several years for 
projects within the Cal Poly Robotics Club, we appreciated how easily the 
board could be used, but we often found the board too limiting or wished that 
it had certain additional features built-in. In order to address these 
limitations and shortcomings, an entirely new board was designed that 
represents a break with the old design. Whereas the old design avoided 
surface mount chips to allow easy assembly, Xiphos embraces SMT technology 
to take advantage of the additional I/O pins available in SMT packages. Xiphos 
also adds numerous new built-in features, while striving to be as flexible as 
possible. Xiphos facilitates easy interfacing with devices running at lower 
voltages, as well as providing a socket for an XBee radio transceiver. Other 
important features for robotics/mechatronics use include a hefty voltage 
regulator, allowing servo power to be regulated, two heavy-duty H-bridges, 
avoiding the need to purchase a costly additional motor controller for 
medium-size robotics projects, and hardware PWM control of the H-bridges, 
avoiding the performance hit of using software interrupts.

Non-compatibility with PolyBot 1.0/1.1
-----------------------------------------
As mentioned in the above section, the Xiphos 1.0 board design represents a 
significant break with the previous PolyBot boards. Consequently, it was
decided to not attempt to make the XiphosLibrary backwards compatible with the 
previous libraries. Adapting old code to use the new library shouldn't be too 
hard, but the user should be aware of differences between the ATmega chips, 
the board versions, and library functions.

Software License
------------------
TODO

Improvements to the code are always welcome.
