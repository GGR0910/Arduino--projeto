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
  
  //Inicialização display lcd i2c
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bem vindo!");
  delay(2000);
  lcd.clear();

  //Configuração do modo padrão
  ConfigureLeds();
  GetData();

} 

void loop()
{
  CheckGasSensor();
  CheckButton();
  GetData();
  Serial.println(Mode);
}

void CheckGasSensor(){
  if(analogRead(MQ2Pin) > MaxGasAcceptedValue){
    lcd.clear();
    digitalWrite(RedLigthPin, HIGH);
    digitalWrite(BuzerPin, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Alerta de");
    lcd.setCursor(0,1);
    lcd.print("gas/fumaca!");
    delay(5000);
  }
  else{
    lcd.clear();
    digitalWrite(RedLigthPin, LOW);
    digitalWrite(BuzerPin, LOW);
  }
}

void GetData(){

  if(Mode == 1){
    //Horario
  }
  if(Mode == 2){
    //Temperatura e umidade
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.setCursor(6,0);
    lcd.print(t);
    lcd.setCursor(12,0);
    lcd.print("C");

    lcd.setCursor(0,1);
    lcd.print("Umid:");
    lcd.setCursor(6, 1);
    lcd.print(h);
  }
  if(Mode == 3){
    //Altura
      lcd.clear();
      digitalWrite(Trigpin, LOW);
      delayMicroseconds(2);
      digitalWrite(Trigpin, HIGH);
      delayMicroseconds(10);
      digitalWrite(Trigpin, LOW);

      long durationInMicroseconds = pulseIn(Echopin, HIGH);
      long distanceInCM = 200 - (durationInMicroseconds / 29 / 2);

      lcd.setCursor(1,0);
      lcd.print("Altura:");
      lcd.setCursor(1, 1);
      if(distanceInCM < 200 && distanceInCM > 0){
        lcd.print(distanceInCM);
        lcd.setCursor(4,1);
        lcd.print("cm");
      }
      else{
        lcd.print("Error");
      }   
  }
}

void CheckButton(){
  if (digitalRead(buttonPin) == HIGH){
    Mode = Mode + 1;
    ConfigureLeds();
    delay(1000);
    lcd.clear();
  }
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



