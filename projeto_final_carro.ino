#include <AFMotor.h>
#include <RotaryEncoder.h>
#include <Servo.h>

AF_DCMotor motorD(1);
AF_DCMotor motorE(2);

RotaryEncoder encoderD(20, 21);
RotaryEncoder encoderE(18, 19);

long posicaoAnteriorD = 0;
long posicaoAnteriorE = 0;
long instanteAnterior = 0;

int terra = 16;
int vcc = 17;
int led = 40;

float raio = 0.028;
float circunferencia = 2 * PI * raio;
float tick = 200;
float distancia_por_tick = circunferencia / tick;

int eixoX = 0;
int eixoY = 0;

Servo servoGarra;
int pinoServo = 10;
int anguloAberto = 0;
int anguloFechado = 25;

int segundos = 0;
int segundos_ligado = 0;
int minutos_ligado = 0;

void tickDoEncoderD() {
  encoderD.tick();
}

void tickDoEncoderE() {
  encoderE.tick();
}

void setup() {
  pinMode(terra, OUTPUT);
  pinMode(vcc, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(terra, LOW);
  digitalWrite(vcc, HIGH);

  Serial.begin(9600);
  Serial3.begin(9600);
  int origem1D = digitalPinToInterrupt(20);
  attachInterrupt(origem1D, tickDoEncoderD, CHANGE);
  int origem2D = digitalPinToInterrupt(21);
  attachInterrupt(origem2D, tickDoEncoderD, CHANGE);

  int origem1E = digitalPinToInterrupt(18);
  attachInterrupt(origem1E, tickDoEncoderE, CHANGE);
  int origem2E = digitalPinToInterrupt(19);
  attachInterrupt(origem2E, tickDoEncoderE, CHANGE);

  servoGarra.attach(pinoServo);
  servoGarra.write(0);
}

void loop() {
  if (Serial3.available() > 0) {
    String texto = Serial3.readStringUntil('\n');
    texto.trim();
    Serial.println(texto);

    if (texto.startsWith("ABRIR")) {
      servoGarra.write(anguloAberto);
    }

    if (texto.startsWith("FECHAR")) {
      servoGarra.write(anguloFechado);
    }

    if (texto.startsWith("ACENDER")) {
      digitalWrite(led, HIGH);
    }

    if (texto.startsWith("APAGAR")) {
      digitalWrite(led, LOW);
    }

    if (texto.startsWith("ANDAR")) {
      int i = texto.indexOf(" ");
      int velocidade = texto.substring(i + 1).toInt();
      eixoY = velocidade;
    }
    else if (texto.startsWith("DIRECAO")) {
      int i = texto.indexOf(" ");
      int velocidade = texto.substring(i + 1).toInt();
      eixoX = velocidade;
    }

    if (abs(eixoY) < 25) {
      eixoY = 0;
    }
    if (abs(eixoX) < 25) {
      eixoX = 0;
    }
    
    int velocidadeD;
    int velocidadeE;
    if (eixoY <= 50) velocidadeD = eixoY / 1.31;
    else if (eixoY <= 100) velocidadeD = eixoY / 1.29;
    else if (eixoY <= 150) velocidadeD = eixoY / 1.21;
    else if (eixoY <= 200) velocidadeD = eixoY / 1.17;
    else if (eixoY <= 255) velocidadeD = eixoY;
    velocidadeE = eixoY;

    if (eixoX > 0) {
      velocidadeD = eixoY - (eixoY * eixoX) / 255;
    }
    else if (eixoX < 0) {
      velocidadeE = eixoY - (eixoY * abs(eixoX) / 255);
    }

    if (velocidadeD == 0) {
      motorD.run(RELEASE);
    }
    else {
      motorD.setSpeed(abs(velocidadeD));
      if (velocidadeD > 0) {
        motorD.run(FORWARD);
      }
      else {
        motorD.run(BACKWARD);
      }
    }
    if (velocidadeE == 0) {
      motorE.run(RELEASE);
    }
    else {
      motorE.setSpeed(abs(velocidadeE));
      if (velocidadeE > 0) {
        motorE.run(FORWARD);
      }
      else {
        motorE.run(BACKWARD);
      }
    }
  }

  int posicaoD = encoderD.getPosition();
  int posicaoE = encoderE.getPosition();
  if (posicaoD != posicaoAnteriorD) {
    //Serial.println("Direito: " + String(posicaoD));
  }
  if (posicaoE != posicaoAnteriorE) {
    //Serial.println("Esquerdo: " + String(posicaoE));
  }
  
  unsigned long instanteAtual = millis();
  if (instanteAtual > instanteAnterior + 1000) {
    segundos++;
    segundos_ligado = segundos % 60;
    minutos_ligado = segundos / 60;
    String tempo = (minutos_ligado < 10 ? "0" : "") + String(minutos_ligado) + ":" + (segundos_ligado < 10 ? "0" : "") + String(segundos_ligado);
    Serial.println(tempo);
    long delta_tickD = posicaoAnteriorD - posicaoD;
    long delta_tickE = posicaoAnteriorE - posicaoE;
    float velocidadeD = (delta_tickD * distancia_por_tick) * 3.6f;
    float velocidadeE = (delta_tickE * distancia_por_tick) * 3.6f;
    float rpmD = (velocidadeD * 1000) / (circunferencia * 60);
    float rpmE = (velocidadeE * 1000) / (circunferencia * 60);
    instanteAnterior = instanteAtual;

    if ((velocidadeD != 0 && rpmD != 0) || (velocidadeE != 0 && rpmE != 0)) {
      Serial.println("------------------------------------------------------------------------");
      Serial.println("Direita: Km/h: " + String(abs(velocidadeD)) + " RPM: " + String(abs(rpmD)));
      Serial.println("Esquerda: Km/h: " + String(abs(velocidadeE)) + " RPM: " + String(abs(rpmE)));
      Serial3.println(String(abs(velocidadeD)) + ',' + String(abs(velocidadeE)) + ',' + String(abs(rpmD)) + ',' + String(abs(rpmE)) + ',' + tempo);
    }
    posicaoAnteriorD = posicaoD;
    posicaoAnteriorE = posicaoE;
  }
}
