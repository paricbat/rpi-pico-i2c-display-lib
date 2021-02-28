# rpi-pico-i2c-display-lib
A C Library for using I2C LCD displays on Raspberry Pi Pico. Essentialy a port of LiquidCrystal_I2C to RPi Pico.\
Based mostly on [rpi-pico-20x4-lcd-i2c-example](https://github.com/ParicBat/rpi-pico-20x4-lcd-i2c-example) and [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) (Arduino Library).

# How to use?
There are two ways:
1. Copy the source code to your main C file.
1. Linking the files (`i2c-display-lib.c` and `i2c-display-lib.h`) to your CMakeLists.txt file.

## Linking the files to CMakeLists.txt
Here's a step-by-step guide:
1. Create a folder `rpi-pico-i2c-display-lib` in the folder where your CMakeLists is located.
1. Put both `i2c-display-lib.c` and `i2c-display-lib.h `into it.
1. Add this line right after the call of `add_executable` in your CMakeLists.txt file: `add_library(i2c-display-lib rpi-pico-i2c-display-lib/i2c-display-lib.c)`
1. Add this line right before the call of `target_link_libraries` for your main project: `target_link_libraries(i2c-display-lib pico_stdlib hardware_i2c)`
1. Modify your `target_link_libraries` call for your main project from this: `target_link_libraries(yourproject pico_stdlib)` to this: `target_link_libraries(yourproject pico_stdlib i2c-display-lib)`
1. Done!

CMakeLists.txt should look like this now:
```
cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
project(yourproject)
pico_sdk_init()

add_executable(yourproject yourproject.c)
add_library(i2c-display-lib rpi-pico-i2c-display-lib/i2c-display-lib.c)

pico_add_extra_outputs(yourprojects)

target_link_libraries(i2c-display-lib pico_stdlib hardware_i2c)
target_link_libraries(yourproject pico_stdlib i2c-display-lib)
```
# An Example
```
#include "pico/stdlib.h"
#include "i2c-display-lib/i2c-display-lib.h"


int main() {
    lcd_init();

    lcd_home(); // or lcd_setCursor(0,0);
    lcd_print("Hello World!");
}
```
