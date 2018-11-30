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

/* define chan của Mạch Ghi Phát Âm Thanh ISD1820 (20s) */
#define Playe_het 3   
#define Playl_het 4
#define Rec_het   6

#define Playe_dapban 7
#define Playl_dapban 8
#define Rec_dapban   9

void Signaling_het();   /*----- hàm báo ra khi het ------*/
void Signaling_dapban();   /*----- hàm báo ra khi dap bàn -----*/
void Sound_recording_het()  /*----- ghi âm giọng hét ------*/
void Sound_recording_dapban() /*----- ghi âm giọng đập bàn ------*/

void setup() 
{
   Serial.begin(9600);
   pinMode(Playe_het,OUTPUT);
   pinMode(Playl_het,OUTPUT);
   pinMode(Rec_het,OUTPUT);
   
   pinMode(Playe_dapban,OUTPUT);
   pinMode(Playl_dapban,OUTPUT);
   pinMode(Rec_dapban,OUTPUT);
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
              Signaling_het();             /*---- xuat doan ghi am -----*/
            //  delay(time_delay);
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
                 Signaling_het();             /*---- xuat doan ghi am -----*/
                // delay(time_delay);
             } else {
             
                Serial.println("Dap ban");
                Signaling_dapban();             /*---- xuat doan ghi am -----*/
              //  delay(time_delay);
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

   /* ----- kiem tra nut nhan ghi am ------*/
   if (digitalRead(Rec_dapban)==HIGH)
        Sound_recording_dapban();
   if (digitalRead(Rec_het)==HIGH)
        Sound_recording_het();
}

void Signaling_het()
{
    digitalWrite(Playe_het,HIGH);
    delay(time_delay);
    digitalWrite(Playe_het,LOW);
}
void Signaling_dapban()
{
    digitalWrite(Playe_dapban,HIGH);
    delay(time_delay);
    digitalWrite(Playe_dapban,LOW);
}
void Sound_recording_het()
{
    digitalWrite(Rec_het,HIGH);
    while(digitalRead(Rec_het)==HIGH);
    digitalWrite(Rec_het,LOW);
}
void Sound_recording_dapban()
{
    digitalWrite(Rec_dapban,HIGH);
    while(digitalRead(Rec_dapban)==HIGH);
    digitalWrite(Rec_dapban,LOW);
}
