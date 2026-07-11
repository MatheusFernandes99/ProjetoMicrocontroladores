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

<img width="836" height="666" alt="Captura de tela 2026-07-09 211935" src="https://github.com/user-attachments/assets/a1b6a1a5-4232-4c5e-84f7-1d47bf3701d5" />

Os motores com encoder permitem o controle da movimentação e a obtenção de informações de velocidade, enquanto o servo motor é responsável pelo acionamento da garra utilizada durante o percurso.

### Controlador

O controlador é composto pelos seguintes elementos:

- Arduino Mega 2560;
- Módulo Bluetooth;
- Shield com display LCD;
- Potenciômetro para controle do ângulo de direção (esquerda e direita);
- Joystick para controle da velocidade e do sentido de deslocamento (frente e ré);
- Botão para acionamento da garra instalada no veículo.

<img width="780" height="631" alt="Captura de tela 2026-07-09 211757" src="https://github.com/user-attachments/assets/0850fcc1-60b8-4fc2-9d7f-9bc20498dc41" />

Por meio desses dispositivos de entrada, o operador controla todos os movimentos do veículo em tempo real.

## Monitoramento em Tempo Real

Além de receber os comandos enviados pelo controlador, o veículo transmite continuamente informações de funcionamento para serem exibidas no display LCD. A interface foi organizada em duas áreas principais:
- **Lado esquerdo:** Apresenta a velocidade atual do veículo, o tempo decorrido desde o início do percurso, um gráfico da velocidade em função do tempo e um indicador visual da direção do veículo, mostrando o sentido da curva (esquerda ou direita), o comando de direção aplicado e a velocidade instantânea.
- **Lado direito:** Exibe informações detalhadas sobre as medições realizadas pelos sensores, bem como o estado de diferentes variáveis internas utilizadas pelo sistema de controle, auxiliando no monitoramento e no processo de debug do funcionamento do veículo.

Essa interface fornece ao operador uma visão completa do estado do sistema durante toda a execução do percurso.

## Obstáculos Desenvolvidos

Além do veículo e do controlador, o grupo desenvolveu dois obstáculos que foram utilizados durante a competição da disciplina para avaliar o desempenho dos veículos construídos pelos diferentes grupos.

### Radar Invertido de Velocidade

O **Radar Invertido de Velocidade** verifica se o veículo é capaz de atingir uma velocidade mínima durante um trecho do percurso.

Sempre que o Arduino é reiniciado, uma velocidade de referência é sorteada aleatoriamente. O sistema utiliza dois sensores de proximidade posicionados a uma distância conhecida entre si. Quando o veículo passa pelo primeiro sensor, o instante de passagem é registrado. Ao passar pelo segundo sensor, um novo instante é registrado e o tempo gasto para percorrer a distância entre os sensores é calculado.

<img width="1590" height="1877" alt="Desafio 1_bb" src="https://github.com/user-attachments/assets/7761b22c-4223-4226-b0fb-30077e86acbd" />

Com base nesses dados, a velocidade média do veículo é obtida pela razão entre a distância percorrida e o tempo de deslocamento. Caso a velocidade calculada seja superior à velocidade de referência sorteada, um LED verde é aceso, indicando sucesso no desafio. Caso contrário, um LED vermelho é acionado, indicando que o veículo não atingiu a velocidade mínima exigida.

### Detector de Pino

O **Detector de Pino** é um sistema responsável por verificar a presença de um objeto em uma posição específica do percurso.

O obstáculo utiliza um sensor de proximidade para detectar a presença de um bloco (pino). Enquanto nenhum objeto é detectado, um LED vermelho permanece aceso, indicando ausência do bloco. Quando o sensor identifica a presença do objeto, o LED vermelho é apagado e um LED verde é acionado, sinalizando que o bloco foi detectado corretamente.

<img width="1348" height="1391" alt="Desafio 2_bb" src="https://github.com/user-attachments/assets/40fd4a4c-a15f-4d43-b9a9-4dce8477fc1d" />

## Vídeo de Demonstração

O funcionamento completo do projeto pode ser visto no vídeo disponível no link abaixo:

**Link:** [Vídeo Projeto Microcontroladores 26.1](https://youtu.be/vwnJGxTPm50)
