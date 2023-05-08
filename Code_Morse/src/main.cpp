// naam esp: espultrasoon
// wachtwoord esp: espultrasoon

#include <Arduino.h>
#include <string>
#include <map>
#include <WiFi.h>
#include "OTAlib.h"

using namespace std;

#define trigPin 17
#define echoPin 16
#define LEDlampYellow 19
#define buttonStartPin 2
#define buttonResetPin 4
#define LEDlampGreen 18
#define Lock_Relay_pin 32
#define LEDlampRed 5

//OTA
/* OTAlib ota("NETGEAR68", "excitedtuba713"); */

int count_values =0;
int total_count=0;
string sequence1 ="";
string total_code="";
bool start_code = false;
string code = "8152";

struct SequenceToDigit{
  string seq;
  int value;
};
SequenceToDigit convert_map[] = {
    {".----", 1},
    {"..---", 2},
    {"...--", 3},
    {"....-", 4},
    {".....", 5},
    {"-....", 6},
    {"--...", 7},
    {"---..", 8},
    {"----.", 9},
    {"-----", 0}
};

// Function to convert Morse code to digits
int morseToDigit(string sequence1) {
  for (int i=0; i<10;i++){
    if (sequence1==convert_map[i].seq){
      return convert_map[i].value;
      break;
    }
  }
  return '\0';
}

void reset(){
    total_code.clear();
    count_values = 0;
    total_count = 0;
    sequence1.clear();
    digitalWrite(LEDlampGreen,LOW);
    digitalWrite(LEDlampRed,LOW);
    digitalWrite(Lock_Relay_pin,LOW);
    Serial.println("alles cleared");
}

void read_sensor(){
  Serial.println("reading sensor");
  long durationindigit, distanceincm;
  digitalWrite(trigPin, LOW);   
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durationindigit = pulseIn(echoPin, HIGH);
  distanceincm = (durationindigit/5) /13;
  Serial.print("distance in cm: ");
  Serial.println(distanceincm);
  if (distanceincm < 20 and distanceincm>0) {
    digitalWrite(LEDlampYellow,   HIGH);
    count_values++;
  }
  else {
    digitalWrite(LEDlampYellow,LOW);
    total_count=count_values;
    count_values=0;
  }
  if (total_count>2){
    sequence1.append("-");
    total_count=0;
  }
  else if (total_count>0){
    sequence1.append(".");
    total_count=0;
  }
  // Serial.print(distanceincm);
  // Serial.println("   cm");
  if (sequence1.length()>4){
    Serial.println(morseToDigit(sequence1));
    total_code.append(to_string(morseToDigit(sequence1)));
    sequence1.clear();
    if (total_code.length()==4){
      Serial.print("total code is:");
      Serial.println(total_code.c_str());
      if (code == total_code){
        Serial.println("de code is sws correct ");
        digitalWrite(LEDlampGreen,HIGH);
        digitalWrite(LEDlampRed,LOW);
        digitalWrite(Lock_Relay_pin,LOW);
        delay(5000);
        reset();
      }
      else {
        Serial.println("code is niet coreect broer");
        digitalWrite(LEDlampRed,HIGH);
        digitalWrite (Lock_Relay_pin,HIGH);
        digitalWrite(LEDlampGreen,LOW);
        delay(2000);
        digitalWrite(LEDlampRed,LOW);
        reset();
      }
    total_code.clear();
    }
    start_code=false;
  }  
}

void setup() {
  // OTA
 /*  ota.setHostname("espultrasoon");  
  ota.setPassword("espultrasoon");
  ota.begin();
   */
  Serial.begin (115200);
  pinMode(trigPin,   OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDlampYellow, OUTPUT);
  pinMode(LEDlampGreen, OUTPUT);
  pinMode(LEDlampRed, OUTPUT);
  pinMode(Lock_Relay_pin,OUTPUT);
  pinMode(buttonStartPin, INPUT_PULLDOWN);
  pinMode(buttonResetPin, INPUT_PULLDOWN);
  digitalWrite(Lock_Relay_pin,HIGH);
}

void   loop() {
  if (digitalRead(buttonResetPin)){
    reset();
    delay(100);
  }
  if (digitalRead(buttonStartPin)){
      start_code=true;
      Serial.println("start code true");
  }
  if (start_code){
    read_sensor();
    delay(300);    
  }
}