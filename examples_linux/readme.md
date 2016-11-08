# RF24 -- Linux Examples #
**Note:** _These examples were originally designed for the **Raspberry Pi**, but they **should** work on any supported Linux platform, with the proper pin and software configuration._
_See [The RF24 Documentation](http://tmrh20.github.io/RF24) for more information._

## Summary of Examples ##
The examples for Linux include a variety of simple **C++** programs, as well as a **Python library**, and an example **Python script** to go along with it.

### C++ Examples ###
The following list describes the structure of the files and directories:
- extra/
  - rpi-hub.cpp
  - scanner.cpp
- interrupts/
  - gettingstarted\_call\_response\_int.cpp
  - gettingstarted\_call\_response\_int2.cpp
  - pingpair\_dyn\_int.cpp
  - transfer\_interrupt.cpp
- gettingstarted.cpp
- gettingstarted\_call\_response.cpp
- pingpair\_dyn.cpp
- transfer.cpp

### Python Example ###
Despite the Python library itself being located in **../pyRF24**, the example Python script is located in the current directory, as follows:
- pingpair\_dyn.py