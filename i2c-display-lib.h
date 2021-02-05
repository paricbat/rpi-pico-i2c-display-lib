#ifndef I2CDISPLAY_H
#define I2CDISPLAY_H

#define I2C_PORT i2c0

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

void i2c_write_byte(uint8_t val);
void lcd_setAddr(uint8_t addr);
void lcd_toggle_enable(uint8_t val);
void lcd_send_byte(uint8_t val, uint8_t mode, uint8_t backlight);
void lcd_clear(void);
void lcd_setCursor(uint8_t line, uint8_t position);
void lcd_write(char val);
void lcd_print(const char *s);
void lcd_createChar(uint8_t location, uint8_t charmap[]);
void lcd_init(uint8_t sda, uint8_t scl);

void lcd_home();

#endif