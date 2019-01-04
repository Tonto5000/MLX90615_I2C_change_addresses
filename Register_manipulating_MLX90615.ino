// by Tonto5000



//to set a new I2C address of the MLX90615, the following steps have to be done:
//
// I made a little circuit with a push button on D5 to have stops in the program. You need those to plug out the MLX and plug it back in(You can also just have a swith on the Vcc of the MLX, instead of pluggin it out....)
// this code makes 12 addresses (0x01 - 0x0C) if you want more, adapt the address and PEC array and change i in the loop
//1. connect the MLX to the µC (if you use a NodeMCU take D1 SDA , D2 SDL)
//2. Read the current MLX address from Register 0x2E (factory default should be 0x5B)
//3. Set the address to 0x00
//4. Important : Turn the MLX on and off to make the change happen!
//5. Do step 2. again and check if the address is 0x00 now
//6. Set the address to the desired one
//7. Important : Turn the MLX on and off to make the change happen!
//8. Do step 2. again and check if the address is 0xXX now
//

#include <Wire.h>

byte address[12]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C}; // 0x10 0x0{1-C} 0x00 in http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
byte PEC[12]={0xB7,0x88,0x9D,0xF6,0xE3,0xDC,0xC9,0xA,0x1F,0x20,0x35,0x5E };

//-------------------------------------------------------------------------------
//To write the address of the MLX to 0x00:

void set_Address_to_zero() {
  Wire.beginTransmission(0x00);
  Wire.write(0x10);   //pointing Register-28
  //desired Address for the MLX - first byte (the important one)
  Serial.println(Wire.write(0x00));
  //leave it free(just let it as 0x00)
  Serial.println(Wire.write(0x00));
  //PEC, has to be calculated with the above mentioned calculator (as follows: CRC input Data: 0x10 0x00 0x00)
  Serial.println(Wire.write(0xA2));
  Wire.endTransmission();

  //Turn the Sensor on and off!
}

//--------------------------------------------------------------------------------
//To write the address of the MLX to 0x05:

void set_Address(byte address, byte PEC) {
  Wire.beginTransmission(0x00);
  Wire.write(0x10);   //pointing Register-28
  //desired Address for the MLX - first byte (the important one)
  Serial.println(Wire.write(address));
  Serial.println(Wire.write(0x00));
  //PEC, has to be calculated with the above mentioned calculator (as follows: CRC input Data: 0x2E 0x05 0x00)
  Serial.println(Wire.write(PEC));
  Wire.endTransmission();

  //Turn the Sensor on and off!

}

void I2Cscan(){
   byte error, address;
      int nDevices;
     
      Serial.println("Scanning...");
     
      nDevices = 0;
      for(address = 1; address < 127; address++ )
      {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
     
        if (error == 0)
        {
          Serial.print("I2C device found at address 0x");
          if (address<16)
            Serial.print("0");
          Serial.print(address,HEX);
          Serial.println("  !");
     
          nDevices++;
        }
        else if (error==4)
        {
          Serial.print("Unknown error at address 0x");
          if (address<16)
            Serial.print("0");
          Serial.println(address,HEX);
        }    
      }
      if (nDevices == 0)
        Serial.println("No I2C devices found\n");
      else
        Serial.println("done\n");
     
      delay(1000);           
}



void setup()
{

  pinMode(D5, INPUT);
  Wire.begin(D1, D2);// D1 SDA , D2 SDL
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nRegister manipulating");
  delay(200);
}



void loop() {
  bool modus=0;
for(int i=0;i<12;i++){ //if you have more addresses change i to X
   Serial.printf("\nPlug MLX %d in", i+1);
   
   modus= digitalRead(D5);
   Serial.printf("\nPush button to go on");
   while(modus==0){
     modus= digitalRead(D5);
     delay(100);
   }
   I2Cscan();
   //2. set MLX to zero:
   Serial.printf("\nSetting address to zero\n");
   set_Address_to_zero();
   Serial.printf("\nPull MLX out and plug it back in");
   modus= digitalRead(D5);
   Serial.printf("\nPush button to go on");
   while(modus==0){
     modus= digitalRead(D5);
     delay(100);
   }   
   Serial.printf("\nMLX can't be found now\n");
   I2Cscan();
 
   modus= digitalRead(D5);
   Serial.printf("\nPush button to go on");
   while(modus==0){
     modus= digitalRead(D5);
     delay(100);
   }

   
  //3. set MLX to desired address:
  Serial.printf("\nSetting address to %x\n",address[i]);
  set_Address(address[i],PEC[i]);
  Serial.printf("\nPull MLX out and plug it back in");
  delay(1000);
  modus= digitalRead(D5);
   Serial.printf("\nPush button to go on");
   while(modus==0){
     modus= digitalRead(D5);
     delay(100);
   }
   delay(1000);
   modus= digitalRead(D5);
   Serial.printf("\npush button to see the new address:\n");
   while(modus==0){
     modus= digitalRead(D5);
     delay(100);
   }
   I2Cscan();
   Serial.printf("\nMLX can be pluged out now\n");

}
}
