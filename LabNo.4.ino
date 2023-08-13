#include <Arduino.h>
#include <ESP32Servo.h>
#include <driver/ledc.h>
#include <stdint.h>

//Servo Parte 1 y 3
const int servoPin = 19;
const int button1Pin = 34;// VERDE
const int button2Pin = 35; // NEGRO
bool button1 = LOW;
bool button2 = LOW;
#define servoChannel 0
#define freq0 5000
#define resolution2 8
// variable para guardar la posición del servo
int servoPosition = 0;
Servo myServo;
void configurarPWM2(void);

// VARIABLES, ENTREGABLES LAB
void configurarPWM(void);
const int ledPin1 = 13; //rojo
const int ledPin2 = 12; //amarillo
const int ledPin3 = 14; //azul

const int buttonPinA = 27; //blanco
const int buttonPinB = 32; //amarillo

bool buttonA = LOW;
bool buttonB = LOW;

// definicion de canales
#define rojo 1
#define verde 2
#define azul 3
// definicion de frecuencias
#define freq1 8000
#define freq2 8000
#define freq3 8000
//resolucion
#define resolution 8 
// Antirrebote
unsigned long ultimoRebote = 0;
unsigned long retraso =  50;
unsigned long actualizacion = 0;
const unsigned long intervalo = 250;

int count = 0; // contador que elige led
int incremento = 0; // contador que incremente brillo

#define pinPWM 15  // salida del PWM

void configurarPWM2(void){
  ledcSetup(servoChannel, freq0, resolution2);
  ledcAttachPin(servoPin, servoChannel);
 }
void configurarPWM(void)
{
  ledcSetup(rojo, freq1, resolution);
  ledcSetup(verde, freq2, resolution);
  ledcSetup(azul, freq3, resolution);
  ledcAttachPin(ledPin1, rojo);
  ledcAttachPin(ledPin2, verde);
  ledcAttachPin(ledPin3, azul);
  }
// INTERRUPCIÓN PARA button1Pin (VERDE)

void  IRAM_ATTR ISRbutton1() {
  button1 = digitalRead(button1Pin);
  if (button1 == HIGH) {
    servoPosition++;
  }
 }

// INTERRUPCIÓN PARA button2Pin (NEGRO)
void IRAM_ATTR ISRbutton2() { 
  button2 = digitalRead(button2Pin);
  if (button2 == HIGH) {
    servoPosition--;
  }

}

// INTERRUPCIÓN PARA LEDS
void IRAM_ATTR ISRbutton3() {
  buttonA = digitalRead(buttonPinA);
  if ((millis() - ultimoRebote) > retraso) {
  if (buttonA == HIGH) {
  count++;
}
 ultimoRebote = millis();
}
}

// Funcion para incrementar birllo
void ISRbutton4() {
 if (digitalRead(buttonPinB) == HIGH) {
    incremento = incremento + 20;
    if (incremento > 255) {
      incremento = 0;
   }
}
}

void setup() {
  configurarPWM2();
  configurarPWM();

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
// ENTREGABLE
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
//Interrupciones
  attachInterrupt(digitalPinToInterrupt(button1Pin), ISRbutton1, RISING);
  attachInterrupt(digitalPinToInterrupt(button2Pin), ISRbutton2, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPinA), ISRbutton3, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPinB), ISRbutton4, RISING);

   myServo.attach(servoPin);  // Asociar el objeto servo al pin
  myServo.write(servoPosition);        // Inicializar la posición del servo
}

void loop() {
  ledcWrite(servoChannel, servoPosition);

  delay(10);
   if (count >= 5) {
    count = 0;
    }
  if  (count == 1){
    servoPosition = 0;
    myServo.write(servoPosition);

  delay(300);
  ledcWrite(rojo, incremento);
  ledcWrite(verde, 0); // 
  ledcWrite(azul, 0); // 

  }
  if  (count == 2){
    servoPosition = 90;
    myServo.write(servoPosition);
  delay(300);
  ledcWrite(verde, incremento);
  ledcWrite(rojo, 0); 
  ledcWrite(azul, 0); 
  }
  if  (count == 3){
    servoPosition = 180;
    myServo.write(servoPosition);
  delay(300);
  ledcWrite(azul, incremento);
  ledcWrite(rojo, 0);
  ledcWrite(verde, 0);
  }

  if (count == 4) {
  ledcWrite(rojo, 0); 
  ledcWrite(verde, 0); 
  ledcWrite(azul, 0); 

}
}
