/*
   MCP42100 Dual 100K Digital Pot Demo
   Based on examples at https://wiki.dfrobot.com/Dual_Digital_Pot__100K__SKU__DFR0520

   pot 1 controls the op amp gain
   pot 0 controls the offset (wiper should be near mid point for full swing)

   Arduino Uno SPI connections:

   Uno            MCP42100
   -------------------------
   D2            CS  (pin 1)
   D11 (MOSI)    SI  (pin 3)
   D13 (SCK)     SCK (pin 2)

   Gadget Reboot
*/

#include <SPI.h>

const byte csPin           = 2;       // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 100000;  // 100k pot resistance between terminals A and B
const byte rWiper          = 52;      // 52 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr
const byte pot1            = 0x12;    // pot1 addr
const byte potBoth         = 0x13;    // pot0 and pot1 simultaneous addr
const byte pot0Shutdown    = 0x21;    // pot0 shutdown
const byte pot1Shutdown    = 0x22;    // pot1 shutdown
const byte potBothShutdown = 0x23;    // pot0 and pot1 simultaneous shutdown

void setup() {

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();

}

void loop() {

  setPotWiper(pot1, 128);                  // op amp gain approx. -1, where  (-Rb/Ra) = (-50k/50k)
  setPotWiper(pot0, 128);                  // op amp bias approx. 2.5v (voltage divider with 50K and 50K on 5V)
  delay(5000);

  // change op amp gain by moving the wiper and changing Rb and Ra
  for (int i = 129; i < 200; i++) {
    setPotWiper(pot1, i);
    delay(500);
  }

  for (int i = 199; i > 50; i--) {
    setPotWiper(pot1, i);
    delay(500);
  }

  // reset gain to -1
  setPotWiper(pot1, 128);

  // change op amp bias/offset by moving the wiper and changing Rb and Ra
  for (int i = 129; i < 200; i++) {
    setPotWiper(pot0, i);
    delay(500);
  }

  for (int i = 199; i > 50; i--) {
    setPotWiper(pot0, i);
    delay(500);
  }

}

void setPotWiper(int addr, int pos) {

  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255

  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance wiper to B terminal: ");
  Serial.print(resistanceWB);
  Serial.println(" ohms");

}
