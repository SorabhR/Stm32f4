#include<Wire.h>

int sends[3] = {1,5,6};
int i = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x08);
}

void loop() {
  // put your main code here, to run repeatedly:
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void receiveEvent(){
  int SlaveReceived = Wire.read();
  Serial.println(SlaveReceived);
}

void requestEvent(){
  Wire.write(sends[0]);
  Wire.write(sends[1]);
  Wire.write(sends[2]);  
//  i++;
//  if (i>1)
//    i = 0;
}
