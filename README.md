hijack-infinity
===============
Bidirectional Digital IO Between Microcontrollers and Smart Phones

Introduction
============
This code base is a work in progress. Currently it supports the MSP430FR5969 and MSP430F1611 microcontroller and has an Android application. 

The code for the MSP430FR5969 will not compile unless you're running on the latest build of msp430gcc. Support for other microcontrollers is in progress, and the Makefile will need to be modified to support them.

The Android app included will display some basic digital IO and analog readings, and is modular enough to form the basis of a new application.

MSP403F1611 Support
===================
Support for the MSP430F1611, compatiable with the HiJack boards sold at SeeedStudio, is at a semi-working state. Currently we operate at a rather slow baud rate, but are in-fact able to send bidirectional digital data. You'll need a way to program the HiJack board, I'd recommend using the HiJack programmer. HiJack was originally designed to be programmed with the binary serial loaded from TinyOS, take a look at the firmware installed on the Seeed studio WiKi page for an example of how to program it. Once you have the tos-bsl you can program it using the following command string:

```tos-bsl --swap-reset-test --invert-reset --invert-test -r -e -I -p out.hex```

Some issues with the current implementation
-------------------------------------------
  - The abstractions aren't the most efficient things out there, excessive computation when the periodic timer polls for a new bit causes jitter in the microcontroller-outputted signal and increases the probability for error.
  - We're using both TimerA and TimerB on the MSP430, it'd be nice to move the implementation to only use one timer and do a little bit of virtualization
  - Baud rate is a little slow, better filtering on the phone-side could help to fix this.
  - *Power draw still remains high*, this device will only be compatible with a select number of Android devices. For all other devices you will have to supply power externally. Devices that have Beats by Dr. Dre have been found to work well with HiJack, the extra amplification provides enough energy to drive the board. Supply HiJack with 2V of regulated power and disable the portion of Android code that generates the 22kHz tone for power and this code should work with many more phones.
  - Some Android devices don't support the 44.1kHz sampling rate, or have processors too slow to process the audio data in real-time. You'll either have to capture and process in blocks, or find a different phone. 

iOS Support
===========
Support for iOS is planned for the future. As of right now, the HiJack code for iOS is pretty unorganized, so refactoring it to be compatible with the communication scheme I layered on top of the basic byte-oriented interface and building the test app is something I'm still hoping to complete. 

Adding New Microcontrollers
===========================
To add support for new microcontrollers see the platform abstraction layer (lib/pal.h). This is where all the platform-specific code is placed and should be the starting point for platform-specific implementations. It should be reasonably easy to add new microcontrollers, support for the F1611 was added within a day. 

Help!
=====
This code is provided as is. Feel free to contact me with questions, however a response is not guaranteed. 
