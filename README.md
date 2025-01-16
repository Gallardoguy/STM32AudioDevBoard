# STM32AudioDevBoard
This is a STM32 audio processor board used as a guitar pedal. It features a 24 bit audio codec sampling at 96k and an STM32H7 operating at 280 Mhz. The idea and goal of this project was to replace my last pedal. In that one, I was buying a dev board and integrating it into my PCB. I wanted to cut out the dev board and design everything myself from the ground up. 
The schematic pdf can be found in the repo along with the firmware.

By taking this approach, I gain several benefits: lower costs, as I avoid purchasing development boards; a more compact design, since there's no need to integrate a dev board into my PCB; and additional cost savings by using a lower-performance STM32 and transitioning to more affordable SMT components.

## The Firmware
The firmware was not the point of this project. I went into this with the idea of it being a more hardware focused project. This is because if I can make an STM32 embedded system that functions at the hardware level, then I can really do anything in the future. The firmware can be changed as needed on a project to project basis. But more importantly, since this was intended to replace my last pedal, all my DSP code and most of my logic was just reused and modified slightly, so it's been documented in my last project.

But I should mention this was the first time I had to write a driver. I needed to configure the many registers in the SGTL5000 to get 24 bit 96k and configure the clocking, routing and what not. This was done via I2C and it was my first time using the protocol. It was pretty simple all things considered.  

I will now talk about the high level architecture. The first thing is to initialize the codec, and start the I2S DMA. I have an input and output buffer for samples for the DMA to read/write to. Once 128/2 samples have been collected, A callback fires where I process the first or second half of the 128 sample long buffer. i.e. double buffering.
I toggled the LED pin and probed with a scope to verify that the sample rate was indeed 96k. the callback fired at about 1.5kHz and each time signifies 64 samples which is 96k. Multiplied by 2 for 128 sample long buffers is 192kHz which makes sense since this is a stereo codec, so I would be receiving a 96k left and right channel.
I verified all GPIO using a jumper wire before soldering in the jacks, pots, and connecters. I shorted the ADC pins to 3.3V, 0V, and 1.8V using a jumper wire and looked at the valeus in the debugger. Similarly I set a breakpoint at interrupts and shorted input pins to ground and watched for the program to halt. This was just in case something 
didn't work, I could save some money on parts.

