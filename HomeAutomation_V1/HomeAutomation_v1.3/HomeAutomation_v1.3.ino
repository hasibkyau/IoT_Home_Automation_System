// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "JHSTC";
const char* password = "henrycollage@123";

// Set web server port numbelanguager to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String LED1state = "off";
String LED2state = "off";
String LED3state = "off";
String LED4state = "off"; 
String FANstate = "off";


// Assign output variables to GPIO pins
const int LED1 = 4;
const int LED2 = 15;
const int LED3 = 26;
const int LED4 = 27;
const int FAN = 18;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(FAN, OUTPUT);

  // Set outputs to LOW
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(FAN, LOW);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            //light r1
            if (header.indexOf("GET /4/on") >= 0) {
              Serial.println(header.indexOf("GET /4/on"));
              Serial.println("GPIO 4 on");
              LED1state = "on";
              digitalWrite(LED1, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              LED1state = "off";
              digitalWrite(LED1, LOW);
            } 
            //light r2
            else if (header.indexOf("GET /15/on") >= 0) {
              Serial.println("GPIO 15 on");
              LED2state = "on";
              digitalWrite(LED2, HIGH);
            } else if (header.indexOf("GET /15/off") >= 0) {
              Serial.println("GPIO 15 off");
              LED2state = "off";
              digitalWrite(LED2, LOW);
            }
            
            //light r3
            else if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              LED3state = "on";
              digitalWrite(LED3, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              LED3state = "off";
              digitalWrite(LED3, LOW);
            }
             //light r4
            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              LED4state = "on";
              digitalWrite(LED4, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              LED4state = "off";
              digitalWrite(LED4, LOW);
            }
             //FANstate
            else if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("GPIO 18 on");
              FANstate = "on";
              digitalWrite(FAN, HIGH);
            } else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("GPIO 18 off");
              FANstate = "off";
              digitalWrite(FAN, LOW);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>IOT Based Home Automation system </h1>");
            client.println("<body><h3>ESP32 web server </h3>");
            client.println("<body><p>Dept of CSE, JHSTC </p>");
            client.println("<body><p>by MOMIN, SHARIF, ARIF, RIPON, JEWEL</p>");
            22
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 4 - State " + LED1state + "</p>");
            // If the LED1state is off, it displays the ON button       
            if (LED1state=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 15 - State " + LED2state + "</p>");
            // If the LED2state is off, it displays the ON button       
            if (LED2state=="off") {
              client.println("<p><a href=\"/15/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/15/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
           // Display current state, and ON/OFF buttons for GPIO 20 
            client.println("<p>GPIO 26 - State " + LED3state + "</p>");
            // If the LED3state is off, it displays the ON button       
            if (LED3state=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 20 
            client.println("<p>GPIO 27 - State " + LED4state + "</p>");
            // If the RB is off, it displays the ON button       
            if (LED4state=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 18 - State " + FANstate + "</p>");
            // If the LED4state is off, it displays the ON button       
            if (FANstate=="off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("</body></html>");

            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}