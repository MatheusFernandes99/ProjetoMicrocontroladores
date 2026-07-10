#include <GFButton.h>
#include <stdio.h>
#include <stdlib.h>
#include <ShiftDisplay.h>

//Componentes

int indexes[] = {3, 2, 1, 0};
ShiftDisplay display(6, 7, 5, COMMON_ANODE, 4, true, indexes);

GFButton sensorInicial (11); // Criando os sensores como botoes para chamar a funcao so uma vez
GFButton sensorFinal (12);
unsigned long tempoInicial;
unsigned long tempoFinal;

float distancia = 10; // Distancia qualquer que pode ser trocada

float velocidadeCarro; // Velocidade do carrinho calculada com o sensor

long velocidadeRadar; // Velocidade que o carrinho precisa alcancar

int sensorProximidadeBloco = 10; // Sensor para o desafio do bloco

int ledParada = 3; // Leds para verificar o bloco
int ledContinuar = 4;

//Funcoes Auxiliares

void setup() {
  Serial.begin(9600);

  sensorInicial.setPressHandler(pegaTempoInicial); //Tratando os sensores como botão para facilitar a leitura
  sensorFinal.setPressHandler(pegaTempoFinalECalculaVelocidade);

  display.update();

  randomSeed(analogRead(0)); //Inicializa o randomizador de numeros para diferenciar entre execucoes

  pinMode(sensorProximidadeBloco, INPUT);

  pinMode(ledParada, OUTPUT);
  pinMode(ledContinuar, OUTPUT);
  digitalWrite(ledParada, HIGH);
  digitalWrite(ledContinuar, LOW);

}

// Funcoes Auxiliares
void pegaTempoInicial(){
  tempoInicial = millis(); //Pegando o tempo que passa no primeiro sesor com millis
  Serial.print("Tempo Inicial: ");
  Serial.print(tempoInicial);
  Serial.println(" Milisegundos");
}

void pegaTempoFinalECalculaVelocidade(){
  tempoFinal = millis(); // Pegando o tempo que passa no segundo sensor com millis
  float tempoEmSegundos = (tempoFinal - tempoInicial) / 1000.0; // Traducao de Milisegundos para Segundos
  velocidadeCarro = distancia / tempoEmSegundos; // Velocidade do Carro em Metros por Segundos
  Serial.print("Velocidade do Carro: "); // Print da Velocidade do Carro
  Serial.print(velocidadeCarro);
  Serial.println(" Metros por segundo");
}


// Loop
void loop() {
  display.update(); //Atualizacoes dos sensores e do display
  sensorInicial.process();
  sensorFinal.process();

  // Vendo a leitura do serial para verificar se precisar de outro valor
  if(Serial.available() > 0){
    String textoPegarOutroValor = Serial.readStringUntil('\n'); // Pegar o texto para comparar
    textoPegarOutroValor.trim();
    if(textoPegarOutroValor == "outro"){ // Comparando para ver se o texto bate com o comando proposto
      velocidadeRadar = random(1, 11); // Escolhendo um valor aleatorio
      display.set(velocidadeRadar); // Colocar valor no display
      display.update();
    }
    else{
      Serial.println("Mensagem nao reconhecida");
    }
  }

  // Segundo Desafio
  if(digitalRead(sensorProximidadeBloco) == 0){ // Verificacao do estado do Sensor
    digitalWrite(ledParada, LOW); // Mudar os estados dos led caso veja o bloco na frente do sensor
    digitalWrite(ledContinuar, HIGH);
  }
  else{
    digitalWrite(ledParada, HIGH); // Muda os estados caso esteja sem o bloco
    digitalWrite(ledContinuar, LOW);

  }

}
