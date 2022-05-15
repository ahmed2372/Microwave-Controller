#include "tm4c123gh6pm.h"
#include "stdint.h"
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
int main(){



		while(1){

		}



}
