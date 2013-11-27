/*
 Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients but the client the message comes from.
 To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 redesigned to make use of operator== 25 Nov 2013
 by Norbert Truchsess
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,6);
IPAddress gateway(192,168,0, 1);
IPAddress subnet(255, 255, 255, 0);


// telnet defaults to port 23
EthernetServer server(23);

EthernetClient clients[4];

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  if (client) {

    boolean newClient = true;
    for (byte i=0;i<4;i++) {
      if (clients[i]==client) {
        newClient = false;
        break;
      }
    }

    if (newClient) {
      for (byte i=0;i<4;i++) {
        if (clients[i]!=client) {
          clients[i] = client;
          break;
        }
      }

      // clead out the input buffer:
      client.flush();    
      Serial.println("We have a new client");
      client.println("Hello, client!");
      client.print("your IP: ");
      client.println(client.remoteIP());
      client.print("your port: ");
      client.println(client.remotePort()); 
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      for (byte i=0;i<4;i++) {
        if (!clients[i] || (clients[i]==client)) {
          continue;
        }
        clients[i].write(thisChar);
      }
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }
  for (byte i=0;i<4;i++) {
    if (!(clients[i].connected())) {
      clients[i].stop();
      clients[i]=EthernetClient();
    }
  }
}
