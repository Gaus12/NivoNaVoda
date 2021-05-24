#include <ESP8266WiFi.h>
const char* ssid     = "SSID";
const char* password = "PW";

WiFiServer server(80);

String header;
String output4State = "off";
String output5State = "off";
String output7State = "off";
String output3State = "off";
int triggerPin = 9;
int echoPin = 12;
long vremetraenje, rastojanie;

long MikrosekundiVoCentimetri(long vremetraenje)
{
  long d = (vremetraenje/2)*0.0343;
  return d;
}
const int output4 = 4;
const int output5 = 5;
const int output7 = 7;
const int output3 = 3;
void setup() {
    Serial.begin(115200);

 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println("SSID");

  WiFi.begin("SSID", "PW");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("My IP: ");
  Serial.println(WiFi.localIP());
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(output7, LOW);
  digitalWrite(output3, LOW);
}

void loop() {
   WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
        delayMicroseconds(10);
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
        }   

    vremetraenje = pulseIn(echoPin, HIGH);
  rastojanie = MikrosekundiVoCentimetri(vremetraenje);
  Serial.print("Rastojanieto e: ");
  Serial.println(rastojanie);
    
     if(rastojanie<=10){
   Serial.println("GPIO 4 on");
    digitalWrite(4, HIGH);
  }
  else
  {
    digitalWrite(4, LOW);
    Serial.println("GPIO 4 off");
  }

  if(rastojanie<=6){
    Serial.println("GPIO 5 on");
    digitalWrite(5, HIGH);
  }
  else
  {
    Serial.println("GPIO 5 off");
    digitalWrite(5, LOW);
  }

  if(rastojanie<=4){
    Serial.println("GPIO 7 on");
    digitalWrite(7, HIGH);
  }
  else
  {
    Serial.println("GPIO 7 off");
    digitalWrite(7, LOW);
  }

  if(rastojanie<=2){
    Serial.println("GPIO 3 on");
    digitalWrite(3, HIGH);
  }
  else
  {
    Serial.println("GPIO 3 off");
    digitalWrite(3, LOW);
  delayMicroseconds(10);
  }
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
    
     String webpage = "<h1>SCADA</h1>";
             webpage += "<p><h2>";   
       if (rastojanie<=10)
       {
        webpage+= " Branata e polna do 25% od maksimalniot kapacitet";
        }
        else if(rastojanie<=6){
          webpage+= " Branata e polna do 50% od maksimalniot kapacitet";
          }
        else if(rastojanie<=4){
          webpage+= " Branata e polna do 75% od maksimalniot kapacitet";
          }
        else if(rastojanie<=2){
          webpage+= " Branata e polna do 100% od maksimalniot kapacitet";
          }
       webpage += "</h2></p></body>";   
            client.println();
            break;
          } else {             currentLine = "";
          }
     
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}
}
