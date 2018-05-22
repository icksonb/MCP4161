#include "spi.h"
#include <msp430.h>

//Inicia o SPI
void SpiInit()
{
  P1DIR |= CS + SPI1 + SPI2;
  P1SEL |= SPI1 + SPI2 + SPI3;
  P1SEL2 |= SPI1 + SPI2 + SPI3;
   
  UCB0CTL1 |= UCSWRST;
  UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCMODE0 + UCSYNC;
  UCB0CTL1 |= UCSSEL_2; //Clock Source SMCLK = DCO/8 (1MHz/8) = 125KHz                                                                                                                           
  UCB0BR0 |= 0x02;      // /2                                                                                                                                                                    
  UCB0BR1 = 0;                                                                                                                                                                                 
  
  HabSpi();
}

//Habilita o SPI
void HabSpi(void)
{ 
  UCB0CTL1 &= ~UCSWRST;
  IE2 &= ~UCB0TXIE;                               // Disable USCI0 RX interrupt*/                                                                                                                                          
}

//Função de transmissão de dados                                                                                                                                                                                                 
void SpiWrite(unsigned char Data)
{
  UCB0TXBUF = Data;                               // Transmit first character                                                                                                                                              
  while (!(IFG2 & UCB0TXIFG));                    // USCI_B0 TX buffer ready?                                                                                                                                              
  IFG2 &= ~UCB0TXIFG;                             //Clear the TX flag                                                                                                                                                      
  __delay_cycles(5);                              //Add some delay between transmissions                                                                                                                                   
}

//Função de leitura de dados
char SpiRead(void)
{
  while (!(IFG2 & UCB0RXIFG));                    // USCI_B0 RX buffer ready?                                                                                                                                              
  IFG2 &= ~UCB0RXIFG;                             //Clear the RX flag                                                                                                                                                      
  __delay_cycles(5);                              //Add some delay between transmissions          
  return UCB0RXBUF;
}

//Função de desactivação do módulo USCI                                                                                                                                                                                          
void DesSpi(void)
{
  UCB0CTL1 |= UCSWRST;
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
  char valor_escrita = (char) (256 - (0x00FF & teste) );
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
  SpiWrite(READ);
  char retorno = SpiRead();
   
  SpiWrite(0xFF);
  char retornoF = SpiRead();
  P1OUT |= CS;
   
  return (char) ((0x01FF & (retorno<<8 | retornoF )));
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