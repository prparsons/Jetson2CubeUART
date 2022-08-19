# Jetson2CubeUART
It appears that the Nvidia Jetson dev boards (Xavier NX dev kit in my case) may have problems communicating with the some of the <a href='https://www.cubepilot.com/'>CubePilot Autopilots</a> (Pixhawk 2.1). The issue stems from a weak serial driver in the Jetson dev kit not being able to overcome the pull up resistors used in the Cube hardware.  This appeared to be the issue I was suffering: when viewing the oscilloscope I could see that the Jetson wasn't pulling the serial line all the way to ground. It was roughly 0.8V and it was quite noisy.

So to overcome, I simply used an msp430g2553 to relay the signal back and forth between the 2 devices so I could choose appropriate resistors for the Jetson, while the msp is able to sink enough current from the Cube.

One thing to note, could change it so when it's outputting high it instead changes to an input and lets the pullup resistor handle things. Then again could probably just design a driver circuit that doesn't need a microcontroller anyways.

Compiled with TI Code Composer Studio 10, programmed via the TI Launchpad.
