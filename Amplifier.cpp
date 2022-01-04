/* My comments */

#include "Amplifier.h"
#include <Wire.h>
#include <avr/sleep.h>

// Amplifier object
Amplifier::Amplifier() {
}


void Amplifier::init(unsigned int digital_volume, unsigned int analog_gain, unsigned int Select_Fsw, unsigned int I2S_Format)  {
  pinMode(MUTEPIN, INPUT);
  Wire.begin();
  // Reset registers
  writeReg8(TAS5825M_ADDRESS, RESET_CTRL, B00000000);
  //Book
  writeReg8(TAS5825M_ADDRESS, Book, B00000000);
  
  //Digital volume
  byte data = 0;
  if( digital_volume <= 24 ) {
    data = ( 48 - digital_volume / 0.5);
  }
  else {
    data = 48;
  }
  writeReg8(TAS5825M_ADDRESS, DIG_VOL, data); //B00110000 defualt
  
  //Play mode
  writeReg8(TAS5825M_ADDRESS, DEVICE_CTRL_2, B00000011);
  
  //Set analog gain
    if( analog_gain <= 15 ) {
    data = analog_gain * 2;
  }
  else {
    data = 0;
  }
  writeReg8(TAS5825M_ADDRESS, AGAIN, data); //=15 dB, B00011111=15.5 dB 
  
  //Select_Fsw
    if( Select_Fsw <= 70 ) {
    data = Select_Fsw;
  }
  else {
    data = 0;
  }
  writeReg8(TAS5825M_ADDRESS, DEVICE_CTRL_1, data);
  
  // if using Fsw=768kHz otherwise delete the below line
  writeReg8(TAS5825M_ADDRESS, ANA_CTRL, B01100000);
   
  //Set I2S Word Length  
    if( I2S_Format <= 70 ) {
    data = I2S_Format;
  }
  else {
    data = 3;
  }
  writeReg8(TAS5825M_ADDRESS, SAP_CTRL1, data); //B00000000 =16bit, B00000010 =24bit, B00000011 =32bit
  //GPIO 0,1,2 is output
  writeReg8(TAS5825M_ADDRESS, GPIO_CTRL, B00000111);
  //GPIO 0 = Clock invalid flag (clock error or clock missing)
  writeReg8(TAS5825M_ADDRESS, GPIO0_SEL, B00000110);
  //GPIO 1 = Auto Mute
  writeReg8(TAS5825M_ADDRESS, GPIO1_SEL, B00000011);
  //GPIO 2 = Fault Indication
  writeReg8(TAS5825M_ADDRESS, GPIO2_SEL, B00001011);
  delay(5);
    
  //writeReg8(TAS5825M_ADDRESS, PIN_CONTROL1, B00010000);
  //Fault clear 
  writeReg8(TAS5825M_ADDRESS, FAULT_CLEAR, B10000000);

  }

/////////////////////////////////////////////////////////////  
boolean Amplifier::btnPressed() {
  boolean data;
  if ( digitalRead(MUTEPIN) == HIGH ) {
    data = true;
  }
  else {
    data = false;
  }
  return data;
}

void Amplifier::sleepBtnWake() {
  
writeReg8(TAS5825M_ADDRESS, DEVICE_CTRL_2, B00001011);
    interrupts ();
    delay(500);
}
//////////////////////////////////////////////////////////////

int Amplifier::getDIE_ID() {
  byte data = readReg8(TAS5825M_ADDRESS, DIE_ID);
  return data;
}

int Amplifier::getCHAN_FAULT() {
  byte data = readReg8(TAS5825M_ADDRESS, CHAN_FAULT);
  return data;
}

int Amplifier::getGPIO0_SEL() {
  byte data = readReg8(TAS5825M_ADDRESS, GPIO0_SEL);
  return data;
}

int Amplifier::getGLOBAL_FAULT2() {
  byte data = readReg8(TAS5825M_ADDRESS, GLOBAL_FAULT2);
  return data;
}

int Amplifier::getWARNING() {
  byte data = readReg8(TAS5825M_ADDRESS, WARNING);
  return data;
}

int Amplifier::getFS_MON() {
  byte data = readReg8(TAS5825M_ADDRESS, FS_MON);
  return data;
}

int Amplifier::getPVDD_ADC() {
  byte data = readReg8(TAS5825M_ADDRESS, PVDD_ADC);
  return data;
}

int Amplifier::getAUTOMUTE_STATE() {
  byte data = readReg8(TAS5825M_ADDRESS, AUTOMUTE_STATE);
  return data;
}

int Amplifier::getGLOBAL_FAULT1() {
  byte data = readReg8(TAS5825M_ADDRESS, GLOBAL_FAULT1);
  return data;
}

int Amplifier::getPOWER_STATE() {
  byte data = readReg8(TAS5825M_ADDRESS, POWER_STATE);
  return data;
}

int Amplifier::getCLKDET_STATUS() {
  byte data = readReg8(TAS5825M_ADDRESS, CLKDET_STATUS);
  return data;
}

int Amplifier::getRESET_CHANGE_PAGE() {
  byte data = readReg8(TAS5825M_ADDRESS, RESET_CHANGE_PAGE);
  return data;
}

int Amplifier::getBook() {
  byte data = readReg8(TAS5825M_ADDRESS, Book);
  return data;
}

int Amplifier::getAGAIN() {
  byte data = readReg8(TAS5825M_ADDRESS, AGAIN);
  return data;
}

int Amplifier::getFAULT_CLEAR() {
  byte data = readReg8(TAS5825M_ADDRESS, FAULT_CLEAR);
  return data;
}


int Amplifier::getDIG_VOL() {
  byte data = readReg8(TAS5825M_ADDRESS, DIG_VOL);
  return data;
}

int Amplifier::getRESET_CTRL() {
  byte data = readReg8(TAS5825M_ADDRESS, RESET_CTRL);
  return data;
}

int Amplifier::getDEVICE_CTRL_1() {
  byte data = readReg8(TAS5825M_ADDRESS, DEVICE_CTRL_1);
  return data;
}


int Amplifier::getDEVICE_CTRL_2() {
  byte data = readReg8(TAS5825M_ADDRESS, DEVICE_CTRL_2);
  return data;
}

int Amplifier::getSIG_CH_CTRL() {
  byte data = readReg8(TAS5825M_ADDRESS, SIG_CH_CTRL);
  return data;
}

int Amplifier::getSAP_CTRL1() {
  byte data = readReg8(TAS5825M_ADDRESS, SAP_CTRL1);
  return data;
}

int Amplifier::getBCK() {
  byte data = readReg8(TAS5825M_ADDRESS, BCK);
  return data;
}
//////////////////////////////////////////////////////////////



byte Amplifier::readReg8(int deviceAddress, int regAddress) {
  byte data = 0;
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, 1);
  data = Wire.read();
  return(data);
}

void Amplifier::writeReg8(int deviceAddress, int regAddress, byte data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

void Amplifier::writeReg16(int deviceAddress, int regAddress, word data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(highByte(data));
  Wire.write(lowByte(data));
  Wire.endTransmission();
}

void wake()
{
  sleep_disable();  
  detachInterrupt (1);  
}
