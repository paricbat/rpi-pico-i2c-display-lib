#ifndef I2CDISPLAYLIB_H
#define I2CDISPLAYLIB_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

#define I2C_PORT i2c0

void lcd_set_cols(uint8_t cols);
void lcd_set_rows(uint8_t rows);

void lcd_begin(uint8_t cols, uint8_t rows, uint8_t charsize);
void lcd_clear();
void lcd_home();
void lcd_noDisplay();
void lcd_display();
void lcd_noBlink();
void lcd_blink();
void lcd_noCursor();
void lcd_cursor();
void lcd_scrollDisplayLeft();
void lcd_scrollDisplayRight();
void lcd_printLeft();
void lcd_printRight();
void lcd_leftToRight();
void lcd_rightToLeft();
void lcd_shiftIncrement();
void lcd_shiftDecrement();
void lcd_noBacklight();
void lcd_backlight();
void lcd_autoscroll();
void lcd_noAutoscroll(); 
void lcd_createChar(uint8_t location, uint8_t charmap[]);
 
void lcd_setCursor(uint8_t col, uint8_t row); 
void lcd_write(uint8_t value);
void lcd_command(uint8_t);
void lcd_init();
void lcd_oled_init();

////compatibility API function aliases
void lcd_blink_on();						// alias for blink()
void lcd_blink_off();       					// alias for noBlink()
void lcd_cursor_on();      	 					// alias for cursor()
void lcd_cursor_off();      					// alias for noCursor()
void lcd_setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
void lcd_load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
void lcd_print(const char* text);

void lcd_send(uint8_t value, uint8_t mode);
void lcd_write4bits(uint8_t value);
void lcd_expanderWrite(uint8_t val);
void lcd_pulseEnable(uint8_t data);

uint8_t lcd_Addr;
uint8_t lcd_displayfunction;
uint8_t lcd_displaycontrol;
uint8_t lcd_displaymode;
uint8_t lcd_numlines;
uint8_t lcd_cols;
uint8_t lcd_rows;
uint8_t lcd_backlightval;

#endif