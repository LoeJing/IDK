  ADC10CTL0 |= SREF_1+REF2_5V+REFON+ADC10IE;       //将AD10基准设置为2.5V 开启AD允许中断
  ADC10CTL0 |= ADC10SHT_0+MSC;                    //打开AD转换，过采样率设置为4个采样周期
  ADC10CTL1 |= ADC10SSEL_3+SHS_0;                 //选择250K的采样时钟，用ADC10SC触发采集
  ADC10CTL1 |= CONSEQ_2+INCH_0;                  //采样模式，通道0
  ADC10CTL0 |= ADC10ON;
  ADC10AE0  |= 0xFF;
  ADC10CTL0 |= ADC10SC+ENC;
  _EINT();//允许中断
