
/**
 * The Arduino side of the NeoStrip library/system.
 * Communicates with a host computer (eg the FRC
 * RoboRio) over a socket (5810, but this can be
 * changed). It also updates the LED strip itself,
 * a NeoPixel/ws2812b LED strip.
 * 
 * Written by Caleb Shilling, FRC Team 957
 */

// IMPORTS
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

// Length of the LED strip. Adjust this to fit the number on the Rio.
int stripLength = 60;

// LED STRIP ON PORT 6
Adafruit_NeoPixel strip =
  Adafruit_NeoPixel(stripLength, 6, NEO_GRB + NEO_KHZ800);

// CLIENT OBJECT AND IP/MAC ADDRESSES
EthernetClient robotClient;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
/**
 * Note to potential readers:
 * If you are not a part of FRC Team 957, you need
 * to change these default IP addresses. Both of
 * these addresses need to follow standard FRC
 * 10.TE.AM.X configurations. Make sure the Rio
 * has a default IP address. If not part of an
 * FRC team, set it to whatever suits your
 * purposes.
 */
IPAddress ip(10,9,57,3);
IPAddress robotIp(10,9,57,2);

// Byte array to hold incoming bytes
char bytes[360] = {53};
int colors[360] = {53};
// Integer value used to restart the client if need be
int connectionTimer = 0;

void setup() {
  // Begins interfacing with the ethernet shield, USB UART, and the LED strip,
  // while attempting to address the RoboRio
  Ethernet.begin(mac,ip);
  Serial.begin(9600); 
  strip.begin();                                    
  robotClient.connect(robotIp, 5810);

  // Disables the SD card slot on Ethernet Shields (Slave Select Pin 4)
  pinMode(4, OUTPUT);
  digitalWrite(4,HIGH);
}

void loop() {

  // If bytes are in the Ethernet buffer, find how many are present, divide it by
  // 3, and push it to the Length integer. This is the length of the LED strip.
  // Then, read all of the bytes using said variable, and flush any new ones
  // afterwards that have arrived.
  if(robotClient.available()){
    connectionTimer = 0;
    robotClient.readBytes(bytes, stripLength*3);
    robotClient.flush();

    Serial.println("Recived bytes");
  }

  // Writes the bytes recived to the LED strip. The bytes are in ASCII format, so subtracting by
  // 48 gives the decimal value, as values 0-9 are 48, 49, 50, etc.
  // Then the LED strip is updated.
  for(int i = 0; i < stripLength; i++){
    strip.setPixelColor(i,((int)bytes[i*3] - 48)*31,((int)bytes[(i*3)+1] - 48)*31,((int)bytes[(i*3)+2] - 48)*31);
  }
  strip.show();

  // Have the Arduino wait for 50 milliseconds for new bytes to be recived.
  // Increase the connection reset counter by 1.
  delay(50);
  connectionTimer++;

  // If the connectionTimer variable is 11 or above (equalling ~500ms), print an alert to console.
  // Then, close the socket with the RoboRio and restart it.
  // Finally, have the Arduino wait for 1 second to give the Rio a chance to connect if it is ready to.
  if(connectionTimer > 20){
    Serial.println("Disconnected from server. Attempting to reaquire");
    robotClient.stop();
    robotClient.connect(robotIp,5810);
    delay(1000);
  }
}



