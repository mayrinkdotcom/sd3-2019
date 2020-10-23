
#include  <msp430f1611.h>   
//------------------ declaração de protótipos de funções ----------------- 

void Config_LCD();
void CLR_LCD();
void envia_dado();
void wr_lcd ();
void tempo();
void contador();
void envia_comando();
void display();
void comp_alarme();
void testa_alarme();

//                      0    0    :    0    0    :    0    0
unsigned int horario[8]={0x30,0x30,0x3A,0x30,0x30,0x3A,0x30,0x30};
unsigned int espaco = 0x84;
//                      0    0    :    0    0    :    2    0
unsigned int alarme[8]={0x30,0x30,0x3A,0x30,0x30,0x3A,0x32,0x30};
unsigned char led = 0;
unsigned char alarme_1 = 0;


int main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;      // Para o watchdog timet
  Config_LCD();                 //configura lcd
  CCTL0 = CCIE;                 // CCR0 interrupt enabled   
  CCR0 = 32767;   
  TACTL = TASSEL_1 + MC_1;     // ACLK, upmode  
  _BIS_SR(GIE);                //  habilita interrupção
  P1DIR = 0xFF;                // habilita P1 como saída
  P1OUT = 0x00;                // leva P1 a nível baixo
  
  while (1) 
  {
    P5OUT = 0x84;
    envia_comando();
    display();
  }
  
}

// Rotina de serviço de interrupção do Timer A0 

#pragma vector=TIMERA0_VECTOR   
__interrupt void Timer_A (void)   
{   
  
  contador();
  comp_alarme();
  testa_alarme();
  
  _BIS_SR(GIE);
} 

void contador( void )
{
  
  horario[7]++;
  if (horario[7]==0x3A)
  {
    horario[7] = 0x30;
    horario[6]++;
  }
  if (horario[6]==0x36)
  {
    horario[4]++;
    horario[6] = 0x30;
  }
}

void display( void )
{
  unsigned int indice = 0;
  while(indice<=7)
  {
    P5OUT = horario[indice];
    envia_dado();
    indice++;
  }
}
void comp_alarme( void )
{
  for(unsigned char i = 8; i>0; i--)
  {
    i--;
    if(alarme[i]==horario[i])
    {
      led++;
    }
    i++;
    
    if(led==8)
    {
      alarme_1 = 6;
    }
  }
}

void testa_alarme( void )
{
  if(alarme_1)
  {
    P1OUT = ~P1OUT;
    alarme_1--;
  }
}