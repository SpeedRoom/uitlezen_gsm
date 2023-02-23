#include <Arduino.h>
#include <string>
#include <map>

using namespace std;

#define trigPin 12
#define echoPin 13
#define   LEDlampYellow 26
#define buttonStartPin 2
#define buttonResetPin 19

int count_values =0;
int total_count=0;
string sequence1 ="";
string total_code="";
bool start_code = false;

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
    Serial.println("alles cleared");
}

void read_sensor(){
  long durationindigit, distanceincm;
  digitalWrite(trigPin, LOW);   
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durationindigit = pulseIn(echoPin, HIGH);
  distanceincm = (durationindigit/5) /13;
  if (distanceincm < 20 and distanceincm>2) {
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
    if (total_code.length()==5){
      Serial.print("total code is:");
      Serial.println(total_code.c_str());
      total_code.clear();
    }
    start_code=false;
  }  
}

void setup() {
  Serial.begin (9600);
  pinMode(trigPin,   OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDlampYellow, OUTPUT);
  pinMode(buttonStartPin, INPUT_PULLDOWN);
  pinMode(buttonResetPin, INPUT_PULLDOWN);
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
