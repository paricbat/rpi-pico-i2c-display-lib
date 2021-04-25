#ifndef I2CDISPLAY_H
#define I2CDISPLAY_H
// Original by raspberrypi/pico-examples/i2c/lcd_1602_i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "i2c-display-lib.h"

#define I2C_PORT i2c0

//
// Variables
//

// commands
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// Modes for lcd_send_byte
const int LCD_CHARACTER = 1;
const int LCD_COMMAND = 0;

static int lcd_addr = 0x27;

//
// Functions
//

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, lcd_addr, &val, 1, false);
}

void lcd_setAddr(uint8_t addr)
{
    lcd_addr = addr;
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, uint8_t mode, uint8_t backlight) {
    uint8_t high;
    uint8_t low;

    if(backlight)
    {
        high = mode | (val & 0xF0) | LCD_BACKLIGHT;
        low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;
    }
    else {
        high = mode | (val & 0xF0);
        low = mode | ((val << 4) & 0xF0);
    }

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND, 1);
}

// go to location on LCD
void lcd_setCursor(uint8_t line, uint8_t position) {
    uint8_t line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    uint8_t val = 0x80 + line_offsets[line] + position;
    lcd_send_byte(val, LCD_COMMAND, 1);
}

void lcd_write(char val) {
    lcd_send_byte(val, LCD_CHARACTER, 1);
}

void lcd_print(const char *s) {
    while (*s) {
        lcd_write(*s++);
    }
}

void lcd_createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    lcd_send_byte(LCD_SETCGRAMADDR | (location << 3), LCD_COMMAND, 1);
    for (int i=0; i<8; i++) {
        lcd_send_byte(charmap[i], 1, 0);
    }
}

void lcd_home() {lcd_setCursor(0,0);}

void lcd_init(uint8_t sda, uint8_t scl) {
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    // Make the I2C pins available to picotool
    // bi_decl( bi_2pins_with_func(sda, scl, GPIO_FUNC_I2C)); For some reason this produces an error.

    lcd_send_byte(0x03, LCD_COMMAND, 1);
    lcd_send_byte(0x03, LCD_COMMAND, 1);
    lcd_send_byte(0x03, LCD_COMMAND, 1);
    lcd_send_byte(0x02, LCD_COMMAND, 1);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND, 1);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND, 1);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND, 1);
    lcd_clear();
}

#endif
