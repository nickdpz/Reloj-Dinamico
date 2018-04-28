#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/*define led's*/
#define LED0	PTED_PTED0
#define LED1	PTED_PTED1
#define LED2	PTED_PTED2
#define LED3	PTED_PTED3
#define LED4	PTED_PTED4
#define LED5	PTED_PTED5
#define LED6	PTED_PTED6
#define LED7	PTED_PTED7

#define DisableWatchdog() CONFIG1_COPD = 1

unsigned int i[2];
unsigned int segundos=0;
unsigned int uMin=0;
unsigned int dMin=0;
unsigned int uHora=0;
unsigned int dHora=0;


int rotarServo(int degrees)
{
	if(degrees >= 0 && degrees <= 180)
	{
		return (degrees*26)+200;	// Daza
	}
	else
	{
		return 700;
	}
}

void main(void) 
{
	//Configuración del microcontrolador
	SOPT1 = 0;//Deshabilita W D
	MCGTRM = 0xAA; // Configura freciencia a 8 MHz
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; // Espera que se enganche
	//Fin configuración del microcontrolador
	IRQSC=0b00010110; //Habilita interupcion externa 
	PTEDD=0xFF;//Configura salidas Para LEDs en puerto E
	PTGPE=0x0F;// Habilita resistencias de Pull-up para entradas
	KBISC_KBACK=1;//Configuracion KBI
	KBISC_KBIE=1;//Habilita KBI
	TPM1SC = 0b01110100; //TIM1 Pre*16, Int. habilitada, TIM deshabilitado
	TPM1MOD=62500;            //T=1sg
	TPM1SC_CLKSA = 1;//Habilita Timer
	EnableInterrupts;//CLI
	//Fin poner a andar timers
	for(;;) 
	{//CICLO PRINCIPAL
		LED0=0;//Ejemplo para poner bit unicos en 1 o 0
		LED1=1;
	}
}

interrupt 2 void IRQ_ISR(void){
	//CALCULA EL PERIODO
}


interrupt 26 void KBI_ISR(void){
	//PARA LOS BOTONES
	
}

interrupt 15 void TIM1_ISR(void){
	TPM1SC_TOF=0;//Borra bandera
  segundos++;
  if (segundos>59){
    segundos=0;
    uMin++;
    if (uMin>9){
      uMin=0;
      dMin++;
      if (dMin>5){
        dMin=0;
        uHora++;
        if (uHora>9){
          uHora=0;
          dHora++;
        }else{
          if (uHora==4){
            if (dHora==2){
              uHora=0;
              dHora=0;
            }
          }
        }
      }
    }
  }    
}

