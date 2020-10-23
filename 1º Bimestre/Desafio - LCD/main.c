/*------------------------------------------------------------------------------
                              DISPLAY LCD
Autores: João Pedro Mayrink, Jade Piquerotti, Guilherme Torres
Data: 21 de fevereiro de 2019
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
------------------------------------------------------------------------------*/

#include  <msp430f1611.h>

// declaração de protótipos
void Config_LCD();
void mostra_display1();
void mostra_display2();
void envia_dado();
void envia_comando();

// variáveis do sistema
int linha1[8] = {'c','e','f','e','t','-','m','g'};
int linha2[16] = {'j','o','a','o','-','j','a','d','e','-','t','o','r','r','e','s'};

// funções do sistema
void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Config_LCD();                                                                 // configura o display
  mostra_display1();                                                            // mostra a primeira linha no display
  mostra_display2();                                                            // mostra a segunda linha no display
}

void mostra_display1( void )
{
  P5OUT = 0x84;                                                                 // define posição inicial de escrita da primeira linha
  envia_comando();                                                              // envia posição inicial para o display
  unsigned int indice;
  for(indice = 0; indice <= 7 ; indice ++)
  {
  P5OUT = linha1[indice];                                                       // envia a primeira linha para o display
  envia_dado();
  }
}

void mostra_display2 ( void )
{
  P5OUT = 0xC0;                                                                 // define posição inicial de escrita da segunda linha
  envia_comando();                                                              // envia posição inicial para o display
  unsigned int indice;
  for(indice = 0; indice <= 15 ; indice ++)
  {
  P5OUT = linha2[indice];                                                       // envia a segunda linha para o display
  envia_dado();
  }
}