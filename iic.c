#include <msp430.h> 
unsigned char I2CBUF;
unsigned char checkbit;
#define SCL_H   (P10OUT |= BIT2)      // P10.2为SCL
#define SCL_L   (P10OUT &= ~BIT2)
#define SDA_H   (P10OUT |= BIT1)     //p10.1为SDA
#define SDA_L   (P10OUT &= ~BIT1)
#define SDA_OUT  (P10DIR |= BIT1)
#define SDA_IN  (P10DIR &= ~ BIT1)
void I2C_Start(void)    //I2C开始信号
{
 SDA_OUT;
 SCL_L;
 SDA_H;
 __delay_cycles(5);
 SCL_H;
 __delay_cycles(5);
 SDA_L;
 __delay_cycles(5);
 SCL_L;
 __delay_cycles(5);
}
unsigned char testACK(void) {
 SDA_IN;
 __delay_cycles(5);
 SCL_H;
 __delay_cycles(5);
 checkbit = (P10IN & BIT1) >> 4;
 __delay_cycles(5);
 SCL_L;
 __delay_cycles(5);
 return (checkbit);
}
void I2C_ACK(void)                //应答信号
{
 SDA_OUT;
 SCL_L;
 SDA_L;
 __delay_cycles(5);
 SCL_H;
 __delay_cycles(5);
 SCL_L;
 __delay_cycles(5);
 SDA_H;
}
void I2C_NACK(void) {
 SDA_OUT;
 SDA_H;
 __delay_cycles(5);
 SCL_H;
 __delay_cycles(5);
 SCL_L;
 __delay_cycles(5);
}
void I2C_STOP(void)             //产生结束信号
{
 SCL_L;
 __delay_cycles(5);
 SDA_L;
 __delay_cycles(5);
 SCL_H;
 __delay_cycles(5);
 SDA_H;
 __delay_cycles(5);
}
void I2C_WRITE(unsigned char data)
{
 SDA_OUT;
 unsigned char i;
 for (i = 8; i > 0; i--)      //高位循环8次，发送1个字节数据
    {
  if (data & 0x80)   //判断数据高位是0还是1
     {
   SDA_H;           //高位数据写1
  }
  else
  {
   SDA_L;
  }
  SCL_H;
  __delay_cycles(5);  //发送数据
  SCL_L;
  __delay_cycles(5);
  data <<= 1;

 }
}
unsigned char I2C_READ(void)
{
 unsigned char j;
 unsigned char BUFFER = 0X00;
 SDA_IN;
 for (j = 8; j >0; j--)      //高位循环8次，发送1个字节数据
    {
  SCL_H;
  __delay_cycles(5);
  BUFFER <<= 1;
  if ((P10IN & BIT1) == BIT1)   //判断数据高位是0还是1
  {
   BUFFER |= 0X01;         //高位数据写1
  }
  else
  {
   BUFFER |= 0XFE;
  }
  __delay_cycles(5);  //发送数据
  SCL_L;
  __delay_cycles(5);
 }
 return BUFFER;
}
void C16_WRITEDATA(unsigned char address, unsigned char DATA)
{
 I2C_Start();
 I2C_WRITE(address);
 testACK();
 I2C_ACK();
 I2C_WRITE(0x00);
 testACK();
 I2C_ACK();
 I2C_WRITE(DATA);
 I2C_STOP();
}
unsigned char C16_READEDATA(unsigned char address)
{
 unsigned char READDATA;
 I2C_Start();
 I2C_WRITE(0XA1);
 testACK();
 I2C_ACK();
 I2C_WRITE(address);
 I2C_Start();
 testACK();
 I2C_ACK();
 I2C_WRITE(0XA1);
 READDATA = I2C_READ();
 I2C_STOP();
 return READDATA;
}
int main(void)
{
    const int DATA = 0X88;
 unsigned char READBUFFER;
 WDTCTL = WDTPW + WDTHOLD;
 P10DIR |= BIT1 + BIT2;
 P2DIR |= BIT0;
 P2OUT &= ~BIT0;
 C16_WRITEDATA(0X00, DATA);
 __delay_cycles(5);
 READBUFFER = C16_READEDATA(0X00);
 while (1)
 {
  if (READBUFFER == 0X88)
  {
   P2OUT |= BIT0;
  } else {
   P2OUT &= ~ BIT0;
  }
  __delay_cycles(15);
 }
}
