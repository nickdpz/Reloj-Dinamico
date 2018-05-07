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

unsigned int i,j,k;
volatile unsigned char u_seg=0,d_seg=0, u_min=0, d_min=2, u_hora=2, d_hora=1,Aux=0,Enable=1;
unsigned char Bandera=0 , Mensaje=0;
//--------------------DECLARACION DE NUMEROS-------------------------
const unsigned char N[10][4]={
		                      {0b00000000,0b11111110,0b10000010,0b11111110},  //0
							  {0b00000000,0b01000010,0b11111110,0b00000010},  //1
							  {0b00000000,0b10011110,0b10010010,0b11110010},  //2
							  {0b00000000,0b11110000,0b00010000,0b11111110},  //3
							  {0b00000000,0b11110010,0b10010010,0b10011110},  //4
							  {0b00000000,0b11111110,0b10010010,0b10011110},  //5
							  {0b00000000,0b10010000,0b10010000,0b11111110},  //6
							  {0b00000000,0b11111110,0b10010010,0b11111110},  //7
							  {0b00000000,0b11110010,0b10010010,0b11111110},  //8
							  {0b00000000,0b11111110,0b10000010,0b11111110}}; //9
const unsigned char M1[12][4]={
		                      {0b01111111, 0b01001001,0b01001001,0b00110110},//B
		                      {0b01111110, 0b00000001,0b00000001,0b01111110},//U
		                      {0b01111111, 0b01001001,0b01001001,0b01000001},//E
		                      {0b01111111, 0b00010000,0b00001000,0b01111111},//N
		                      {0b01111110, 0b10000001,0b10000001,0b01111110},//0
		                      {0b01111001, 0b01001001,0b01001001,0b01001111},//S
		                      {0b00000000, 0b00000000,0b00000000,0b00000000},//-
		                      {0b00000000, 0b00000000,0b00000000,0b00000000},//-
		                      {0b11111111, 0b10000001,0b10000001,0b01111110},//D
		                      {0b01000001, 0b01111111,0b01000001,0b00000000},//I
		                      {0b00111111, 0b01001000,0b01001000,0b00111111},//A
		                      {0b01111001, 0b01001001,0b01001001,0b01001111},};//S

const unsigned char M2[14][4]={
		                      {0b11111111, 0b10010000,0b10010000,0b01100000},//P
		                      {0b11111111, 0b10010001,0b10010001,0b10000001},//E
		                      {0b11111111, 0b10010000,0b10011000,0b01100111},//R
		                      {0b11111111, 0b10000001,0b10000001,0b01111110},//D
		                      {0b01111110, 0b10000001,0b10000001,0b01111110},//0
		                      {0b11111111, 0b00100000,0b00010000,0b11111111},//N
		                      {0b00000000, 0b00000000,0b00000000,0b00000000},//-
		                      {0b00000000, 0b00000000,0b00000000,0b00000000},//-
		                      {0b01111110, 0b10000001,0b10000001,0b10000001},//C
		                      {0b11111111, 0b00011000,0b00011000,0b11111111},//H
		                      {0b01111111, 0b10001000,0b10001000,0b01111111},//A
		                      {0b11111111, 0b10010000,0b10011000,0b01100111},//R
		                      {0b11111111, 0b10010000,0b10011000,0b01100111},//R
		                      {0b11000000, 0b00100000,0b00011111,0b11100000}};//Y
void tiempo(int t){
	for(i=0;i<t;i++);
	return;	
}

void main(void) {
	//---------------Configuración del microcontrolador-------------
	SOPT1 = 0;//Deshabilita W D
	MCGTRM = 0xAA; // Configura freciencia a 8 MHz
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; // Espera que se enganche
	PTEDD=0xFF;
	PTCDD=0xFF;
	Bandera=0;
	LRESET=1;
	tiempo(10000);
	LRESET=0;
	//---------------Configuracion KBI-----------------------------
	PTGDD=0;
	PTGPE=0b00000011;//			 	;HABILITAR RESISTENCIAS DE PULL UP G1-G0
	KBIPE=0b11000011;//HABILITAR INTERRUPCIONES DE PUERTOS 	
	KBISC_KBACK=1;
	KBISC_KBIE=1;
	KBIES=0x0;
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
	  if(Bandera==0){//Para Hora
		  for(j=0;j<=3;j++){
		    PTED=N[d_hora][j];
		  	tiempo(694); 
		   }
		  for(j=0;j<=3;j++){
			  PTED=N[u_hora][j];
			  tiempo(694); 
				  }
		  PTED=0b00100100;
		  tiempo(694);
		  for(j=0;j<=3;j++){
			  PTED=N[d_min][j];
			  tiempo(694); 
				  }
		  for(j=0;j<=3;j++){
			  PTED=N[u_min][j];
			  tiempo(694); 
				  }
			  PTED=0b00100100;
			  tiempo(694);
		  for(j=0;j<=3;j++){
			  PTED=N[d_min][j];
			  tiempo(694); 
				  }
		  for(j=0;j<=3;j++){
			  PTED=N[u_min][j];
			  tiempo(694); 
				  }
		  for(j=0;j<=46;j++){
			  PTED=0b00011000;
			  tiempo(694);
		  }	  
	  }else{//Para mensaje
		  if(Mensaje==1){
			  for(j=0;j<=12;j++){
				  for(k=0;k<=4;k++){
					 PTED=M1[j][k];
					 tiempo(694);
				  } 
			  }
			  for(j=0;j<=24;j++){
				PTED=0b00011000;
				 tiempo(694);
			 }	  
		  }else{
			  for(j=0;j<=14;j++){
			  	for(k=0;k<=4;k++){
			  		PTED=M1[j][k];
			  		 tiempo(694);
			  	 } 
			   }
			   for(j=0;j<=16;j++){
			  	 PTED=0b00011000;
			  	  tiempo(694);
			  }	  
			  
		  }
	  }
	   
  } 
}

interrupt VectorNumber_Vkeyboard void KBI_ISR(void){
	if(Enable==1){
	Aux=(0b00000011)&PTGD;
	if(Aux==0b0000001){
		if(Bandera==1){
			Bandera=0;//CAMBIA MODO
		}else{
			Bandera=1;
		}
	}
	if(Aux==0b0000010){
		if(Bandera==0){
			if(Mensaje==1){
				Mensaje=0;
			}else{
				Mensaje=1;
			}
		}else{
			//Suma de a minuto
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
	Enable=0;
	KBISC_KBACK=1;		
}
interrupt VectorNumber_Vtpm1ovf void TPM1_ISR(void){
	TPM1SC_TOF=0;
}

interrupt 18 void TPM2_ISR(void){
	TPM2SC_TOF=0;
	Enable=1;
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
