# Projeto de Microcontroladores

Este projeto foi desenvolvido pelos alunos Matheus Fernandes Dutra, Matheus Bueno Julião e Pedro Antero Simas de Oliveira como trabalho final da disciplina Microcontroladores, oferecida pela PUC-Rio no período letivo 2026.1.

O objetivo do projeto foi desenvolver um veículo de controle remoto capaz de percorrer um circuito composto por diferentes obstáculos, projetados pelos grupos da disciplina ao longo do semestre. Para isso, foi necessário integrar sistemas de controle, comunicação sem fio e monitoramento em tempo real, resultando em um veículo capaz de realizar o percurso de forma eficiente e fornecer ao operador informações detalhadas sobre seu funcionamento.

## Funcionamento do Projeto

O sistema é composto por duas unidades principais: o **Veículo** e o **Controlador**, cada um baseado em um Arduino Mega 2560. A comunicação entre os dois dispositivos é realizada via Bluetooth, permitindo o envio de comandos ao veículo e o retorno de informações ao operador.

### Veículo

O veículo é equipado com os seguintes componentes:
- Arduino Mega 2560;
- Módulo Bluetooth;
- Shield para controle dos motores;
- Dois motores DC com encoder;
- Três rodas (duas rodas motrizes e uma roda frontal livre);
- Servo motor para acionamento de uma garra;
- LED indicador;
- Estruturas mecânicas impressas em 3D.

Os motores com encoder permitem o controle da movimentação e a obtenção de informações de velocidade, enquanto o servo motor é responsável pelo acionamento da garra utilizada durante o percurso.

### Controlador

O controlador é composto pelos seguintes elementos:

- Arduino Mega 2560;
- Módulo Bluetooth;
- Shield com display LCD;
- Potenciômetro para controle do ângulo de direção (esquerda e direita);
- Joystick para controle da velocidade e do sentido de deslocamento (frente e ré);
- Botão para acionamento da garra instalada no veículo.

Por meio desses dispositivos de entrada, o operador controla todos os movimentos do veículo em tempo real.

## Monitoramento em Tempo Real

Além de receber os comandos enviados pelo controlador, o veículo transmite continuamente informações de funcionamento para serem exibidas no display LCD. A interface foi organizada em duas áreas principais:
- **Lado esquerdo:** Apresenta a velocidade atual do veículo, o tempo decorrido desde o início do percurso, um gráfico da velocidade em função do tempo e um indicador visual da direção do veículo, mostrando o sentido da curva (esquerda ou direita), o comando de direção aplicado e a velocidade instantânea.
- **Lado direito:** Exibe informações detalhadas sobre as medições realizadas pelos sensores, bem como o estado de diferentes variáveis internas utilizadas pelo sistema de controle, auxiliando no monitoramento e no processo de debug do funcionamento do veículo.

Essa interface fornece ao operador uma visão completa do estado do sistema durante toda a execução do percurso.

## Vídeo de Demonstração

O funcionamento completo do projeto pode ser visto no vídeo disponível no link abaixo:

**Link:** XXX
