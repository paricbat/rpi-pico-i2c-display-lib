#ifndef I2CDISPLAY_H
#define I2CDISPLAY_H

#define I2C_PORT i2c0

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

void i2c_write_byte(uint8_t val);
void lcd_setAddr(int addr);
void lcd_toggle_enable(uint8_t val);
void lcd_send_byte(uint8_t val, int mode);
void lcd_clear(void);
void lcd_setCursor(int line, int position);
static void inline lcd_write(char val);
void lcd_print(const char *s);
void lcd_createChar(uint8_t location, uint8_t charmap[]);
void lcd_init(int sda, int scl);

void lcd_home();

#endif