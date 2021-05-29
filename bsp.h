/* bsp.h
*
* author: MUHAMMET FATIH KESKIN - PROJE 2
*
* */

#ifndef BSP_H_
#define BSP_H_
void init_variable(void);
void init_GPIO(void);
void keypad_enable(void);
void D1_digit();
void D1_digit_with_delay(uint32_t x);
void D2_digit_with_delay(uint32_t x);
void D3_digit_with_delay(uint32_t x);
void D4_digit_with_delay(uint32_t x);
void school_id(void);
void clearSSD(void);
void setSSD(int x);
void send_result_SSD(uint32_t temp);
void dot_calc(int temp);
void clear_Rows_Keypad(void);
void set_Rows_Keypad(void);
#endif
