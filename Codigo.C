#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Configurações do HC-SR04 ultrassonico
#define Echopin 10
#define Trigpin 9

//Configurações da tela
#define adress 0x27
#define columns 16
#define lines 2
LiquidCrystal_I2C lcd(adress, columns, lines);

//Configurações de sensor DHT11
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Configuração do botão
#define buttonPin 6

//Configuração do sensor de gás e componentes
#define MQ2Pin A0
#define BuzerPin 8
#define RedLigthPin 5
#define MaxGasAcceptedValue 300

//LedsDeModo
#define TimeLed 2
#define TempNWaterLed 3
#define HeigthLed 4

//Variaveis globais
int Mode = 1;

void setup()
{
  Serial.begin(9600);
  
  //Configurações de modo
  pinMode(TimeLed, OUTPUT);
  pinMode(TempNWaterLed, OUTPUT);
  pinMode(HeigthLed, OUTPUT);
  pinMode(buttonPin, INPUT);

 //Configurações do sensor de gás
  pinMode(MQ2Pin,INPUT);
  pinMode(RedLigthPin, OUTPUT);
  pinMode(BuzerPin, OUTPUT);

  //Configurações do sensor ultrassonico 
  pinMode(Trigpin, OUTPUT);
  pinMode(Echopin, INPUT);

  //Inicialização de sensores
  dht.begin();

  //Configuração do modo padrão
  ConfigureLeds();
  ShowData();

}

void loop()
{
  CheckButton();
  CheckGasSensor();
}

void CheckGasSensor(){
  if(analogRead(MQ2Pin) > MaxGasAcceptedValue){
    digitalWrite(RedLigthPin, HIGH);
    digitalWrite(BuzerPin, HIGH);
  }
  else{
    digitalWrite(RedLigthPin, LOW);
    digitalWrite(BuzerPin, LOW);
  }
}

void GetData(){
  switch (Mode)
  {
    case 1:
    //horario
      break;
    case 2:
    //Temperatura e umidade
      float t = dht.readTemperature();
      float h = dht.readHumidity();
      break;
    case 3:
      //Altura
      digitalWrite(Trigpin, LOW);
      delayMicroseconds(2);
      digitalWrite(Trigpin, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trigpin, LOW);

      long durationInMicroseconds = pulseIn(Echopin, HIGH);
      long distanceInCM = 200 - (durationInMicroseconds / 29 / 2);
      break;
  }
}

void CheckButton(){
  if (digitalRead(buttonPin) == HIGH){
    Mode = Mode + 1;
    ConfigureLeds();
    delay(1000);
  }
  ShowData();
}

void TurnLedsOff(){
  digitalWrite (HeigthLed, LOW);
  digitalWrite (TempNWaterLed, LOW);
  digitalWrite (TimeLed, LOW);
}

void ConfigureLeds(){
  TurnLedsOff();
  switch (Mode)
  {
    case 1:
      digitalWrite (TimeLed, HIGH);
      break;
    case 2:
      digitalWrite (TempNWaterLed, HIGH);
      break;
    case 3:
      digitalWrite (HeigthLed, HIGH);
      break;
    default:
      digitalWrite (2, HIGH);
      Mode = 1;
      break;
  }
}

void ShowData(){
  GetData();
}


