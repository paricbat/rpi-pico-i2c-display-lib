// Original Code: raspberrypi/pico-examples (i2c/lcd_1602_i2c folder) and johnrickman/LiquidCrystal_I2C

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

uint8_t lcd_Addr = 0x27;
uint8_t lcd_displayfunction;
uint8_t lcd_displaycontrol;
uint8_t lcd_displaymode;
uint8_t lcd_numlines = 4;
uint8_t lcd_cols = 20;
uint8_t lcd_rows = 4;
uint8_t lcd_backlightval = LCD_NOBACKLIGHT;

#define I2C_PORT i2c0

void lcd_set_cols(uint8_t cols) { lcd_cols = cols; }
void lcd_set_rows(uint8_t cols) { lcd_cols = cols; }

/* Quick helper function for single byte transfers */
void lcd_expanderWrite(uint8_t val) {
    i2c_write_blocking(I2C_PORT, lcd_Addr, &val, 1, false);
}

/************ low level data pushing commands **********/

void lcd_pulseEnable(uint8_t _data){
	lcd_expanderWrite(_data | En);	// En high
	sleep_us(1);		// enable pulse must be >450ns
	
	lcd_expanderWrite(_data & ~En);	// En low
	sleep_us(50);		// commands need > 37us to settle
} 

void lcd_write4bits(uint8_t value) {
	lcd_expanderWrite(value);
	lcd_pulseEnable(value);
}

// write either command or data
void lcd_send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
    lcd_write4bits((highnib)|mode);
	lcd_write4bits((lownib)|mode); 
}


inline void lcd_write(uint8_t value) {
	lcd_send(value, Rs);
}

/*********** mid level commands, for sending data/cmds */

inline void lcd_command(uint8_t value) {
	lcd_send(value, 0);
}

// Turn the display on/off (quickly)
void lcd_noDisplay() {
	lcd_displaycontrol &= ~LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

/********** high level commands, for the user! */
void lcd_print(const char *text)
{
	while(*text)
	{
		lcd_write(*text);
		++text;
	}
}

void lcd_display() {
	lcd_displaycontrol |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

void lcd_clear(){
	lcd_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	sleep_us(2000);  // this command takes a long time!
}

void lcd_home(){
	lcd_command(LCD_RETURNHOME);  // set cursor position to zero
	sleep_us(2000);  // this command takes a long time!
}

void lcd_setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > lcd_numlines ) {
		row = lcd_numlines-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turns the underline cursor on/off
void lcd_noCursor() {
	lcd_displaycontrol &= ~LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}
void lcd_cursor() {
	lcd_displaycontrol |= LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink() {
	lcd_displaycontrol &= ~LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}
void lcd_blink() {
	lcd_displaycontrol |= LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcd_scrollDisplayLeft(void) {
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void lcd_scrollDisplayRight(void) {
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
	lcd_displaymode |= LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
	lcd_displaymode &= ~LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
	lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | lcd_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
	lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	lcd_command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		lcd_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void lcd_noBacklight(void) {
	lcd_backlightval=LCD_NOBACKLIGHT;
	lcd_expanderWrite(0);
}

void lcd_backlight(void) {
	lcd_backlightval=LCD_BACKLIGHT;
	lcd_expanderWrite(0);
}

// Alias functions

void lcd_cursor_on(){
	lcd_cursor();
}

void lcd_cursor_off(){
	lcd_noCursor();
}

void lcd_blink_on(){
	lcd_blink();
}

void lcd_blink_off(){
	lcd_noBlink();
}

void lcd_load_custom_character(uint8_t char_num, uint8_t *rows){
		lcd_createChar(char_num, rows);
}

void lcd_setBacklight(uint8_t new_val){
	if(new_val){
		lcd_backlight();		// turn backlight on
	}else{
		lcd_noBacklight();		// turn backlight off
	}
}

void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	if (lines > 1) {
		lcd_displayfunction |= LCD_2LINE;
	}
	lcd_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		lcd_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	sleep_ms(50); 
  
	// Now we pull both RS and R/W low to begin commands
	lcd_expanderWrite(lcd_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	sleep_ms(1000);

  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	  // we start in 8bit mode, try to set 4 bit mode
   lcd_write4bits(0x03 << 4);
   sleep_us(4500); // wait min 4.1ms
   
   // second try
   lcd_write4bits(0x03 << 4);
   sleep_us(4500); // wait min 4.1ms
   
   // third go!
   lcd_write4bits(0x03 << 4); 
   sleep_us(150);
   
   // finally, set to 4-bit interface
   lcd_write4bits(0x02 << 4); 


	// set # lines, font size, etc.
	lcd_command(LCD_FUNCTIONSET | lcd_displayfunction);  
	
	// turn the display on with no cursor or blinking default
	lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_display();
	
	// clear it off
	lcd_clear();
	
	// Initialize to default text direction (for roman languages)
	lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
	
	lcd_home();
  
}

void lcd_init()
{
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    // Make the I2C pins available to picotool
    bi_decl( bi_2pins_with_func(4, 5, GPIO_FUNC_I2C));

	lcd_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	lcd_begin(lcd_cols, lcd_rows, 0);  
}