#include "spi2452.h"
#include <msp430.h>

char flagIE = 0;
char RxData;
//Inicia o SPI
void SpiInit()
{

  P1DIR |= CS | SPI1 | SPI2;
  P1DIR &= ~SPI3;
  //// setup SPI
  

  USICTL0   |= USISWRST;                     // put USI in reset mode, source USI clock from SMCLK
  USICTL0  |= USIPE5 | USIPE6 | USIPE7 | USIMST | USIOE | USIGE;
  USICKCTL  |= USIDIV_4 | USISSEL_2;         // default speed 16MHz/1
  USICTL1    |= USICKPH;                      // SPI_MODE_0, CPHA=0
  USICKCTL  &= ~USICKPL;       // CPOL=0
  USICTL0  &= ~USILSB;       // MSBFIRST
  USICTL0 &= ~USISWRST; // release for operation


  HabSpi();
}

//Habilita o SPI
void HabSpi(void)
{ 
  USICTL0 &= ~USISWRST; // release for operation
}

//Função de transmissão de dados                                                                                                                                                                                                 
void SpiWrite(unsigned char Data)
{
  USISRL = Data;
  USICNT |= 8;
  while (!(USICTL1&USIIFG));
  __delay_cycles(5);
  
}

//Função de leitura de dados
char SpiRead(unsigned char teste)
{
  USISRL = teste;
  USICNT = 8;
  while((USICTL1 & USIIFG) == 0);
  USISRL = 0xFF;
  USICNT = 8;
  while((USICTL1 & USIIFG) == 0);
  RxData =  USISRL;
  return (char) RxData;
}

//Função de desactivação do módulo USCI                                                                                                                                                                                          
void DesSpi(void)
{
  USICTL0 |= USISWRST;
}


//Transforma o valor da posição do RW em valor (aproximado) em ohms
int PosToOhm(char pos)
{
  return (int) 256 - (pos * PASSO + RW);
}

//Transforma o valor em ohm na Posição do RW e envia
void EscOhm(int ohms)
{
  int ohms_novo = 0;
  ohms_novo = ohms - 75;
  int teste = 0;
  teste = (int) ( ohms_novo / PASSO ) ;
  unsigned char valor_escrita = (unsigned char) (256 - (0x00FF & teste) );
  EscRwPos(valor_escrita);  
}

//Envia o valor da posição do RW
void EscRwPos(unsigned char valor)
{
  P1OUT &= ~CS;  

  SpiWrite(TCON1);
  SpiWrite(TCON2);
  SpiWrite(0x00); 
  SpiWrite(valor);
   __delay_cycles(50);
  
  P1OUT |= CS;
}

//Ler o valor da posição do RW
char RwPosRead()
{
  P1OUT &= ~CS;    
  char teste = SpiRead(READ);
  P1OUT |= CS;
  return teste;
}

//Incrementa o valor da resistência em 1 posição (o -1 é por causa do valor de RAW)
void incrementar(void)
{
   EscRwPos( RwPosRead() - 1);
}

//Decrementa o valor da resistência em 1 posição (o +1 é por causa do valor de RAW)
void decrementar(void)
{
   EscRwPos( RwPosRead() + 1);
}
