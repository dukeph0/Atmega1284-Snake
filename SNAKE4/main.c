/*
 * SNAKE3.c
 *
 * Created: 3/3/2020 3:50:13 AM
 * Author : Duke Pham
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include "snake.h"
#include "nokia5110.h"
#include "SPI_Master_H_file.h"
#include "ADC_H.h"
#include "io.h"
#include "LCD_16x2_H_file.h"


void set_borders(void);
void switch_keys(void);

volatile unsigned short Timer1;
unsigned char game_on;
unsigned char key1_lock, key2_lock, key3_lock, key4_lock;

int ADC_ValueX;
int ADC_ValueY;
int ADC_ValueB;
int i = 0;

int main(void)
{

		
	//Timer2
	OCR2A 	= 78;
	TCCR2A 	|= (1<<WGM21);
	TCCR2B 	|= (1<<CS22)|(1<<CS21)|(1<<CS20);
	TIMSK2 	= (1<<OCIE2A);
	
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Initialize the LCD Display
	//LCD_init();
	
	
	check_and_load_defaults();
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_draw();
	nokia_lcd_render();
	_delay_ms(3000);
	nokia_lcd_clear();
	_delay_ms(500);

	snake_init(ram_cfg.users_cnt);
	
	nokia_lcd_render();
	sei(); // enable interrupts
	game_on = 1;
	
	ADC_Init();
	//LCD_Init();
    /* Replace with your application code */
    while (1) 
    {

		ADC_ValueX = ADC_Read(0);
		ADC_ValueY = ADC_Read(1);
		ADC_ValueB = ADC_Read(2); // reset button

			if(!Timer1) 
			{
				switch_keys();
				set_borders();
				// Start snake game
				if(snakes(0)->draw_snake_fun) snakes(0)->draw_snake_fun(snakes(0));
				Timer1 = ram_cfg.speed;
				
			}	
			//LCD_Char("Score:");
	}
} 


// Set Borders
void set_borders(void)
{
	//Y-borders
	for(int i = 1; i < 44; i++)
	{
		nokia_lcd_set_pixel(0, i, 1);
		nokia_lcd_set_pixel(82, i, 1);
	}
	
	//X-borders
	for(int i = 0; i < 83; i++)
	{
		nokia_lcd_set_pixel(i, 1, 1);
		nokia_lcd_set_pixel(i, 44, 1);
	}
}

// Set Joystick Controls
void switch_keys(void)
{
	
	if((ADC_ValueY >= 800) && (ADC_ValueX <= 600 && ADC_ValueX >= 400)){
		// initiates when up on joystick
		snakes(0)->direction = up;
	} else if((ADC_ValueY <= 200) && (ADC_ValueX <= 600 && ADC_ValueX >= 400)){
		// initiates when down on joystick
		snakes(0)->direction = down;
	} else if((ADC_ValueX <= 200) && (ADC_ValueY <= 600 && ADC_ValueY >= 400)){
		// initiates when right on joystick
		snakes(0)->direction = right;
	} else if((ADC_ValueX >= 800) && (ADC_ValueY <= 600 && ADC_ValueY >= 400)){
		// initiates when left on joystick
		snakes(0)->direction = left;
	}
	//LCD_String("Score:");
	
} 

void LCD_16x2_Score()
{
	char R_array[15],W_array[15] = "EEPROM TEST";
	LCD_Init();
	memset(R_array,0,15);
	eeprom_busy_wait();		/* Initialize LCD */
	eeprom_write_block(W_array,0,strlen(W_array));	/* Write W_array 
							from EEPROM address 0 */
	eeprom_read_block(R_array,0,strlen(W_array));	/* Read EEPROM 
							from address 0 */
	LCD_String(R_array);		/* Print Read_array on LCD */
	//return(0);
}

// Set ISR Timer
ISR (TIMER2_COMPA_vect)
{
	unsigned short n;

	n = Timer1;
	if(n) Timer1 = --n;
}