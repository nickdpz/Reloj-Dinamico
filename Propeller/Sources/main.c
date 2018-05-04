#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define LRESET	PTCD_PTCD2
#define LED0	PTED_PTED0
#define LED1	PTED_PTED1
#define LED2	PTED_PTED2
#define LED3	PTED_PTED3
#define LED4	PTED_PTED4
#define LED5	PTED_PTED5
#define LED6	PTED_PTED6
#define LED7	PTED_PTED7

unsigned int i,j;
volatile unsigned int u_seg=0,d_seg=0, u_min=0, d_min=2, u_hora=2, d_hora=1;
int Bandera=0;
//--------------------DECLARACION DE NUMEROS-------------------------
const unsigned int N[4][4]={{0b00000000,0b01000010},{0b11111110,0b01000001},{0b00000000,0b01000010},{0b11111110,0b01000001}};
//

void tiempo(int t){
	for(i=0;i<t;i++);
	return;	
}

void main(void) {
	//---------------Configuración del microcontrolador-------------
	//--------------------------------------------------------------
	SOPT1 = 0;//Deshabilita W D
	MCGTRM = 0xAA; // Configura freciencia a 8 MHz
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; // Espera que se enganche
  //---------------Inicio de Variables--------------
	
  //---------Condiguracion Timer 2----------
	TPM2SC = 0b01000111;
	TPM2MOD = 62500;
  //---------Configuracion Timer 1-----------
  	TPM1SC = 0b01000000; //TIM1 Pre*1, Int. habilitada, TIM deshabilitado
  	TPM1MOD=5556;      //T=0.694444*10^(-3) Para un grado
  	EnableInterrupts;
  	TPM2SC_CLKSA = 1;	 //Habilita Timer 2
	TPM1SC_CLKSA = 1;	 //Habilita Timer 1

  for(;;) {
	  if(Bandera==0){
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[d_hora][i]);//Efectivo
			 tiempo(694); 
		  }
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[u_hora][i]);//Efectivo
			  tiempo(694); 
				  }
		  PTED=0b00100100;
		  tiempo(694);
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[d_min][i]);//Efectivo
			  tiempo(694); 
				  }
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[u_min][i]);//Efectivo
			  tiempo(694); 
				  }
			  PTED=0b00100100;
			  tiempo(694);
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[d_min][i]);//Efectivo
			  tiempo(694); 
				  }
		  for(i=0;i==3;i++){
			  PTED=0xFF&(N[u_min][i]);//Efectivo
			  tiempo(694); 
				  }
		  for(i=0;i==48;i++){
			  PTED=0b00011000;
			  tiempo(694);
		  }	  
	  }else{
		  //
	  }
	  
  } /* loop forever */
  /* please make sure that you never leave main */
}

interrupt VectorNumber_Vtpm1ovf void TPM1_ISR(void){
	TPM1SC_TOF=0;
}

interrupt 18 void TPM2_ISR(void){
	TPM2SC_TOF=0;
	u_seg++;
	if(u_seg==10){
		u_seg=0;
		d_seg++;
	if(d_seg == 6){
		u_seg=0;
		d_seg=0;
		u_min++;
		if(u_min == 0x0A){
			u_min=0;
			d_min++;
			if(d_min==6){
				d_min=0;
				u_hora++;
				if(u_hora==0x0A){
					u_hora=0;
					d_hora++;
					
				}else{
					if(u_hora==2){
						if(d_hora==1){
							u_hora=0;
							d_hora=0;
						}
					}
				}
			}
		}
	}
  }
}

