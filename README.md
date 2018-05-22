# MCP4161
Biblioteca do MCP4161 para MSP430

O padrão utilizado foi pela resistência RAW, por isso em alguns momentos preciso retirar 256 para ajuste da resistência.

As funções implementadas foram:
- Escrita da posição RW;
- Leitura da posição RW;
- Transformação da posição do RW em Ohms;
- Transformação de Ohms para posição do RW;
- Incremento e decremento do RW.

No arquivo main, foi utilizado a seguinte estrutura:
- Posicionamento em 50% (5k);
- Set de 3k ohm;
- Incremento de 1k ohm;
- Incremento de 1 posição de RW;
- Decremento de 1 posição de RW;
- Incremento de 10 posições de RW.
