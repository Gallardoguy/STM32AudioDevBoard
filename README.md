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

## The Hardware
I chose this STM32 since it was a little over half the clock speed of the last dev board. I only needed about 45% of the 480Mhz that was available, so 280 was a nice middle ground just in case I needed a bit more power. I chose the SGTL5000 codec from NXP because it was a cheap and readily available codec that ticked all my boxes. I wanted 24 
bits at a 96k sample rate. It also has an internal bypass I can use when disabling the effect.  One thing I also wanted to do was make sure I had low noise. So I took some precautions such as separate LDOs for analog and digital circuitry, as well as a capacitance multiplier circuit to filter the power going to the 24 bit ADC and audio op 
amps. I found the schematic for this in the book The Art of Electronics. This is also the reason why I used linear regulators rather than a switching one.

I had a STlink V3 MINIE on hand already so I used compatible header pins to program it via JTAG/SWD. This was one of the more difficult parts of the project because I was unsure of where to find the information on pinouts. I ended up finding a schematic for another STM32 board that used the same header and copied it.

The audio circuitry is basically I direct copy paste of my schematic for the last pedal. I didn't really need an input anti-aliasing filter, since the real sampling rate of the codec is quite high, something like 24.6 Mhz (256 * fs), but I thought why not? I did change the cutoff frequency since I could get away with it and reduce the BOM. The 
output anti-aliasing filter is needed for two reasons.

1. The next system the audio goes to is sampling at a lower rate i.e. 44.1k. This could cause issue if they lack a filter of their own. Now you might think, it would be a bad decision to overlook an input filter for an ADC, but my guitar amplifier is digital and does not include such filter. So I need to do everything myself. 

2. The second reason is that these codecs tend to have quite a lot of out of band noise due to the sigma delta DAC's used. They move quantization noise out of band, so I needed to remove that. Though this noise tends to be at high frequencies like 90k for example. I had to deal with this in my last pedal project.

## Assembly
I reflowed this board myself. I went to a faculty member I’m friends with on campus to get access to a hot plate. He managed to get a hold of one from the chemistry department. It was all looking good, it had digital departure control and a temp readout. It could easily do the 235C required to melt the solder paste. The problem came when I 
started placing MLCCs. The stirring magnet in the hot plate was pulling the caps off the board. In the end we disassembled the plate and removed the magnet. After this it was simple to reflow the board. I had to rework 1 cap and many of the STM pins. I believe the paste got pushed under the stencil when scraping it which caused an excess. 
Reworking it was simple enough with an iron though. 

The PCB was designed in Altium and manufactured by JLCPCB in China. This is a four layer board. The stackup is signal / GND / GND / signal. I followed good practices such as capacitors very close to Vcc pins, keeping tracks away from each other, and keeping analog and digital circuitry far away from each other. The board is mechanically the 
exact same as the last pedal. The pots, LED, jacks, and power jack are all in the same position. The board size is the same. This allowed me to reuse my last enclosure for this project as well. So it saved me time redesigning a new enclosure and making sure it and the pcb would mate.

## The mistakes
I found many mistakes as I continued testing the board. The first mistake I found was when I could not program it. It turns out I connected the crystal to the low speed external pins rather than the high speed external. So the STM32 wasn’t doing anything. So I had to swap to the internal crystal which worked out. The second thing I found is 
that JTAG wont work until I program it using SWD first. I have no clue why this is so I just rolled with it. I ended up just using SWD, but the pins are the same so it doesn’t really matter, but it was odd for sure. Another mistake was swapping the footswitch IO pins, but that was just a software fix. The biggest mistake I made was using I2S 
to communicate with the codec. I didn’t know that the STM32 I2S peripheral can only do 16 bit DMA transfers. And since 24 bit audio was being sent to the STM32, I was losing the 8 LSBs on the DMA transfer. So I really only had 16 bit audio. To fix this I would have to swap to the serial audio interface peripheral (SAI) which can do 32 bit DMA 
transfers. Although I would have to go back and redesign the schematic, pcb, and whatnot. Another mistake was with my debounce circuitry, it's not doing a good job, but to fix it I would need to add a resistor to make an RC filter. With the schmitt trigger, it should fix it up nicely. You may notice the toggle switch is missing. I thought I 
had one on hand, but it had solder lugs rather than pcb pins so I could not use it. I’ll get one next time I order parts and solder it in, but it works just fine as it is for now. I tested it by shorting the signal pins to ground with a jumper wire. Due to the supply voltage of the codec, it actually has smaller outputs than my past pedal. So 
if I reuse the same code the volume would be much lower than my last pedal, so it would be noticeable if I gave you both and you needed to guess which one is the new version. It’s not a real issue though since a clean guitar has a much smaller p-p voltage anyway and that’s really the job of the guitar amp. The final mistake / mystery I never 
really figured out was the DAC. Ideally, since this is a mono project, I could just process every other sample since the samples come as L R L R. But I could only get audio out if I processed every sample. If I failed to do so, I would literally get half the output. Half a sine wave then ground for the other half. I really don’t know why this 
is since I did verify my 96k sample rate. This would suggest that I am actually getting a 192k mono sample rate but this codec is not capable of that. In any case, this is doubling my CPU load, but thankfully this STM32 can handle it regardless. I’m not really sure what to make of this quite frankly. I tried processing the first half of the 
data, the second, only evens, only odds, but it still only works if I process all. What's even more odd, was in the debugger I was able to verify audio data coming in only on the left channel and only writing to the left indexes in the output buffer.

## What I Learned, Conclusions, and What's Next
There was a lot of learning needed to put this together. I did a lot of things for the first time so I will list them here. It was my first time using SMT components, writing a driver, reflow soldering, using STM32CUBE IDE, using I2C, I2S, and using Altium designer. I did a couple weeks of research into STM32’s to learn how to implement this. 
I had no clue how programming one of these worked so I had to learn how to set all this up from scratch. What needs to be connected to what pins, how to properly design a crystal, and how to configure the boot pins. But now I understand why some boards need you to hold boot then press reset to program via USB dfu. I also learned a lot about 
these audio codecs and how they work. If I knew they sampled at 256*Fs, I probably would have changed anti-aliasing filter in my last project. I learned a lot about navigating datasheets and application notes to implement designs.

But all in all, I believe this project was a huge success. The level of complexity of this project compared to my last pedal was night and day. I fully expected this to fail and I would need a few revisions before getting to this point. And while I did make many mistakes, I can play guitar through it and the effects work just like my last 
pedal. And it only took one try!



What I would change:
I dislike this audio codec package. When I reflowed the board, I had absolutely no clue whether this thing soldered correctly. It is an extremely tiny 3mm x 3mm QFN package and the pads are microscopic. Even with a magnifying glass I really couldn't tell if there was a short so I just reworked it a bit anyway. I also need different solder 
paste. I was using T4 paste from a jar, but it seems a bit dry and doesn't spread easily. If I had used a more liquidy T5 paste from a syringe I think the stencil would have been much easier to work with. 

I’m all about pushing my boundaries and experimenting with new things. This time it was designing an STM32, next time it will be an FPGA. I plan on implementing another guitar effect pedal, but this time on an FPGA, I’ll probably also change the effect to something else. But The level of hardware complexity needed for an FPGA dwarfs an STM32 
based on the research I’ve done so far. So hopefully I’ll have that ready to show off soon.
