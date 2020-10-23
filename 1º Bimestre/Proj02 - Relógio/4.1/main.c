
#include  <msp430f1611.h>
//------------------ declaração de protótipos de funções ----------------- 
void Config_KB();
void Config_LCD();
char read_KB();
void envia_dado();
void converte_tecla();


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  Config_KB();
  Config_LCD();
  for(;;)
  {
    
  }
}

#pragma vector = PORT2_VECTOR		// Início da função de interrupção
__interrupt void PORT2 (void)
{

 unsigned char digito = read_KB();
 P5OUT = digito;
 envia_dado();
 
}