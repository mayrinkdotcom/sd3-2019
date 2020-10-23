
#include  <msp430f1611.h>   
//------------------ declaração de protótipos de funções ----------------- 

void Config_LCD();
void CLR_LCD();
void envia_dado();
void wr_lcd ();
void tempo();
void contador();
void envia_comando();

unsigned int numero = 0x30;

int main( void )
{
  
WDTCTL = WDTPW + WDTHOLD;      // Para o watchdog timet
Config_LCD();                 //configura lcd
CCTL0 = CCIE;                 // CCR0 interrupt enabled   
CCR0 = 32767;   
TACTL = TASSEL_1 + MC_1;     // ACLK, upmode  
_BIS_SR(GIE);                //  habilita interrupção
  
while (1) 
{
  
}

}

// Rotina de serviço de interrupção do Timer A0 

#pragma vector=TIMERA0_VECTOR   
__interrupt void Timer_A (void)   
{   
  contador();
 
 _BIS_SR(GIE);
} 

void contador( void )
{
  CLR_LCD();
  P5OUT = 0x87;
  envia_comando();
  P5OUT = 0x30;
  envia_dado();
  P5OUT = 0x88;
  envia_comando();
  P5OUT = numero;
  envia_dado();
  if(numero==0x39)
  {
    numero = 0x30;
  }
  else
  {
    numero++;
  }
}