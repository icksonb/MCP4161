
//Verificar Datasheet
#define RAB 10000
#define RW 75
#define RAW_MAX RAB-RW
#define RESOLUCAO 256
#define ESCALA 100
#define PASSO ((RAW_MAX - RW) / RESOLUCAO)

#define CS BIT3
#define SPI1 BIT5
#define SPI2 BIT6
#define SPI3 BIT7

#define TCON1 0x40 
#define TCON2 0x0F 

#define READ 0b00001100
#define INC  0b00000100
#define DEC  0b00001000

char SpiRead();
float step_increment();
void EscRwPos(unsigned char valor);
void SpiWrite(unsigned char Data);
void SpiInit();
void HabSpi(void);
void DesSpi(void);
void EscOhm(int ohms);
int PosToOhm(char pos);
void incrementar(void);
void decrementar(void);
char SpiRead(void);
char RwPosRead();
