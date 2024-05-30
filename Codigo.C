#include <LiquidCrystal_I2C.h>
#include <DHT.h>

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
#define botao 6

//Variaveis globais
int Modo = 1;

void setup()
{
  Serial.begin(9600);
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (8, OUTPUT);
  pinMode (botao, INPUT);
  ConfigurarLeds();
  MostrarDados();
  dht.begin();
}

void loop()
{
  ChecaBotao();
}

void PegarDados(){

  int sensor;
  int valorCalculado;
  switch (Modo)
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
       
      break;
  }
}

void ChecaBotao(){
  if (digitalRead(botao) == HIGH){
    Serial.println(Modo);
    Modo = Modo + 1;
    ConfigurarLeds();
    delay(1200);
  }
  MostrarDados();
}

void ApagarLeds(){
  digitalWrite (2, LOW);
  digitalWrite (3, LOW);
  digitalWrite (4, LOW);
}

void ConfigurarLeds(){
  ApagarLeds();
  switch (Modo)
  {
    case 1:
      digitalWrite (2, HIGH);
      break;
    case 2:
      digitalWrite (3, HIGH);
      break;
    case 3:
      digitalWrite (4, HIGH);
      break;
    default:
      digitalWrite (2, HIGH);
      Modo = 1;
      break;
  }
}

void MostrarDados(){
  PegarDados();
}

