<h1 align = "center">DDS9833</h1>

## ad9833 [library1](https://www.best-microcontroller-projects.com/ad9833.html) 


## ad9833 [library2](https://github.com/y-x-c/wearable-microphone-jamming/blob/master/jammer_hardware_source/arduino_code/wearable_microphone_jammer/wearable_microphone_jammer.ino)

***

## Schématics

![TTGO](doc/schematics.png "DDS SCH")

CONNECTION
    --------
    AD9833 PCB to Pro Trinket board:
    DAT --> pin 11 (MOSI) [note: pin 12 is MISO]
    CLK --> pin 13 (SCK)
    FSY --> pin A0 (AD9833 SS)
    CS --> pin A1 (MCP41010i SS)
    
    AD9833 PCB OVERVIEW
    --------
    PCB has following INPUTS (in order): CS, DAT, CLK, FSY, GND, VCC
                  and OUTPUTS (in order): GND, PGA, (footprint for SMA connector), VOUT, GND
    LOOKING AT PCB WITH INPUTS FACING YOU: left side = PGA, right side = waveform generator
    (left side)
    PGA stands for Programmable Gain Amplifier and is constructed of a MCP41010i digital potentiometer (addressed via SPI)
    and an AD8051 high speed operational amplifier. By changing the MCP41010i resistance value, you set the amplifier's gain.
    The SMA connector footprint also connects to the PGA output.
    MCP41010i is on the SPI bus, thus has a shared connection to DAT and CLK with the AD9833. Its chip select is the CS input.
    MCP41010i [datasheet](http://ww1.microchip.com/downloads/en/devicedoc/11195c.pdf)
    AD8051 [datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/AD8051_8052_8054.pdf)
    (right side)
    A large 25 MHz xtal on top half of board. It determines the reference frequency used in the code below. AD9833 situated
    on bottom half of board. This part is completely controlled via SPI.
    AD9833 is on the SPI bus, thus has a shared connection to DAT and CLK with the MCP41010i. Its chip select is the FSY input.
    AD9833 [datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/AD9833.pdf)
	


