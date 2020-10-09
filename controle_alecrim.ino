//Bibliotecas
#include "DHT.h"
#include "Adafrruit_sensor.h"

//Definições do sensor DHT22 (Temperatura e Umidade do Ar)
#define DHTPIN 1;
#define DHTTYPE DHT22;
DHT dht(DHTPIN, DHTTYPE);
float temperatura; //Variável de temperatura
float umidade_ar; //Vairável de umidade do ar

//Definições do Sensor Capacitivo (Umidade do Solo)
const int ar = 620;
const int agua = 310;
int umidade_solo = 0;
int umidade_solo_percent = 0;

//Definições dos Relés
#define rele_led 3
#define rele_vent 5
#define rele_resist 7
#define rele_bomba 9

//Iluminação
unsigned long controle_iluminacao;
unsigned long check_iluminacao;


//Definindo código de inicialização
void setup() {
    Serial.begin(9600);
    dht.begin();
    controle_ilumincao = millis();
}

//Definindo Loop
void loop() {

    //========================================SENSORIAMENTO========================================
    
    //Atualizando valores de temperatura e umidade do ar    
    umidade_ar = dht.readHumidity();
    temperatura = dht.readTemperature();

    //Prints no monitor
    Serial.print("Umidade do ar: ");
    Serial.print(umnidade_ar);
    Serial.print(" %, Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    
    //Atualizando valores de umidade do solo    
    umidade_solo = analogRead(A0);
    umidade_solo_percent = map(umidade_solo, ar, agua, 0, 100);

    //Prints no monitor
    if(umidade_solo_percent >= 100)
      Serial.println("Umidade do solo: 100%");
    else if(umidade_solo_percent <= 0)
      Serial.println("Umidade do solo: 0%");
    else if(umidade_solo_percent >0 && umidade_solo_percent <100)
      {Serial.print("Umidade do solo:")
      Serial.print(umidade_solo_percent);
      Serial.println(" %");}
        
    //===========================================CONTROLE===========================================

    //Umidade do Ar
    if(umidade_ar>70) digitalWrite(rele_vent, HIGH);
    else digitalWrite(rele_vent, LOW);

    //Temperatura
    if(temperatura>20) digitalWrite(rele_vent, HIGH);
    else digitalWrite(rele_vent, LOW);
    if(temperatura<15) digitalWrite(rele_resist, HIGH);
    else digitalWrite(rele_resist, LOW);

    //Umidade do Solo
    if(umidade_solo_percent<40) digitalWrite(rele_bomba, HIGH);
    else digitalWrite(rele_bomba, LOW);

    //==========================================ILUMINAÇÃO==========================================
    //Alecrim requer 8 horas de período claro 8h=28800000ms. 16h=57600000
    check_ilumincao = millis();
    if((check_iluminacao - controle_iluminacao) > 28800000 && digitalRead(rele_led)==HIGH)
      {digitalWrite(rele_led, LOW);
      controle_iluminacao = millis();}
    else if((check_iluminacao - controle_iluminacao) > 57600000 && digitalRead(rele_led)==LOW)
      {digitalWrite(rele_led, HIGH);
      controle_iluminacao = millis();}
}
