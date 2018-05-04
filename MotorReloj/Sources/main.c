#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/*define led's*/
#define LRESET	PTCD_PTCD2
#define LESTADO	PTCD_PTCD5

#define DisableWatchdog() CONFIG1_COPD = 1

unsigned int i,j;
const unsigned char tabla[]={"PERIODO:"};
volatile unsigned int Uperiodo=0;
volatile unsigned int Dperiodo=0;
volatile unsigned int Cperiodo=0;
volatile unsigned int Aux1=0,Aux=0,De=0;

//----------RUTINAS LCD-----------------------
//--------------------------------------------

void tiempo(int t){
	for(i=0;i<t;i++);
	return;	
}
void salto(){
	PTDD_PTDD1=1;
	for(i=0;i<20;i++);
	PTDD_PTDD1=0;
	tiempo(50);
	return;
}

void comando(){
	PTDD_PTDD0=0;
	salto();
	return;
}
void cursor(char pos){
	PTED=pos;
	comando();
	return;
}
void dato(){
	PTDD_PTDD0=1;
	salto();		
	return;
}
//------------------------------------------------------------------
void act_lcd(){
	Cperiodo=Aux/100;
	Aux1=Aux-Cperiodo*100;
	Dperiodo=Aux1/10;
	Uperiodo=Aux1%10;
	cursor(0b10001101);
	PTED=Cperiodo+0x30;
	dato();
	cursor(0b10001110);
	PTED=Dperiodo+0x30;
	dato();
	cursor(0b10001111);
	PTED=Uperiodo+0x30;
	dato(); 
	return;
}
void main(void) {
	//---------------Configuraci�n del microcontrolador-------------
	//--------------------------------------------------------------
	SOPT1 = 0;//Deshabilita W D
	MCGTRM = 0xAA; // Configura freciencia a 8 MHz
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; // Espera que se enganche
	//----------------------------------------------
	//---------Configuracion IRQ--------------------
	PTCDD=0b00100100;
	//PTFPE=0b00010000;//Habilita resistencias de Pull-Up
	LRESET=1;
	LESTADO=1;
	tiempo(5000);
	LRESET=0;
	LESTADO=0;
	IRQSC=0b00010110; //Se hablilita por flanco de bajada, Se habilita interrupcion,habilita interrupcion
	//-------------------------------------------------
	//-------Configuracion LCD---------------
	//--------------------------------------------------------
	PTEDD=0xFF;//Configura salidas Para LCD
	tiempo(50000);
	//PTGPE=0XFF;
	PTEDD=0XFF;
	PTDDD=3;
	PTDD_PTDD1=0;
	PTED=0b00111000;
	comando();
	PTED=0b00000110;
	comando();
	PTED=0b00001100;
	comando();
	PTED=0b00000001;
	comando();
	tiempo(20000);
	//--------------------------------------------------------------
	// ------------INICIALIZACION LCD--------------------
		cursor(0b10000000);
		for(j=0;j<sizeof(tabla)-1;j++){
				PTED=tabla[j];
				dato();
			}	
	//---------------------------------------------
	//---------Configuracion Timer 1 PWM-----------
	TPM1SC = 0b00000101; //TIM1 Pre*32, Int. deshabilitada, TIM deshabilitado
	TPM1MOD=0xFFFF;      //T=0.52428s
	TPM1C2SC=0b00101000; //Configuracion de canal-Modo Edge alined PWM-Deshabilita interupcion CPWMS = 0
	TPM1C2V=0x5FFF; 		 //Configuracion de comparador de canal
	//---------------------------------------------
	//---------Configuracion Timer 2 Contador------
	TPM2SC = 0b00000101; 	//TIM2 Pre*32, Int. deshabilitada, TIM deshabilitado
	//TPM2C0SC =0b00000100;	//Configurado Modo Captura
	TPM2MOD=0xFFFF;         //T=0.5sg
	//--------------------------------------------
	TPM2SC_CLKSA = 1;	 //Habilita Timer 2
	TPM1SC_CLKSA = 1;	 //Habilita Timer 1
	EnableInterrupts;//CLI
	//Fin poner a andar timers
	for(;;) 
	{
		
	}
}
/*
interrupt 18 void TPM2_ISR(void){	
	TPM2SC_TOF=0;
}
*/
/*
interrupt 15 void TPM1_ISR(void){
	TPM1SC_TOF=0;
}
*/
interrupt 2 void IRQ_ISR(void){
	if(De==0){
		TPM2CNT=0x0000;
		De=1;
	}else{
		Aux=TPM2CNT/250;
			if(Aux<70){
				if(Aux>60){
				//No haga nada
				LESTADO=1;	
				}else{
					Aux1=TPM1C2V-1000; 		 //Configuracion de comparador de canal
					TPM1C2V=Aux1;
				LESTADO=0;
				}
			}else{
				if(TPM1C2V<0xBFFF){
				Aux1=TPM1C2V+1000;
				TPM1C2V=Aux1;
				LESTADO=0;}
			}
			act_lcd();
			De=0;
	}
	IRQSC_IRQACK=1;
}

/*
 Codigo de rescate
Aux=Aux+1;
cursor(0b10001111);
PTED=Aux%10+0x30;
dato();
cursor(0b10001110);
PTED=Aux/10+0x30;
dato();	
*/