int sensorProximidadeBloco = 10; // Sensor para o desafio do bloco

int ledParada = 3; // Leds para verificar o bloco
int ledContinuar = 4;
unsigned long tempoAnterior;

void setup() {
  Serial.begin(9600);
  pinMode(sensorProximidadeBloco, INPUT);

  pinMode(ledParada, OUTPUT);
  pinMode(ledContinuar, OUTPUT);
  digitalWrite(ledParada, HIGH);
  digitalWrite(ledContinuar, LOW);

}

void loop() {
  if(digitalRead(sensorProximidadeBloco) == 0){ // Verifica o Sensor
    delay(1000);
    digitalWrite(ledParada, LOW); // Mudar estados led com bloco
    digitalWrite(ledContinuar, HIGH);
    
  }
  else{
    digitalWrite(ledParada, HIGH); // Muda os estados caso esteja sem o bloco
    digitalWrite(ledContinuar, LOW);

  }

}
