#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
//#include <Udp.h>

int UD = 0;
int LR = 0;

const String GROUP = "0x4E";
const int CEILING = 600;
const int FLOOR = 300;
int brightness = 0x01;
int color = 0x00; // Violet

// Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte remoteIp [] = {255,255,255,255};
unsigned int localPort = 5600;

IPAddress ip(10, 0, 1, 5);
EthernetUDP Udp;

void setup() {
  Serial.begin(9600);
  
  // Start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
}

void loop() {
  UD = analogRead(A0);
  LR = analogRead(A1);
  
  if (UD > CEILING) {
    brighten();
  }
  
  if (UD < FLOOR) {
    darken();
  }

//  Serial.print (UD);
//  Serial.print (" ");
//  Serial.print (LR);
//  Serial.println ("");

  
  if (LR > 800) {
    Serial.println ("color");
    changeColorUp ();
  }

  if (LR < 100) {
    changeColorDown ();
  }

  Serial.println (color);
  delay (200);
}

void brighten () {
    if (brightness == 1) {
      Serial.println("Turn on");
      turnOn ();
    }
  
    if (brightness >= 1 && brightness < 27) {
      turnOn();
      brightness += 2;
      sendPacket(brightness);
    }
}

void darken () {
    if (brightness <= 1) {
      turnOff ();
    }
    
    if (brightness >= 2 && brightness <= 27) {
      turnOn();
      brightness -= 2;
      sendPacket(brightness);
    }
}

void changeColorUp () {
  color += 10;
  sendPacket (0x40, color);
}

void changeColorDown () {
  color -= 10;
  sendPacket (0x40, color);
}

void turnOn () {
  sendPacket (0x42, 0x00);
}

void turnOff () {
  sendPacket (0x41, 0x00);
}

void sendPacket (int midValue) {
  byte packet [] = { 0x4E, midValue, 0x55 };
  Udp.beginPacket(remoteIp, 8899);
  Udp.write(packet,3);
  Udp.endPacket();
}

void sendPacket (int firstValue, int midValue) {
  byte packet [] = { firstValue, midValue, 0x55 };
  Udp.beginPacket(remoteIp, 8899);
  Udp.write(packet,3);
  Udp.endPacket();
}

