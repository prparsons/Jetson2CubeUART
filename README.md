# Jetson2CubeUART
Best I've been able to determine, the issues with Nvidia Jetson dev boards (Xavier NX dev kit in my case) communicating with various cube autopilots is due to a weak serial driver in the Jetson dev kit not being able to overcome the pull down resistors used in the Cube hardware.  This appeared to be the issue I was suffering: when viewing with oscilloscope it wasn't pulling the serial line all the way to ground (roughly 0.8V), and it's noisy when "low".

So to overcome, I simply used an msp430g2553 to relay the signal back and forth between the 2 devices so I could choose appropriate resistors for the Jetson, while the msp is able to sink enough current from the Cube.

One thing to note, probably should change it so when it's outputting high it instead changes to an input and lets the pullup resistor handle things.

Compiled with TI Code Composer Studio 10, programmed via the TI Launchpad.
