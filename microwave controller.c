#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
/**********LCD PORTS***********

RS=PD0   Rgr select
RW=PD1   Rgr read or write
EN=PD2   Enable the clock of the LCD
D0=PA7
D1=PA6
D2=PA5
D3=PB4
D4=PE5
D5=PE4
D6=PB1
D7=PB0
Dx is the input data bit

**********KEYPAD PORTS***********

PE0 --> column1
PE1 --> column2
PE2 --> column3
PE3 --> column4

PC4 --> row1
PC5 --> row2
PC6 --> row3
PC7 --> row4

***********BUZZER PORT***********

PB3 --> Buzzer

*/

#define on 1
#define off 0
void portb_init(){
		SYSCTL_RCGCGPIO_R |= 0x02;
		while((SYSCTL_PRGPIO_R & 0x02)==0);
		GPIO_PORTB_DIR_R |= 0x1B; //set B0-B1 and B3-B4 as output pins
		GPIO_PORTB_DEN_R |=0x1B;
		GPIO_PORTB_AMSEL_R &= ~0x1B;
		GPIO_PORTB_AFSEL_R &= ~0x1B;
		GPIO_PORTB_PCTL_R &=~0x000FF0FF;
	SYSCTL_RCGCGPIO_R |= 0x04;        //Enable clock to PORTC and PORTE  
  while ((SYSCTL_RCGCGPIO_R&0x04)==0);  //wait for clock to be setted
  GPIO_PORTC_CR_R  |= 0xF0;             //Allow settings for all pins of PORTC
  
  GPIO_PORTC_DIR_R |=0xF0;
  GPIO_PORTC_DEN_R |= 0xF0;             //Set PORTC as digital pins
  
}

void porte_init(){
		SYSCTL_RCGCGPIO_R |= 0x10;
		while((SYSCTL_PRGPIO_R & 0x10)==0);
		GPIO_PORTE_DIR_R &= ~0x0F; //set E0-E3 as input pins
		GPIO_PORTE_DIR_R |= 0x30;
		GPIO_PORTE_DEN_R |=0x3F;
		GPIO_PORTE_AMSEL_R &= ~0x3F;
		GPIO_PORTE_AFSEL_R &= ~0x3F;
		GPIO_PORTE_PDR_R |= 0x0F;
}

void PortF_Init(){
			SYSCTL_RCGCGPIO_R |= 0x20; // activate Port F
			while((SYSCTL_PRGPIO_R&0x00000020) == 0){};
			GPIO_PORTF_DIR_R |= 0x0E; // PF4,PF0 in, PF3-1 out
			GPIO_PORTF_DEN_R |= 0x0E; // digital Input on PF1-3
			GPIO_PORTF_AMSEL_R &= ~0x0E;
			GPIO_PORTF_AFSEL_R &= ~0x0E;
}

void buzz(int num){
	if (num) GPIO_PORTB_DATA_R |= 0x08;
	else GPIO_PORTB_DATA_R &= ~0x08;
		
}

void SysTick_Wait(uint32_t delay){
		NVIC_ST_CTRL_R = 0;
		NVIC_ST_RELOAD_R = delay - 1;
		NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R = 0x00000005;
		while((NVIC_ST_CTRL_R & 0x00010000)==0){}
}

void delay_ms(uint32_t delay){
		volatile uint32_t i;
		for(i = 0;i<=delay;i++){
			SysTick_Wait(16000);
		}
}

void delay_us(uint32_t delay){
		volatile uint32_t i;
		for(i = 0;i<delay;i++){
			SysTick_Wait(16);
		}
}

void row_activate(unsigned char row){
		GPIO_PORTC_DATA_R = (1<<(row+4));
}

unsigned char portE_DATA(){
		return (GPIO_PORTE_DATA_R & 0x0F);
}

