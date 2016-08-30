# Standardised Serial debugging Output. #
---
##  Introduction:  ##

Standardised Serial debugging output is a C macro library for giving a standardised Serial out put for debugging embedded code on the Arduino 320p.

The library is designed to be fast so as not to interfere with the execution of code, and it also allows for time shifting of the serial writes.

## Features: ##
- Standardised Serial output for easy machine reading.
- Time stamped for easy ordering.
- Hierarchy system for viewing location in state machines of operations. 
- Buffer for storing wrights to less time critical parts of code
- Settable buffer size.


##Usage:##


###Sending a Debug line###

	Sdebug(Loop hierarchy level,unique symbol(unique to level),variables)

	ex Sdebug(1,Y,byte_x,Float_y,...,int_z);

Repeat for final values:

	Sdebug(Loop hierarchy level,unique symbol,variables)

###Save Debug line to buffer###

	Sdebug(Loop hierarchy level,delay sending/save on buffer(B for buffer S for send now/Blank),unique symbol(unique to level),variables)

	ex Sdebug(1,Y,B,byte_x,Float_y,...,int_z);

Repeat for final values:

	Sdebug(Loop hierarchy level,delay sending/save on buffer(R for buffer S for send now/Blank),unique symbol(unique to level),variables)

	ex Sdebug(1,Y,B,byte_x,Float_y,...,int_z);


##Upload the buffer###

	Sdebug(SendBuffer):

###Increase buffer size### 

	#Sdebuffer byte value;
	
## Unique symbols ##

To successful use the debugging code you must label every state entry and exit with a unique symbol.

The symbols are the ASCII characters @ A...Z[\]^_.

The symbols must only be unique to the hierarchy level of the debug out put

eg. Level 1 A , Level 2 A will not cause a clash, 
but level 1 A, level 1 A will.


