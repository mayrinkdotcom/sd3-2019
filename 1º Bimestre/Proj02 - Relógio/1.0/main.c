
#include  <msp430f1611.h>   
//------------------ declaração de protótipos de funções ----------------- 

void Config_LCD();
void CLR_LCD();
void envia_dado();
void wr_lcd ();
void tempo();
void contador();
void envia_comando();
void doce_de_leite();

unsigned int seg0 = 0x30;
unsigned int seg1 = 0x30;

unsigned int min0 = 0x30;
unsigned int min1 = 0x30;

unsigned int hor0 = 0x30;
unsigned int hor1 = 0x30;

unsigned int ponto = 0x3A;


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
  doce_de_leite();  
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
  
  if(seg0==0x39)
  {
    seg0 = 0x30;
    seg1++;
  }
  else
  {
    seg0++;
  }
  
  if(seg1==0x36)
  {
    seg1 = 0x30;
    min0++;
  }
  
  if(min0==0x39)
  {
    min0 = 0x30;
    min1++;
  }
  
  if(min1==0x35)
  {
    min1 = 0x30;
    hor0++;
  }
  
  if(hor0==0x33)
  {
    if(hor1==0x32)
    {
      hor0 = 0x30;
      hor1 = 0x30;
    }
    else
    {
    }
  }
      
}

void doce_de_leite( void )
{
  P5OUT = 0x84;
  envia_comando();
  P5OUT = hor1;
  envia_dado();
  
  P5OUT = 0x85;
  envia_comando();
  P5OUT = hor0;
  envia_dado();
  
  P5OUT = 0x86;
  envia_comando();
  P5OUT = ponto;
  envia_dado();
  
  P5OUT = 0x87;
  envia_comando();
  P5OUT = min1;
  envia_dado();
  
  P5OUT = 0x88;
  envia_comando();
  P5OUT = min0;
  envia_dado();
  
  P5OUT = 0x89;
  envia_comando();
  P5OUT = ponto;
  envia_dado();
  
  P5OUT = 0x8A;
  envia_comando();
  P5OUT = seg1;
  envia_dado();
  
  P5OUT = 0x8B;
  envia_comando();
  P5OUT = seg0;
  envia_dado();
}