int encoder_pin = 17; // pulse output from the module
unsigned int rpm; // rpm reading
volatile byte pulses; // number of pulses
unsigned long timeold;
// number of pulses per revolution
// based on your encoder disc
unsigned int pulsesperturn = 12;
void counter()
{
   //Update count
   pulses++;
}
void setup()
{
   Serial.begin(9600);
   pinMode(encoder_pin, INPUT);
   //Interrupt 0 is digital pin 2
   //Triggers on Falling Edge (change from HIGH to LOW)
   attachInterrupt(0, counter, FALLING);
   // Initialize
   pulses = 0;
   rpm = 0;
   timeold = 0;
}
void loop()
{
   if (millis() - timeold >= 1000) {
      //Don't process interrupts during calculations
      detachInterrupt(0);
      rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses;
      timeold = millis();
      pulses = 0;
      Serial.print("RPM = ");
      Serial.println(rpm,DEC);
      if(rpm>35){
        Serial.println("OVERSPEEDING OVERSPEEDING");
      }
      //Restart the interrupt processing
      attachInterrupt(0, counter, FALLING);
   }
}
