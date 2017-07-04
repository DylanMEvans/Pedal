

class BoardTester
{
  int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
  int POT0, POT1, POT2, out_DAC0, out_DAC1; //variables for 3 pots (ADC8, ADC9, ADC10)
  int LED = 3;
  int FOOTSWITCH = 7; 
  int TOGGLE = 2; 

  int cnt = 1;

  public:
  BoardTester()
  {
    //ADC Configuration
    ADC->ADC_MR |= 0x80;   // DAC in free running mode.
    ADC->ADC_CR=2;         // Starts ADC conversion.
    ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0 and 1.  
    
    
    
    //DAC Configuration
    analogWrite(DAC0,0);  // Enables DAC0
    analogWrite(DAC1,0);  // Enables DAC0
  }
  void update()
  {
   // for(int i = 0; i < 2000000; i++);
   //delay(250);
  
    
    //Read the ADCs
    while((ADC->ADC_ISR & 0x1CC0)!=0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
    in_ADC0=ADC->ADC_CDR[7];               // read data from ADC0
    in_ADC1=ADC->ADC_CDR[6];               // read data from ADC1  
  

    //Add volume feature with POT2
//  String str1 = " POT2: ";
//  str1 = str1 + POT2 + "  ";
//  Serial.print(str1);
  
 //  String str = " in_ADC0: ";
 //  str = str + in_ADC0 + "  in_ADC1: " + in_ADC1 + "\n";
 //  Serial.print(str);
    POT0=ADC->ADC_CDR[11];                 // read data from ADC8        
    POT1=ADC->ADC_CDR[11];                 // read data from ADC9   
    POT2=ADC->ADC_CDR[12];                 // read data from ADC10 
       
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

BoardTester *pTester;

void setup()
{
 Serial.begin(250000);
 //Serial.print("setup");
 pTester = new BoardTester();
}
 
void loop()
{
  //Serial.print("loop");
  pTester->update();
}
