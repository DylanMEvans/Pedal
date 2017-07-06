
  int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
  int POT0, POT1, POT2, out_DAC0, out_DAC1; //variables for 3 pots (ADC8, ADC9, ADC10)
  int LED = 3;
  int FOOTSWITCH = 7; 
  int TOGGLE = 2; 

class BoardTester
{

  int cnt = 1;

  public:
  BoardTester()
  {
    Serial.print("BoardTester constructed.\n");
  }
  void update()
  {
   // for(int i = 0; i < 2000000; i++);
   //delay(250);
  
//  String str1 = " POT2: ";
//  str1 = str1 + POT2 + "  ";
//  Serial.print(str1);
  
 //  String str = " in_ADC0: ";
 //  str = str + in_ADC0 + "  in_ADC1: " + in_ADC1 + "\n";
 //  Serial.print(str);
       
  // if(cnt++ ==5) {cnt=1; str1="\n"; Serial.print(str1);}
    
    out_DAC0=map(in_ADC0,0,4095,1,POT2);
    out_DAC1=map(in_ADC1,0,4095,1,POT2);

    //out_DAC0 = in_ADC0;
    //out_DAC1 = in_ADC1;
     
    //Write the DACs
    dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
    dacc_set_channel_selection(DACC_INTERFACE, 1);       //select DAC channel 1
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);//write on DAC
    
  }
};

BoardTester *pCurrentTask;
void startTimer()
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);
  TC_Configure(TC1,1,TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1,1,238); //44.1-kHz interrupt rate
  TC_Start(TC1,1);
  TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(TC4_IRQn);

  
}

void setup()
{
 Serial.begin(250000);
 delay(250);
 
  //ADC Configuration
  ADC->ADC_MR |= 0x80;   // DAC in free running mode.
  ADC->ADC_CR=2;         // Starts ADC conversion.
  ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0 and 1.  
  
  
  
  //DAC Configuration
  analogWrite(DAC0,0);  // Enables DAC0
  analogWrite(DAC1,0);  // Enables DAC0
  
  //Serial.print("setup");
  pCurrentTask = new BoardTester();
  //pCurrentTask = new Reverb();
  startTimer();
}
int cnt = 0;
void loop()
{
  String str32 = " loop";
  Serial.print(str32 + cnt);
   if(cnt++ == 10) {cnt=1; str32="\n"; Serial.print(str32);}

}

void TC4_Handler()
{
  //Serial.print("T");
  TC_GetStatus(TC1,1);//Reset timer interrupt
  
  //Read the ADCs
  while((ADC->ADC_ISR & 0x1CC0)!=0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
  in_ADC0=ADC->ADC_CDR[7];               // read data from ADC0
  in_ADC1=ADC->ADC_CDR[6];               // read data from ADC1  
  POT0=ADC->ADC_CDR[11];                 // read data from ADC8        
  POT1=ADC->ADC_CDR[11];                 // read data from ADC9   
  POT2=ADC->ADC_CDR[12];                 // read data from ADC10 
  
  pCurrentTask->update();
}


