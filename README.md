# rpi-pico-i2c-display-lib
A C Library for using I2C LCD displays on Raspberry Pi Pico. Essentialy a port of LiquidCrystal_I2C to RPi Pico.\
Based mostly on [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) (Arduino Library) and [rpi-pico-20x4-lcd-i2c-example](https://github.com/ParicBat/rpi-pico-20x4-lcd-i2c-example).

# How to use?
There are two ways:
1. Copy the source code to your main c file.
1. Add the files (`i2c-display-lib.c` and `i2c-display-lib.h`) to your CMakeLists.txt file

## Adding the files to CMakeLists.txt
Here's a step-by-step guide:
1. Create a folder `include`.
1. Put both `i2c-display-lib.c` and `i2c-display-lib.h `into it.
1. Add this line before the call of add_executable in your CMakeLists.txt file: `file(GLOB INCLUDE include/*.h include/*.c)`
1. Edit your add_executable line from: `add_executable(yourproj yourproj.c)` to: `add_executable(yourproj yourproj.c ${INCLUDE})`
1. Done!
