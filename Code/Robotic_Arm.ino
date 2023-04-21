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
const uint8_t DXL_EF = 2;
const float DXL_PROTOCOL_VERSION = 2.0;

DynamixelShield dxl;

using namespace ControlTableItem;

int Open = 30;
int Closed = 0;
 
Servo Base;     
Servo Arm1; 
Servo Arm2;            
int Basecur = 90;
int Arm1cur = 110;
int Arm2cur = 40;
int Wristcur = 0;
int EFcur = Closed; //0 = Closed, 30 = Open
int O[] = {90,110,40,0};

void setup() 
{ 
  DEBUG_SERIAL.begin(115200);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.ping(DXL_WRIST);
  dxl.ping(DXL_EF);
  dxl.torqueOff(DXL_WRIST);
  dxl.torqueOff(DXL_EF);
  dxl.setOperatingMode(DXL_WRIST, OP_POSITION);
  dxl.setOperatingMode(DXL_EF, OP_POSITION);
  dxl.torqueOn(DXL_WRIST);
  dxl.torqueOn(DXL_EF);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_WRIST, 75);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_EF, 75);
  
  //Reset upon start
  Base.write(Basecur);
  Arm1.write(Arm1cur);
  Arm2.write(Arm2cur);
  dxl.setGoalPosition(DXL_WRIST, Wristcur);
  dxl.setGoalPosition(DXL_EF, EFcur, UNIT_DEGREE);
  
  delay(2000);
  
  Base.attach(7);
  Arm1.attach(8);
  Arm2.attach(9);
}
 
void loop() { 
  Switch_Grip();
  MoveDown(60,130,35,90);
  Switch_Grip();
  MoveUp(60,110,60,90);
  MoveUp(120,110,60,90);
  MoveDown(120,130,35,90);
  Switch_Grip();
  MoveUp(O[0],O[1],O[2],O[3]);
  MoveDown(120,130,35,90);
  Switch_Grip();
  MoveUp(120,110,60,90);
  MoveUp(60,110,60,90);
  MoveDown(60,130,35,90);
  Switch_Grip();
  MoveUp(O[0],O[1],O[2],O[3]);
  Switch_Grip();
} 

void MoveUp(int Basenew, int Arm1new, int Arm2new, int Wristnew){
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
  if (Wristnew != Wristcur) {
    dxl.setGoalPosition(DXL_WRIST, Wristnew, UNIT_DEGREE);
    Wristcur = Wristnew;
    delay(30);
  }
  delay(2000);
}

void MoveDown(int Basenew, int Arm1new, int Arm2new, int Wristnew){
  if (Wristnew != Wristcur) {
    dxl.setGoalPosition(DXL_WRIST, Wristnew, UNIT_DEGREE);
    Wristcur = Wristnew;
    delay(30);
  }
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
  delay(2000);
}

void Switch_Grip () {
  if (EFcur == Closed) {
    dxl.setGoalPosition(DXL_EF, Open, UNIT_DEGREE);
    EFcur = Open;
    }
  else {
    dxl.setGoalPosition(DXL_EF, Closed, UNIT_DEGREE);
    EFcur = Closed;
    }
  delay(1000);
}
