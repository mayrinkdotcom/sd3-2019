
#include  <msp430f149.h>   
//------------------ declaração de protótipos de funções ----------------- 

void Config_LCD();
void Config_KB();
void CLR_LCD();
void envia_dado();
void wr_lcd ();
void tempo();
void contador();
void envia_comando();
void display();
void comp_alarme();
void testa_alarme();
unsigned char read_KB();
void envia_pergunta();
void zera_vetor();
void display_alarme();

//----------------------Variáveis do sistema ----------------------------
//                          0    0    :    0    0    :    0    0
unsigned char horario[8]={0x30,0x30,0x3A,0x30,0x30,0x3A,0x30,0x30};
unsigned char espaco = 0x84;
//                         0    0    :    0    0    :    0    A                    Alarme fictício, feito somente para prevenir disparos indesejados na inicialização
unsigned char alarme[8]={0x30,0x30,0x3A,0x30,0x30,0x3A,0x30,0x41};
unsigned char p_linha1[12]={"Relogio -> 1"};
unsigned char p_linha2[12]={"Alarme  -> 2"};
unsigned char led = 0;
unsigned char alarme_1 = 0;
unsigned char digito = 0;
unsigned char estado = 0;


void main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;      // Para o watchdog timer
  Config_LCD();                 //configura lcd
  Config_KB();                  // configura teclado
  CCTL0 = CCIE;                 // CCR0 interrupt enabled   
  CCR0 = 32767;   
  TACTL = TASSEL_1 + MC_1;     // ACLK, upmode  
  P4DIR |= 0x01;                // habilita P4.0 como saída
  P4OUT &= 0xFE;                // leva P4.0 a nível baixo
  CLR_LCD();
  _BIS_SR(GIE);                //  habilita interrupção geral
  
  while (1) 
  {
    
  }
  
}

// Rotina de serviço de interrupção do Teclado

#pragma vector = PORT2_VECTOR		// Início da função de interrupção
__interrupt void PORT2 (void)
{
  envia_pergunta();
  tempo(10000);                                    // Espera o usuário soltar a tecla pressionada
  while((P2IN&0x07)==0x00)
  {
    tempo(4000);                                   // Aguarda o usuário escolher a alteração desejada
  }
  
  estado = read_KB();                            // Executa a varredura do teclado
  
  switch(estado)
  {
  case 0x31:                                      // Solicitado alteração de horário
    zera_vetor(horario);                          // Zera o vetor que se deseja alterar (HORARIO)
    CLR_LCD();                                    // Limpa o display
    display();                                    // Exibe o vetor HORARIO
    P5OUT = 0x84;
    envia_comando();
    P3OUT|=0x0F;
    for(unsigned char i = 0; i<=7; i++)
    {
      switch(i)
      {
      case 2:
        horario[i] = 0x3A;                        // Coloca o :
        break;
        
      case 5:
        horario[i] = 0x3A;                        // Coloca o :
        break;
        
      default:
        while((P2IN&0x07)==0x00)
        {
          tempo(1000);
        }
        horario[i] = read_KB();
        display();
        break;
      }
    }
    break;
    
  case 0x32:                                      // Solicitado alteração de horário
    zera_vetor(alarme);                          // Zera o vetor que se deseja alterar (ALARME)
    CLR_LCD();                                    // Limpa o display
    display_alarme();                             // Exibe o vetor ALARME
    P5OUT = 0x84;
    envia_comando();
    P3OUT|=0x0F;
    for(unsigned char i = 0; i<=7; i++)
    {
      switch(i)
      {
      case 2:
        alarme[i] = 0x3A;                        // Coloca o :
        break;
        
      case 5:
        alarme[i] = 0x3A;                        // Coloca o :
        break;
        
      default:
        while((P2IN&0x07)==0x00)
        {
          tempo(1000);
        }
        alarme[i] = read_KB();
        display_alarme();
        break;
      }
    }
    break;
    }
  CLR_LCD();
  P2IFG = 0x00;                                   // Limpa os flags de interrupção
  P3OUT|=0x0F;                                    // Coloca P3.0 a P3.3 em nível alto (1)
  tempo(50);                                      // Aguarda a tensão em P3.0 a P3.3 se estabilizar
  
  _BIS_SR(GIE);                                   //Habilita interrupção geral.
}


// Rotina de serviço de interrupção do Timer A0 

#pragma vector=TIMERA0_VECTOR   
__interrupt void Timer_A (void)   
{   
  
  contador();
  comp_alarme();
  testa_alarme();
  display();
  
  _BIS_SR(GIE);
} 




void contador( void )
{
  
  horario[7]++;                                   // incrementa o segundo
  if (horario[7]==0x3A)                           // testa se chegou a 10
  {
    horario[7] = 0x30;
    horario[6]++;
  }
  if (horario[6]==0x36)
  {
    horario[4]++;
    horario[6] = 0x30;
  }
  if (horario[4]==0x3A)
  {
    horario[3]++;
    horario[4] = 0x30;
  }
  if (horario[3]==0x36)
  {
    horario[1]++;
    horario[3] = 0x30;
  }
  if (horario[1]==0x34)
  {
    if(horario[0]==0x32)
    {
      zera_vetor(horario);
    }
    else
    {
      horario[0]++;
      horario[1] = 0x30;
    }
  }
}

void display( void )
{
  P5OUT = 0x84;
  envia_comando();
  unsigned char indice = 0;
  while(indice<=7)
  {
    P5OUT = horario[indice];
    envia_dado();
    indice++;
  }
}

void comp_alarme( void )
{
  for(unsigned char i = 0; i<=7; i++)
  {
    if(alarme[i]==horario[i])
    {
      led++;
    }
  }
  
  if(led==8)
  {
    alarme_1 = 6;
  }
  led = 0;
}

void testa_alarme( void )
{
  if(alarme_1)
  {
    P4OUT = ~P4OUT;
    alarme_1--;
  }
}

void envia_pergunta( void )
{
  CLR_LCD();
  espaco = 0x82;
  for(unsigned char i1 = 0; i1<=11; i1++)
  {
    P5OUT = espaco;
    envia_comando();
    P5OUT = p_linha1[i1];
    envia_dado();
    espaco++;
  }
  espaco = 0xC2;
  for(unsigned char i2 = 0; i2<=11; i2++)
  {
    P5OUT = espaco;
    envia_comando();
    P5OUT = p_linha2[i2];
    envia_dado();
    espaco++;
  }
}

void zera_vetor( unsigned char *nome )
{
  for(unsigned char i = 0; i<=7; i++)
  {
    switch(i)
    {
    case 2:
      *nome = 0x3A;                        // Coloca o :
      nome++;
      break;
      
    case 5:
      *nome = 0x3A;                        // Coloca o :
      nome++;
      break;
      
    default:
      *nome = 0x30;                        // Zera a posição i
      nome++;
      break;
    }
  }
}

void display_alarme( void )
{
  P5OUT = 0x84;
  envia_comando();
  unsigned char indice = 0;
  while(indice<=7)
  {
    P5OUT = alarme[indice];
    envia_dado();
    indice++;
  }
}