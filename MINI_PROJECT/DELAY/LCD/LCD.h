#include"types.h"

void lcd_init(void);
void lcd_cmd(u8 cmd);
void lcd_char(s8 data);
void lcd_str(s8 *);
void lcd_int(s32 num);
void lcd_f32(f32 num , u32 nDP);
void WriteToCGRAM(s8 *a);

