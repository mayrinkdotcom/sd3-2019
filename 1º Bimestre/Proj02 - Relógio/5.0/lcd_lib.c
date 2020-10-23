/* Título: bllbioteca de funcões LCD 
Autor: Joel A. Santos
Data: novembro/2014
Ambiente: IAR Embeded Workbench IDE; ver.: 5.5.2.1
última rev: 06/2016última rev: 06/2016
--------------------------------------------------------------------------------
*/

#include  <msp430f1611.h> 


void tempo(volatile unsigned int i)

{
        
 do (i--);
 while (i != 0);
}

void envia_comando()
{
P3OUT &= 0x7F;  // leva P3.7 a zero
tempo(100);     //delay 15 ms
P3OUT |= 0x40;  // P3.6 -> alto
tempo(100);   // delay 15 ms
P3OUT &= 0xBF;  // P3.6 -> baixo
tempo(100);    //delay 15 ms
}

void envia_dado()
{
P3OUT |= 0x80;  // leva P3.7 a um
tempo(100);   //delay 15 ms
P3OUT |= 0xC0; // P3.6 -> alto
tempo(100);   // delay 15 ms
P3OUT &= 0xBF;  // P3.6 -> baixo
tempo(100);   //delay 15 ms
P3OUT &= 0x7F;  // leva P3.7 a zero
tempo(100);   //delay 15 ms
}

void CLR_LCD ()
{
P5OUT = 0x01; // limpa LCD
envia_comando();  
}


void Config_LCD()
   
{
P5DIR |= 0xFF;//seleciona todos bits da porta 5 como saída: sinais D0 a D7 - LCD
P3DIR |= 0xC0; //seleciona P3.6, P3.7 como saída: sinais "E" e "RS" - LCD    
                            
                           
P5OUT = 0x38;  //8bits de dados – display 1 linha – M 7x5
envia_comando();      
P5OUT = 0x0E;  //display e cursor ativos sem piscar
envia_comando();
P5OUT = 0x06; //deslocamento do cursor para a direita
envia_comando();
P5OUT = 0x0C; // apaga o cursor
envia_comando();
CLR_LCD ();                           
}

