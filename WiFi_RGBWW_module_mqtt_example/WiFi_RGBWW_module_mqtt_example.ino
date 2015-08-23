#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// #define PWMRANGE 255
// #define PWM_VALUE 31
//int gamma_table[PWM_VALUE+1] = {
//    0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11, 13, 16, 19, 23,
//    27, 32, 38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215, 255
//};
 
 #define PWM_VALUE 63
//int gamma_table[PWM_VALUE+1] = {
//    0, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 9, 10,
//    11, 12, 13, 15, 17, 19, 21, 23, 26, 29, 32, 36, 40, 44, 49, 55,
//    61, 68, 76, 85, 94, 105, 117, 131, 146, 162, 181, 202, 225, 250,
//    279, 311, 346, 386, 430, 479, 534, 595, 663, 739, 824, 918, 1023
//};

int gamma_table[PWM_VALUE+1] = {
    0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 9, 10,
    11, 12, 13, 15, 17, 19, 21, 23, 26, 29, 32, 36, 40, 44, 49, 55,
    61, 68, 76, 85, 94, 105, 117, 131, 146, 162, 181, 202, 225, 250,
    279, 311, 346, 386, 430, 479, 534, 595, 663, 739, 824, 918, 1023
};



// RGB FET
#define redPIN    12
#define greenPIN  13
#define bluePIN   15


// W FET
#define w1PIN     14
#define w2PIN     4

// onbaord green LED D1
#define LEDPIN    5
// onbaord red LED D2
#define LED2PIN   1

// note 
// TX GPIO2 @Serial1 (Serial ONE)
// RX GPIO3 @Serial    


#define LEDoff digitalWrite(LEDPIN,HIGH)
#define LEDon digitalWrite(LEDPIN,LOW)

#define LED2off digitalWrite(LED2PIN,HIGH)
#define LED2on digitalWrite(LED2PIN,LOW)

const char *ssid =  "*************";  //  your network SSID (name)
const char *pass =  "********";       // your network password


// Update these with values suitable for your network.
IPAddress server(192, 168, 2, 10);

void callback(const MQTT::Publish& pub) {
  Serial1.print(pub.topic());
  Serial1.print(" => ");
  Serial1.println(pub.payload_string());
  
  String payload = pub.payload_string();

if(String(pub.topic()) == "/openHAB/RGB_2/Color"){
  int c1 = payload.indexOf(';');
  int c2 = payload.indexOf(';',c1+1);
  
  int red = map(payload.toInt(),0,100,0,PWM_VALUE);
  red = constrain(red,0,PWM_VALUE);
  int green = map(payload.substring(c1+1,c2).toInt(),0,100,0,PWM_VALUE);
  green = constrain(green, 0, PWM_VALUE);
  int blue = map(payload.substring(c2+1).toInt(),0,100,0,PWM_VALUE);
  blue = constrain(blue,0,PWM_VALUE);
  
  red = gamma_table[red];
  green = gamma_table[green];
  blue = gamma_table[blue];
  
  
  analogWrite(redPIN, red);
  analogWrite(greenPIN, green);
  analogWrite(bluePIN, blue);

//   Serial1.println(red);
//   Serial1.println(green);
//   Serial1.println(blue);
 }
 else if(String(pub.topic()) == "/openHAB/RGB_2/SW1"){
   int value = map(payload.toInt(),0,100,0,PWM_VALUE);
    value = constrain(value,0,PWM_VALUE);
    value = gamma_table[value];
    analogWrite(w1PIN, value);
    //Serial1.println(value);
   }
 else if(String(pub.topic()) == "/openHAB/RGB_2/SW2"){
   int value = map(payload.toInt(),0,100,0,PWM_VALUE);
    value = constrain(value,0,PWM_VALUE);
    value = gamma_table[value];
 
    
    analogWrite(w2PIN, value);
    //Serial1.println(value);
   }   
}

PubSubClient client(server);

void setup()
{
  pinMode(LEDPIN, OUTPUT);  
  pinMode(LED2PIN, OUTPUT);  
  
  pinMode(redPIN, OUTPUT);
  pinMode(greenPIN, OUTPUT);
  pinMode(bluePIN, OUTPUT);
  pinMode(w1PIN, OUTPUT);
  pinMode(w2PIN, OUTPUT);    
  
  // Setup console
  Serial1.begin(115200);
  delay(10);
  Serial1.println();
  Serial1.println();

  client.set_callback(callback);

  WiFi.begin(ssid, pass);

  LEDon;
  
  while (WiFi.status() != WL_CONNECTED) {

    LED2off;
    delay(500);
    Serial1.print(".");
    LED2on;
  }
  
  Serial1.println("");
  
  Serial1.println("WiFi connected");
  Serial1.println("IP address: ");
  Serial1.println(WiFi.localIP());
   
  
  if (client.connect("WiFi_RGB_2")) {
    client.subscribe("/openHAB/RGB_2/Color");
    client.subscribe("/openHAB/RGB_2/SW1");
    client.subscribe("/openHAB/RGB_2/SW2");
    Serial1.println("MQTT connected");  
  }

  Serial1.println("");
}


void loop()
{
  client.loop();
}
