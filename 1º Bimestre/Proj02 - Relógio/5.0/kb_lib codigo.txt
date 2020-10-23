/* Título: biblioteca de funcões KeyBoard
Autor: João Pedro Mayrink
Data: abril/2019
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
Última revisão: 04/2019
--------------------------------------------------------------------------------
*/

#include  <msp430f1611.h>
//----------------------Variáveis do sistema ----------------------------
//                         1    2    3    4    5    6    7    8    9    A    0    E
unsigned char keycode[]={0x41,0x21,0x11,0x42,0x22,0x12,0x44,0x24,0x14,0x48,0x28,0x18};
unsigned char ascii[] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x30,0x45};
unsigned char tecla = 0;


//------------------ declaração de protótipos de funções -----------------
void tempo();



void Config_KB( void )
{
  P2DIR&=0xF8;                                  // Configura P2.0 a P2.2 como entradas - COLUNAS
  P2IES&=0xF8;                                  // Define como borda de subida
  P2IFG = 0x00;                                 // limpa flags de interrupção de P2
  
  P3DIR|=0x0F;                                  // Configura P3.0 a P3.3 como saídas
  P3OUT|=0x0F;                                  // Coloca P3.0 a P3.3 em nível alto (1)
  
  P2IE|=0x07;                                   // Habilita interrupção para esses bits
}

unsigned char read_KB( void )
{
  tempo(100);                                    // Tratamento de repique da tecla pressionada
  unsigned char linha = 0x01;
  unsigned char coluna = 0x00;
  
  while( coluna==0 )
  {
    P3OUT = linha;
    tempo(50);
    coluna = P2IN&0x07;                        // Atribui o valor de P2.0 - P2.2 para COLUNA
    if(coluna==0)
    {
      linha = linha << 1;
    }
    else
    {
      coluna = coluna << 4;
      tecla = (coluna + linha);
    }
  }
  
  unsigned char i = 0;
  while(keycode[i] != tecla)
  {
    i++;
  }
  tecla = ascii[i];
  P3OUT|=0x0F;
  tempo(4000);
  return tecla;
}