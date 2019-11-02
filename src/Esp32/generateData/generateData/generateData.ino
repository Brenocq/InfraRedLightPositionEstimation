#define LIGHT_0  36 
#define LIGHT_1  39
#define LIGHT_2  34
#define LIGHT_3  35
#define LIGHT_4  32
#define LIGHT_5  33
#define LIGHT_6  25
#define LIGHT_7  26
#define LIGHT_8  27
#define LIGHT_9  14
#define LIGHT_10 15
#define LIGHT_11 13

int pins[]={ 
  LIGHT_0  
, LIGHT_1 
, LIGHT_2 
, LIGHT_3 
, LIGHT_4 
, LIGHT_5 
, LIGHT_6 
, LIGHT_7 
, LIGHT_8 
, LIGHT_9 
, LIGHT_10
, LIGHT_11};

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);

  for(int i=0;i<12;i++){
    pinMode(pins[i],  INPUT); 
  }
}

int reading;
void loop() {
  String firstNum, secondNum;
  reading = 1;
  while(true){
    char c;
    if (Serial2.available() > 0){
      c = Serial2.read();
    }
    if(reading==1){
      if(c==','){
        reading=2;
      }
  
      if(c>='0'&& c<='9')
        firstNum+=c;
    }else{
      if(c==')'){
        goto end;
      }
      if(c>='0'&& c<='9')
        secondNum+=c;
    }
  }
  end:
  int x = firstNum.toInt();
  int y = secondNum.toInt();
  Serial.print(x);Serial.print(",");Serial.print(y);Serial.print(",");
  for(int i=0;i<12;i++){
        Serial.print(analogRead(pins[i]));
        if(i!=11)
          Serial.print(",");
        else
          Serial.println();
      }
}
