# RF24 Libraries Cleanup Project #
Each component of the _RF24_ set of libraries by _TMRh20_ will have a **CLEANUP.md** file, which shall contain notes
about what has been done to cleanup and/or improve the library.

## Objectives ##
_Please note that the list of objectives is likely to change._

Some of the more immediate objectives include:
- Cleanup whitespace
  - Remove excess whitespace
  - Properly indent code
  - Replace space characters with tab characters
- Ensure consistency in hardware configuration across examples

And some ideal objectives:
- Define all hardware configuration in one file per repository, to be included in each example
- Better yet, for Linux platforms, read a configuration file from $PREFIX/etc (ie: /usr/local/etc/RF24/RF24_hardware.cfg)
- Have example apps take arguments
  - Display usage information for invalid arguments, or if invoked with **-h** or **--help**


## Progress Notes ##
- **2016-09-07**: Cleaned up whitespace in quite a few files


## Other Notes ##

### CFLAGS ###

	-march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -O2 -Wall -pthread -pipe -fstack-protector --param=ssp-buffer-size=4
