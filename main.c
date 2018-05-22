#include <msp430g2553.h>
#include "spi.h"


int main (void)
 {
    WDTCTL = WDTPW | WDTHOLD;
    
    SpiInit();
    
    EscRwPos(1);
    
    //Fica na metade
    __delay_cycles(100000);
    EscRwPos(128);
    
    //Escreve um valor de 3000 ohms
    __delay_cycles(100000);
    EscOhm(3000);
    
    //Aumenta o valor para 4000 ohms
    __delay_cycles(100000);
    EscOhm( PosToOhm(RwPosRead()) + 1000);
    
    //Incrementa 1 posição do RW
    __delay_cycles(100000);
    incrementar();
    
    //Decrementa 1 posição do RW
    __delay_cycles(100000);
    decrementar();
    
    //Incrementa 10 posição do RW
    __delay_cycles(100000);
    EscRwPos(RwPosRead() - 10);
    
    while(1);
    
 }