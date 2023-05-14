/// naam esp: espultrasoon
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

void reset(){//reset functie
    total_code.clear();
    count_values = 0;
    total_count = 0;
    sequence.clear();
    digitalWrite(LEDlampGreen,LOW);
    digitalWrite(LEDlampRed,LOW);
    digitalWrite(LEDlampYellow,LOW);
    digitalWrite(Lock_Relay_pin,LOW);
    Serial.println("reset");
    start_code = false;
}

void check_code(){//checkt of de code correct is

    if (total_code.length()==4){
      if (code == total_code){
        Serial.println("Code correct");

        digitalWrite(LEDlampGreen,HIGH);
        digitalWrite(LEDlampRed,LOW);
        digitalWrite(Lock_Relay_pin,HIGH);
        vTaskDelay(5000/ portTICK_PERIOD_MS);
        reset();
      }
      else {
        Serial.println("Code niet correct");

        digitalWrite(LEDlampGreen,LOW);
        digitalWrite(LEDlampRed,HIGH);
        digitalWrite (Lock_Relay_pin,LOW);
        
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        digitalWrite(LEDlampRed,LOW);
        reset();
      }

    total_code.clear();
}
}

void read_sensor(){//leest de sensor uit
  Serial.println("reading sensor");
  long detected;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  taskYIELD();
  detected = pulseIn(echoPin, HIGH);
  Serial.println(detected);
  if (detected <10000 and detected>0) {
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
  taskYIELD();
  if (sequence.length()>4){
    total_code.append(to_string(morseToDigit(sequence)));
    sequence.clear();
    check_code();
    start_code=false;
  }
}  

void setup() {//setup
  // OTA
	ota.setHostname("espultrasoon");  
	ota.setPassword("espultrasoon");
	ota.begin();

  Serial.begin (115200);
  
  //pinmodes
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
  digitalWrite(Lock_Relay_pin,LOW);
}

void loop() {
  if (digitalRead(buttonResetPin)){
    reset();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  if (digitalRead(buttonStartPin)){
      start_code=true;
      Serial.println("start");
  }
  if (start_code){
    read_sensor();
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
