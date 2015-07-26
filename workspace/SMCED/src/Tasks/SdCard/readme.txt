This is ChaN's FatFS
with an SD card implementation for NXP's LPC17xx SPI port.

The base implementation was retrieved from:
AN10916 "FAT library EFSL and FatFs port on NXP LPC1700"

Changed by Rob Jansen to support SSP1, including support for
the Embedded Artists Base Board.

--------------------------------------------------------------------
There are two configuration options in lpc1xxx_spi_sd/lpc17xx_spi.c:
--------------------------------------------------------------------
SSP: defined as either 0 or 1 depending on the SSP port used
USE_EA_BASEBOARD: defined as 1 when the EA Base Board is used

Please note that SSP must be 1 when USE_EA_BASEBOARD is 1.


--------------------------------------------------------------------
PLEASE READ: Notes on testing
--------------------------------------------------------------------
This software has only been tested on the EA Base Board using SD Card
and on my personal lpc1754 module using micro SD Card (CS line on P0.6)

SSP0 has not been tested by me and is not guaranteed to work !!!

--------------------------------------------------------------------
Porting to other boards / processors
--------------------------------------------------------------------
There are some defines in the lpc17xx_spi.c file that control which
SSP is used and which GPIO line is used as CS for the SD Card.
Please note that the I/O configuration done in SPI_Init is currently
NOT USING THESE DEFINES !!!
You must adjust the GPIO assignment in SPI_Init() also.

For porting this to another lpc1xxx processor, add the SPI files for
the processor in the lpc1xxx_spi_sd directory. Then exclude the lpc17xx
files by excluding them from the build