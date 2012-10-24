hijack-infinity
===============
Bidirectional Digital IO Between Microcontrollers and Smart Phones

Introduction
============
This code base is a work in progress. Currently it supports the MSP430FR5969 microcontroller and has an Android application. 

The code for the MSP430FR5969 will not compile unless you're running on the latest build of msp430gcc. Support for other microcontrollers is in progress, and the Makefile will need to be modified to support them.

The Android app included will display some basic digital IO and analog readings, and is modular enough to form the basis of a new application.

Adding New Microcontrollers
===========================
To add support for new microcontrollers see the platform abstraction layer (lib/pal.h). This is where all the platform-specific code is placed and should be the starting point for platform-specific implementations. 

Help!
=====
This code is provided as is. Feel free to contact me with questions, however a response is not guaranteed. 