unsigned char matrix[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

char keypad(void){
		while(1){
			volatile unsigned char row,column;
				for (row=0;row<4;row++){
						row_activate(row);
						delay_us(2);
						for(column=0;column<4;column++){
								if(portE_DATA() & (1<<column)){
										return matrix[row][column];
								}
						}
				}
		}
}

void ports_init(){
		SYSCTL_RCGCGPIO_R |=0x09;
		while((SYSCTL_PRGPIO_R & 0x09) ==0);
		GPIO_PORTA_DEN_R |=0xE0;
		
		GPIO_PORTD_DEN_R |=0x07;
		
		GPIO_PORTA_DIR_R |=0xE0;
		
		GPIO_PORTD_DIR_R |=0x07;
		
		GPIO_PORTA_AMSEL_R &=~0xE0;
		
		GPIO_PORTD_AMSEL_R &=~0x07;
		
		GPIO_PORTA_PCTL_R &=~0xFFF00000;
		
		GPIO_PORTD_PCTL_R &=~0x00000FFF;
		
		GPIO_PORTA_AFSEL_R &=~0xE0;
		
		GPIO_PORTD_AFSEL_R &=~0x07;
		
		
}

void print_data(unsigned char data)
{
	if((data & 0x01)==0x01) {GPIO_PORTA_DATA_R |=0x80;} //check if bit0=1
	else {GPIO_PORTA_DATA_R &=~0x80;}
	
	if ((data & 0x02)==0x02) {GPIO_PORTA_DATA_R |=0x40;} //check if bit1=1
	else {GPIO_PORTA_DATA_R &=~0x40;}
	
	if((data & 0x04)==0x04) {GPIO_PORTA_DATA_R |=0x20;} //check if bit2=1
	else {GPIO_PORTA_DATA_R &=~0x20;}
	
	if((data & 0x08)==0x08) {GPIO_PORTB_DATA_R |=0x10;} //check if bit3=1
	else {GPIO_PORTB_DATA_R &=~0x10;}
	
	if((data & 0x10)==0x10) {GPIO_PORTE_DATA_R |=0x20;} //check if bit4=1
	else {GPIO_PORTE_DATA_R &=~0x20;}
	
	if((data & 0x20)==0x20) {GPIO_PORTE_DATA_R |=0x10;} //check if bit5=1
	else {GPIO_PORTE_DATA_R &=~0x10;}
	
	if((data & 0x40)==0x40) {GPIO_PORTB_DATA_R |=0x02;}		//check if bit6=1
	else {GPIO_PORTB_DATA_R &=~0x02;}
	
	if((data & 0x80)==0x80) {GPIO_PORTB_DATA_R |=0x01;}	//check if bit7=1		
	else {GPIO_PORTB_DATA_R &=~0x01;}
}

void lcd_cmd(unsigned char cmd) //passing command to lcd
{
	print_data(cmd);
	GPIO_PORTD_DATA_R &=~0x02;
	GPIO_PORTD_DATA_R &=~0x01;
	GPIO_PORTD_DATA_R |=0x04;
	delay_us(1000);
	GPIO_PORTD_DATA_R &=~0x04;
}

void lcd_data(unsigned char data) //passing a char to lcd
{
	print_data(data);
	GPIO_PORTD_DATA_R &=~0x02;
	GPIO_PORTD_DATA_R |=0x01;
	GPIO_PORTD_DATA_R |=0x04;
	delay_us(1000);
	GPIO_PORTD_DATA_R &=~0x04;
}

void lcd_string(char *string) //passing a string to lcd 
{
	volatile char i;
	for(i=0; i <strlen(string);i++)
	{
		lcd_data(string[i]);
	}
}  

void lcd_init(void)
{
	lcd_cmd(0x38);  //8-bit mode
	lcd_cmd(0x06);  //Shift the cursor one position right 
	lcd_cmd(0x0C);  //Display ON, cursor OFF
	lcd_cmd(0x01); //clear screen
}

void count_down(uint32_t num){
 volatile int k ;
 char text []="";
 for (k = num ; k>=0 ; k--)
 {
  sprintf (text ,"%d", k);                             
  lcd_cmd(0x01);
  lcd_string (text);
  delay_ms(1000);
  
 }
 delay_ms(2000);
 lcd_cmd(0x01);
 
}
int main()
{
	portb_init();
	porte_init();
	PortF_Init();
	ports_init();
	delay_us(1000);
	lcd_init();
	lcd_cmd(0x80);
	while(1){
		volatile char key = keypad();
		lcd_cmd(0x01);	
				if(key=='A'){
						lcd_string("Popcorn");
						delay_ms(2000);
						count_down(60);
						lcd_cmd(0x01);
				}
				
			}
}

