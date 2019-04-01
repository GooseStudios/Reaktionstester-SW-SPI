#include <u8g2.h>
#include <util/delay.h>
#include <u8x8_avr.h>
#include <avr/power.h>

u8g2_t u8g2;
//GPIO DELAY fuer SPI
uint8_t u8g2_gpio_and_delay_pcd8544(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr){
	
	switch(msg){
		case U8X8_MSG_GPIO_AND_DELAY_INIT: //SPI wird initialisiert
			break;
      
		case U8X8_MSG_DELAY_MILLI: //Hier wird eine Millisekunde verzoegert
			//_delay_ms(arg_int);
			for(uint8_t i=arg_int; i > 0; i--) { // Modified here, AVR5
				_delay_ms(1);
			}		
			break;
      
		case U8X8_MSG_DELAY_10MICRO: //es wird 10 Microsekunden verzoegert
			_delay_us(10);
			break;
  
		case U8X8_MSG_GPIO_SPI_CLOCK: //SPI Clock
			if (arg_int == 0) { // arg_int=0: Output low at SCLK pin
				DDRB |= _BV(DDB5);
				PORTB &= ~(_BV(PORTB5));
			} else {		   // arg_int=1: Interne Pullup wird High an SCLK
				DDRB &= ~(_BV(PORTB5));	 // The same with the rest of pins
				PORTB |= _BV(PORTB5);
			}
			break;
      
		case U8X8_MSG_GPIO_SPI_DATA: //Die Datenleitung Display SDA
			if (arg_int == 0) { // arg_int=0: Mosi Pin wird Low
				DDRB |= _BV(DDB3);
				PORTB &= ~(_BV(PORTB3));
			} else {	            // arg_int=1: Interne Pullup wird High an MOSI
				DDRB &= ~(_BV(PORTB3));
				PORTB |= _BV(PORTB3);
			}	
			break;	
      
		case U8X8_MSG_GPIO_CS:	//Display Pin CS
			if (arg_int == 0) { // arg_int=0: OC1B Pin wird Low
				DDRB |= _BV(DDB2);
				PORTB &= ~(_BV(PORTB2));
			} else {	            // arg_int=1: Interne Pullup wird High an OC1B
				DDRB &= ~(_BV(PORTB2));
				PORTB |= _BV(PORTB2);
			}	
			break;
      
		case U8X8_MSG_GPIO_DC:	//Display Pin DC	
			if (arg_int == 0) { // arg_int=0: OC1A Pin wird Low
				DDRB |= _BV(DDB1);
				PORTB &= ~(_BV(PORTB1));
			} else {	            // arg_int=1: Interne Pullup wird High an OC1A
				DDRB &= ~(_BV(PORTB1));
				PORTB |= _BV(PORTB1);
			}
			break;
      
		case U8X8_MSG_GPIO_RESET: //Display Pin RESET
			if (arg_int == 0) { // arg_int=0: Reset Pin wird Low
				DDRC |= _BV(DDC6);
				PORTC &= ~(_BV(PORTC6));
			} else {	            // arg_int=1: Interne Pullup wird High an RESET
				DDRC &= ~(_BV(PORTC6));
				PORTC |= _BV(PORTC6);
			}
			break;
      
		default:
			return 0; //A message was received which is not implemented, return 0 to indicate an error
		}							
	return 1; // command processed successfully.
}

void Display_Init (void)
{
             //Displaytyp mit Auflösung           Rotation Kommunikation           GPIO DELAY
	u8g2_Setup_sh1106_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_pcd8544);
	u8g2_InitDisplay(&u8g2); //Display wird initialisiert
	u8g2_SetPowerSave(&u8g2, 0); //Display erwacht
}

int main (void)
 {
	Display_Init();

  while (1) 
  {
    u8g2_ClearBuffer(&u8g2); //Loescht den Buffer
	
    u8g2_SetFont(&u8g2, u8g2_font_sandyforest_tu); //Aendern der Schrift
	u8g2_DrawLine(&u8g2, 10, 5, 120, 5); //Zeichnet eine Linie
    u8g2_DrawStr(&u8g2, 40, 17, "SPI WORKS"); //Schreibt den String
	
	u8g2_SetFont(&u8g2, u8g2_font_6x13_mr); 
	u8g2_DrawStr(&u8g2, 60, 29, "ON");
	
	u8g2_SetFont(&u8g2, u8g2_font_t0_12b_mr);
	u8g2_DrawLine(&u8g2, 10, 45, 120, 45);
	u8g2_DrawStr(&u8g2, 20, 43, "REAKTIONSTESTER!");
	
	u8g2_DrawLine(&u8g2, 10, 6, 10, 45);
	u8g2_DrawLine(&u8g2, 120, 6, 120, 45);

	u8g2_SendBuffer(&u8g2); //Sendet den Buffer neu
  }
}
