#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/*define led's*/
#define LRESET	PTCD_PTCD2
#define LED0	PTED_PTED0
#define LED1	PTED_PTED1
#define LED2	PTED_PTED2
#define LED3	PTED_PTED3
#define LED4	PTED_PTED4
#define LED5	PTED_PTED5
#define LED6	PTED_PTED6
#define LED7	PTED_PTED7

#define DisableWatchdog() CONFIG1_COPD = 1

unsigned int i,j;
const unsigned char tabla[]={"PERIODO:"};
volatile unsigned int Uperiodo=0;
volatile unsigned int Dperiodo=0;
volatile unsigned int Cperiodo=0;
volatile unsigned int Aux1=0,Aux=0;

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

void main(void) {
	//---------------Configuración del microcontrolador-------------
	//--------------------------------------------------------------
	SOPT1 = 0;//Deshabilita W D
	MCGTRM = 0xAA; // Configura freciencia a 8 MHz
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; // Espera que se enganche
	//----------------------------------------------
	//---------Configuracion IRQ--------------------
	PTCDD=0b00000100;
	//PTFPE=0b00010000;//Habilita resistencias de Pull-Up
	LRESET=1;
	tiempo(5000);
	LRESET=0;
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
	TPM1SC = 0b00000111; //TIM1 Pre*64, Int. deshabilitada, TIM deshabilitado
	TPM1MOD=0xFFFF;      //T=0.52428s
	TPM1C2SC=0b00101000; //Configuracion de canal-Modo Edge alined PWM-Deshabilita interupcion CPWMS = 0
	TPM1C2V=0x00FF; 		 //Configuracion de comparador de canal
	//---------------------------------------------
	//---------Configuracion Timer 2 Contador------
	TPM2SC = 0b00000111; 	//TIM2 Pre*64, Int. deshabilitada, TIM deshabilitado
	TPM2C0SC =0b00000100;	//Configurado Modo Captura
	TPM2MOD=0xFFFF;         //T=1sg
	//--------------------------------------------
	TPM2SC_CLKSA = 1;	 //Habilita Timer 2
	TPM1SC_CLKSA = 1;	 //Habilita Timer 1
	EnableInterrupts;//CLI
	//Fin poner a andar timers
	for(;;) 
	{/*
		while(TPM2C0SC_CH0F==0){}
		TPM2C0SC_CH0F=0;
		TPM2CNTH=0; //REINICIO CONTADOR
		while(TPM2C0SC_CH0F==0){}
		TPM2C0SC_CH0F=0;
		Aux=TPM2C0V/125;
		Periodo(Aux);
	*/	
	}
}

//interrupt 18 void TPM2_ISR(void){
//	TPM2SC_TOF=0;}

//interrupt 15 void TPM1_ISR(void){}

interrupt 2 void IRQ_ISR(void){
	Aux=TPM2CNT/125;
	if(Aux<35){
		if(Aux>25){
		//No haga nada	
		}else{
			Aux1=TPM1C2V-10; 		 //Configuracion de comparador de canal
			TPM1C2V=Aux1;
		}
	}else{
		Aux1=TPM1C2V+10;
		TPM1C2V=Aux1;
	}
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
	TPM2CNT=0x0000;
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
