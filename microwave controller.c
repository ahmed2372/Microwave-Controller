#include <stdint.h>
#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
/*
PE0 --> column1
PE1 --> column2
PE2 --> column3
PE3 --> column4

PB3 --> Buzzer

PB4 --> row1
PB5 --> row2
PB6 --> row3
PB7 --> row4



btn 1 --> 0x02 red
btn 2 --> 0x04 blue 
btn 3 --> 0x06
btn A --> 0x08 green
btn 4 --> 0x0A
btn 5 --> 0x0C
btn 6 --> 0x0E white
btn B --> 0x0E white with buzzer
btn 7 --> 0x0C with buzzer
btn 8 --> 0x0A with buzzer
btn 9 --> 0x08 green with buzzer
btn C --> 0x06 with buzzer
btn 0 --> 0x04 blue with buzzer
btn D --> 0x02 red with buzzer
*/
#define on 1
#define off 0
void portb_init(){
		SYSCTL_RCGCGPIO_R |= 0x02; // activate Port B
		while((SYSCTL_PRGPIO_R & 0x02)==0);
		GPIO_PORTB_DIR_R |= 0xF8; //set B3-B7 as output pins
		GPIO_PORTB_DEN_R |=0xF8;  //set B3-B7 as digital pins
		GPIO_PORTB_AMSEL_R &= ~0xF8;
		GPIO_PORTB_AFSEL_R &= ~0xF8;
}
void porte_init(){
		SYSCTL_RCGCGPIO_R |= 0x10; // activate Port E
		while((SYSCTL_PRGPIO_R & 0x10)==0);
		GPIO_PORTE_DIR_R &= ~0x0F; //set E0-E3 as input pins
		GPIO_PORTE_DEN_R |=0x0F;   //set E0-E3 as digital pins
		GPIO_PORTE_AMSEL_R &= ~0x0F;
		GPIO_PORTE_AFSEL_R &= ~0x0F;
		GPIO_PORTE_PDR_R |= 0x0F;	//Enable pull up resistors for E0-E3
}
void PortF_Init(){
			SYSCTL_RCGCGPIO_R |= 0x20; // activate Port F
			while((SYSCTL_PRGPIO_R&0x00000020) == 0){};
			GPIO_PORTF_DIR_R |= 0x0E; // PF3-1 out
			GPIO_PORTF_DEN_R |= 0x0E; // digital Input on PF1-3
			GPIO_PORTF_AMSEL_R &= ~0x0E;
			GPIO_PORTF_AFSEL_R &= ~0x0E;
}
void buzz(int num){
	if (num) GPIO_PORTB_DATA_R |= 0x08;
	else GPIO_PORTE_DATA_R &= ~0x08;
		
}
void SysTick_Wait(uint32_t delay){
		NVIC_ST_CTRL_R = 0;
		NVIC_ST_RELOAD_R = delay - 1;
		NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R = 0x00000005;
		while((NVIC_ST_CTRL_R & 0x00010000)==0){}
}
void delay_ms(uint32_t delay){
		uint32_t i;
		for(i = 0;i<=delay;i++){
			SysTick_Wait(80000/5);
		}
}
void delay_us(uint32_t delay){
		uint32_t i;
		for(i = 0;i<delay;i++){
			SysTick_Wait(16);
		}
}
void row_activate(unsigned short row){
		GPIO_PORTB_DATA_R = (1<<(row+4));
}
unsigned short portE_DATA(){
		return (GPIO_PORTE_DATA_R & 0x0F);
}
unsigned char matrix[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

char keypad(void){
		while(1){
			unsigned short row,column;
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
int main(){
		portb_init();
		porte_init();
		PortF_Init();
		while(1){
				if(keypad()=='1'){
						GPIO_PORTF_DATA_R = 0x2;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='2'){
						GPIO_PORTF_DATA_R = 0x4;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='3'){
						GPIO_PORTF_DATA_R = 0x6;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='A'){
						GPIO_PORTF_DATA_R = 0x8;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='4'){
						GPIO_PORTF_DATA_R = 0xA;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='5'){
						GPIO_PORTF_DATA_R = 0xc;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='6'){
						GPIO_PORTF_DATA_R = 0xE;
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
				}
				if(keypad()=='B'){
						GPIO_PORTF_DATA_R = 0xE;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='7'){
						GPIO_PORTF_DATA_R = 0xc;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='8'){
						GPIO_PORTF_DATA_R = 0xA;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='9'){
						GPIO_PORTF_DATA_R = 0x8;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='C'){
						GPIO_PORTF_DATA_R = 0x6;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='0'){
						GPIO_PORTF_DATA_R = 0x4;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
				if(keypad()=='D'){
						GPIO_PORTF_DATA_R = 0x2;
						buzz(on);
						delay_ms(1000);
						GPIO_PORTF_DATA_R = 0x00;
						buzz(off);
				}
			}

}

