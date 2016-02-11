#!/bin/sh

########################################################################
#
#	bar3.sh
#	This demo starts iCserver, 3 iCboxes and the control program
#	'bar3' which is generated by iCmake bar3.ic
#
#	iCbox IB1 is an analog input initialized to 1.
#	This sets the clocking speed of the running lights on QX0
#	to 100 ms.
#
#	By changing the input slider IB1 the clocking speed
#	on QX0 is set to IB1 * 100 ms.
#	A value of 0 on input IB1 lets you clock the running
#	lights manually via input IX0.0
#
#	Output QX2.0 indicates the direction of the running
#	lights on QX0 - just to show a bit of internal logic.
#
#	Stop the demo with ctrl-C
#
########################################################################

iCserver 'iCbox X0 X2 B1=1' 'bar3 -d1000'