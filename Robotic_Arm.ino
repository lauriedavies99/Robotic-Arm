#include <Servo.h> 
#include <DynamixelShield.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL soft_serial
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
  #define DEBUG_SERIAL SerialUSB    
#else
  #define DEBUG_SERIAL Serial
#endif

const uint8_t DXL_WRIST = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

DynamixelShield dxl;

using namespace ControlTableItem;
 
Servo Base;     
Servo Arm1; 
Servo Arm2;            
int Basecur = 90;
int Arm1cur = 90;
int Arm2cur = 90;
int Wristcur = 90;
 
void setup() 
{ 
  DEBUG_SERIAL.begin(115200);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.ping(DXL_WRIST);
  dxl.torqueOff(DXL_WRIST);
  dxl.setOperatingMode(DXL_WRIST, OP_POSITION);
  dxl.torqueOn(DXL_WRIST);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_WRIST, 75);
  
  //Reset upon start
  Base.write(90);
  Arm1.write(90);
  Arm2.write(90);

  delay(2000);
  
  Base.attach(7);
  Arm1.attach(8);
  Arm2.attach(9);

  delay(2000);
  Move(120,140,30,0);
  delay(4000);
  Move(90,90,90,512);
} 
 
void loop() 
{ 

} 

void Move(int Basenew, int Arm1new, int Arm2new, int Wristnew){
  if (Basenew > Basecur){
    for (; Basecur <= Basenew; Basecur += 1){
      Base.write(Basecur);
      delay(30);
    }
  }
  else if (Basenew < Basecur){
    for (; Basecur >= Basenew; Basecur -= 1){
      Base.write(Basecur);
      delay(30);
    }
  }
   if (Arm1new > Arm1cur){
    for (; Arm1cur <= Arm1new; Arm1cur += 1){
      Arm1.write(Arm1cur);
      delay(30);
    }
  }
  else if (Arm1new < Arm1cur){
    for (; Arm1cur >= Arm1new; Arm1cur -= 1){
      Arm1.write(Arm1cur);
      delay(30);
    }
  }
  if (Arm2new > Arm2cur){
    for (; Arm2cur <= Arm2new; Arm2cur += 1){
      Arm2.write(Arm2cur);
      delay(30);
    }
  }
  else if (Arm2new < Arm2cur){
    for (; Arm2cur >= Arm2new; Arm2cur -= 1){
      Arm2.write(Arm2cur);
      delay(30);
    }
  }
  dxl.setGoalPosition(DXL_WRIST,Wristnew);
}
  
