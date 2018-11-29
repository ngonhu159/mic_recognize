const int sampleWindow = 15; // lấy mẫu 50ms (50 mS = 20Hz)
unsigned int sample;

int i_sample=0;
int count_threshold_dapBan=0;
int count_threshold_het=0;

#define count_dapBan 1
#define count_het 4
#define threshold_dapBan 520
#define threshold_het 450
#define time_delay 2000 /* thời gian delay */

void setup() 
{
   Serial.begin(9600);
}

void loop() 
{
   unsigned long startMillis= millis();  
   unsigned int peakToPeak = 0;   
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(A1);
      if (sample < 1024)  
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // lưu lại giá trị max
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // lưu lại giá trị min
         }
      }
   }
   
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   if (i_sample>=10) {
        if (count_threshold_het>=count_het) {
              Serial.println("tieng het");
              delay(time_delay);
        }
        else if (count_threshold_dapBan>=2 && count_threshold_dapBan<=3) {
             startMillis= millis();  
             peakToPeak = 0;   
             signalMax = 0;
             signalMin = 1024;
             while (millis() - startMillis < sampleWindow)
             {
                  sample = analogRead(A1);
                  if (sample < 1024)  
                  {
                       if (sample > signalMax)
                       {
                            signalMax = sample;  // lưu lại giá trị max
                       }
                       else if (sample < signalMin)
                       {
                            signalMin = sample;  // lưu lại giá trị min
                       }
                  }
             }
             peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
             if (peakToPeak>350){
                 Serial.println("tieng het");
                 delay(time_delay);
             } else {
             
                Serial.println("Dap ban");
                delay(time_delay);
             }
        }
        i_sample=0;
        count_threshold_het=0;
        count_threshold_dapBan=0;
        return;
   }
  
   if (peakToPeak >= threshold_het) {
       count_threshold_het++;
       if (peakToPeak >= threshold_dapBan) {
            count_threshold_dapBan++; 
       }
   }
   i_sample++;
}
