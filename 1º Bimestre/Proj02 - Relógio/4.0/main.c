
#include  <msp430f1611.h>
//------------------ declara��o de prot�tipos de fun��es ----------------- 
void Config_KB();
void Config_LCD();
char read_KB();
void envia_comando();
void envia_dado();
void converte_tecla();
void tempo();


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  
  Config_LCD();
  Config_KB();
  _BIS_SR(GIE);   //Habilita interrup��o geral.

  for(;;)
  {
  }
}

#pragma vector = PORT2_VECTOR		// In�cio da fun��o de interrup��o
__interrupt void PORT2 (void)
{
 
 tempo(50);
 unsigned char digito = read_KB();
 P5OUT = 0x87;
 envia_comando();
 P5OUT = digito;
 envia_dado();
 P2IFG = 0x00;                                   // Limpa os flags de interrup��o
 P3OUT|=0x0F;                                  // Coloca P3.0 a P3.3 em n�vel alto (1)
 tempo(50);
 _BIS_SR(GIE);   //Habilita interrup��o geral.
}