# Raspberry Pi Pico W Tests
Testing various hardware with the pico_w.

### Usage:
Clone and build.
```
git clone https://github.com/kubakos/pico-test.git
cd pico-test
mkdir build
cd build
cmake ..
make
```

Then copy the .UF2 to the Pico from the build directory of the corresponding hardware. 
To be able to see the recieved data from the pico you will need a serial communication program (e.g. bootterm, minicom).


### TODO
- wifi communication
- RTOS
- microswitch
- OLED panel
- stepper motor control
