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
#define SSID          "NETGEAR68"
#define PWD           "excitedtuba713"

int count_values =0;
int total_count=0;
bool start_code = false;
string sequence ="";
string total_code="";
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

//OTA
OTAlib ota(SSID, PWD);
/* OTAlib ota("NETGEAR68", "excitedtuba713"); */

int morseToDigit(string sequence) {
  for (int i=0; i<10;i++){
    if (sequence==convert_map[i].seq){
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
    sequence.clear();
    digitalWrite(LEDlampGreen,LOW);
    digitalWrite(LEDlampRed,LOW);
    digitalWrite(Lock_Relay_pin,HIGH);
    Serial.println("reset");
}

void check_code(){
    total_code.append(to_string(morseToDigit(sequence)));
    sequence.clear();

    if (total_code.length()==4){
      if (code == total_code){
        Serial.println("Code correct");

        digitalWrite(LEDlampGreen,HIGH);
        digitalWrite(LEDlampRed,LOW);
        digitalWrite(Lock_Relay_pin,LOW);

        delay(5000);
        //taskYIELD();

        reset();
      }
      else {
        Serial.println("Code niet correct");

        digitalWrite(LEDlampGreen,LOW);
        digitalWrite(LEDlampRed,HIGH);
        digitalWrite (Lock_Relay_pin,HIGH);
        
        delay(2000);
        //taskYIELD();
        digitalWrite(LEDlampRed,LOW);
        
        reset();
      }

    total_code.clear();
}
}

void read_sensor(){
  Serial.println("reading sensor");

  long detected_digit, detected;

  digitalWrite(trigPin, LOW);   
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  detected_digit = pulseIn(echoPin, HIGH);
  detected = (detected_digit/5) /13;

  if (detected < 20 and detected>0) {
    digitalWrite(LEDlampYellow,   HIGH);
    count_values++;
  }
  else {
    digitalWrite(LEDlampYellow,LOW);
    total_count=count_values;
    count_values=0;
  }

  if (total_count>2){
    sequence.append("-");
    total_count=0;
  }
  else if (total_count>0){
    sequence.append(".");
    total_count=0;
  }

  if (sequence.length()>4){
    check_code();
    }

    start_code=false;
}  

void setup() {
  // OTA
	ota.setHostname("espultrasoon");  
	ota.setPassword("espultrasoon");
	ota.begin();

  Serial.begin (115200);
  pinMode(trigPin,   OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDlampYellow, OUTPUT);
  pinMode(LEDlampGreen, OUTPUT);
  pinMode(LEDlampRed, OUTPUT);
  pinMode(Lock_Relay_pin,OUTPUT);
  pinMode(buttonStartPin, INPUT_PULLDOWN);
  pinMode(buttonResetPin, INPUT_PULLDOWN);

  digitalWrite(LEDlampGreen,LOW);
  digitalWrite(LEDlampRed,LOW);
  digitalWrite(Lock_Relay_pin,HIGH);
}

void loop() {
  if (digitalRead(buttonResetPin)){
    reset();
    delay(100);
    //taskYIELD();
  }
  if (digitalRead(buttonStartPin)){
      start_code=true;
      Serial.println("start");
  }
  if (start_code){
    read_sensor();
    delay(300);    
    //taskYIELD();
  }
}