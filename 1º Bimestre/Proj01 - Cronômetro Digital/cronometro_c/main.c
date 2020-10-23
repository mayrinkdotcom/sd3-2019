/* Título: cronômetro 0 a 999seg
Autor: Joel A. Santos
Data: novembro/2014
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
última rev: 06/2016
--------------------------------------------------------------------------------
*/


#include  <msp430f1611.h>   
//------------------ declaração de protótipos de funções ----------------- 

void Config_LCD();
void Config_KB();
void CLR_LCD();
void envia_dado();
void wr_lcd ();
void tempo();
//----------------------Variáveis do sistema ----------------------------

int cron_vetor[3]= {0,0,0}; // armazena contagem de segundos de 000 a 999
int short botao=0; //  seta flag do botão no modo "parado"
int estado=0; //seta estado "desativado"

//------------------------funções do sistema ----------------------------
void Config_KB (void)

{
  
  P1DIR |=  0x00; // define parte baixa da porta 1 como entrada
  P1OUT=0x00;
  P1IE  |= 0x01; // habilita interrupção do botão 1
  P1IES |=0x01;  //  seleciona borda de descida 
  P1IFG &= ~0x00; // limpa os flags de interrupção dos botões
}

void cron_up_date (void)
{
 
  cron_vetor[2]++;
  if (cron_vetor[2]>9)
  {
    cron_vetor[2]=0;
    cron_vetor[1]++;
  }
  if (cron_vetor[1]>9)
  {
    cron_vetor[1]=0;
    cron_vetor[0]++;
  }
  
  if(cron_vetor[0]>9)
  {
    cron_vetor[0]=0;
  }
}

void clr_cron( )
{
  
CLR_LCD (); 
int i=0; //define tamanho máximo da mensagem
while (i!=4)
{
  cron_vetor[i]=0;
  i++;
}
}

void wr_lcd (int mens[3])

{
CLR_LCD (); 
int i=0; //define tamanho máximo da mensagem
while (i!=3)
{
  P5OUT = mens[i]+0x30; //envia caracter ao LCD
  envia_dado();
  i++;
}
}

int main( void )
{
  
WDTCTL = WDTPW + WDTHOLD;      // Para o watchdog timet
Config_KB ();
Config_LCD();                 //configura lcd
CCTL0 = CCIE;                 // CCR0 interrupt enabled   
CCR0 = 32767;   
TACTL = TASSEL_1 + MC_1;     // ACLK, upmode  
_BIS_SR(GIE);                //  habilita interrupção
  
while (1) 
{
wr_lcd (cron_vetor);  
}

}

// Rotina de serviço de interrupção do Timer A0 

#pragma vector=TIMERA0_VECTOR   
__interrupt void Timer_A (void)   
{   
 if (botao== 1)
 {
  cron_up_date ();

 }
 
 _BIS_SR(GIE);
} 

// Rotina de serviço de interrupção da Porta 1 

#pragma vector=PORT1_VECTOR   
__interrupt void PORT_1 (void)   
{   
 estado++;
 
 switch (estado)
  {
  
case 1:   //inicia contagem
  {
  botao=1;
 // estado=2;  // muda estado para "contando"
  break;
  }
  case 2:   //// muda estado para "parar cronômetro"
  {
  botao=0;
  wr_lcd (cron_vetor);
  break;
  }
  case 3:   // muda estado para "parado" e limpa cronômetro
    {
   clr_cron();
   botao=0;
   estado=0;
   break;
    }
  }
  P1IFG &= ~0x0F; // limpa o flag de interrupção do botão
 _BIS_SR(GIE);             //  habilita interrupção
 
}
