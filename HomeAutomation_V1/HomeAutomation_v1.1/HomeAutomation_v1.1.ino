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
String R1B = "off";
String R2B = "off";
String R3B = "off";
String R4B = "off"; 
String FAN = "off";


// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
const int output20 = 20;
const int output21 = 21;
const int output22 = 22;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(output20, OUTPUT);
  pinMode(output21, OUTPUT);
  pinMode(output22, OUTPUT);

  // Set outputs to LOW
  
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(output20, LOW);
  digitalWrite(output21, LOW);
  digitalWrite(output22, LOW);
  
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
            if (header.indexOf("GET /26/on") >= 0) {
              //Serial.println(header.indexOf("GET /26/on"));
              Serial.println("GPIO 26 on");
              R1B = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              R1B = "off";
              digitalWrite(output26, LOW);
            } 
            //light r2
            else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              R2B = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              R2B = "off";
              digitalWrite(output27, LOW);
            }
            
            //light r3
            else if (header.indexOf("GET /20/on") >= 0) {
              Serial.println("GPIO 20 on");
              R3B = "on";
              digitalWrite(output20, HIGH);
            } else if (header.indexOf("GET /20/off") >= 0) {
              Serial.println("GPIO 20 off");
              R3B = "off";
              digitalWrite(output20, LOW);
            }
             //light r4
            else if (header.indexOf("GET /21/on") >= 0) {
              Serial.println("GPIO 21 on");
              R4B = "on";
              digitalWrite(output21, HIGH);
            } else if (header.indexOf("GET /21/off") >= 0) {
              Serial.println("GPIO 21 off");
              R4B = "off";
              digitalWrite(output21, LOW);
            }
             //fan
            else if (header.indexOf("GET /22/on") >= 0) {
              Serial.println("GPIO 22 on");
              FAN = "on";
              digitalWrite(output22, HIGH);
            } else if (header.indexOf("GET /22/off") >= 0) {
              Serial.println("GPIO 22 off");
              FAN = "off";
              digitalWrite(output22, LOW);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-flex;flex-direction:colum; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>IOT Based Home Automation system </h1>");
            client.println("<body><h3>ESP32 web server </h3>");
            client.println("<body><p>Dept of CSE, JHSTC </p>");
            client.println("<body><p>by MOMIN, SHARIF, ARIF, RIPON, JEWEL</p>");
            
            


        
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + R1B + "</p>");
            // If the R1B is off, it displays the ON button       
            if (R1B=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + R2B + "</p>");
            // If the R2B is off, it displays the ON button       
            if (R2B=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
           // Display current state, and ON/OFF buttons for GPIO 20 
            client.println("<p>GPIO 20 - State " + R3B + "</p>");
            // If the R3B is off, it displays the ON button       
            if (R3B=="off") {
              client.println("<p><a href=\"/20/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/20/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 20 
            client.println("<p>GPIO 21 - State " + R4B + "</p>");
            // If the RB is off, it displays the ON button       
            if (R4B=="off") {
              client.println("<p><a href=\"/21/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/21/off\"><button class=\"button button2\">OFF</button></a></p>");
            }         //    if (R4B=="off") {

            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 22 - State " + FAN + "</p>");
            // If the R4B is off, it displays the ON button       
            if (FAN=="off") {
              client.println("<p><a href=\"/22/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/22/off\"><button class=\"button button2\">OFF</button></a></p>");
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
