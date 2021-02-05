// Original by raspberrypi/pico-examples/i2c/lcd_1602_i2c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "i2c-display-lib.h"

static int lcd_addr = 0x27;

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, lcd_addr, &val, 1, false);
}

void lcd_setAddr(int addr)
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
void lcd_send_byte(uint8_t val, int mode) {
	uint8_t high;
	uint8_t low;

	if(mode != 1)
	{
		uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
		uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;
	}
	else {
		uint8_t high = mode | (val & 0xF0);
		uint8_t low = mode | ((val << 4) & 0xF0);
	}

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_setCursor(int line, int position) {
    int line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    int val = 0x80 + line_offsets[line] + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_write(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_print(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	lcd_send_byte(LCD_SETCGRAMADDR | (location << 3), LCD_COMMAND);
	for (int i=0; i<8; i++) {
		lcd_send_byte(charmap[i], 1);
	}
}

void lcd_init(int sda, int scl) {
	i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

	// Make the I2C pins available to picotool
    bi_decl( bi_2pins_with_func(sda, scl, GPIO_FUNC_I2C));

    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}