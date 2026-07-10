#include <math.h>
#include <string.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <GFButton.h>

#define MAX_POINTS 100

MCUFRIEND_kbv screen;

unsigned long instanteAnterior = 0;
unsigned long instanteAnterior1 = 0;
unsigned long instanteAnterior2 = 0;

// Potentiometer and joystick configuration variables:
int eixoX = A14;
int eixoY = A15;
int potentiometer = A13;

// Variables for the direction indicator plot:
int direction_screen_start_x = 65;
int direction_screen_start_y = 220;
int direction_screen_radius = 55;

// Additional variables:
int previous_angle;
int previous_js_y_pos_mapped;

// Mapping variables:
int min_map_value = -255;
int max_map_value = 255;

// Variable to store car speed:
float speed = 0;
int speed_mapped = 0;

// Button pins:
GFButton claw_button(A10);
GFButton led_button(A9);
int led_button_pressed = 0;
int claw_button_pressed = 0;

// Variables to control claw and led state:
bool claw_open = true;
bool led_on = false;

// Arrays for the speed points of the speed plot:
int current_points[MAX_POINTS];
int previous_points[MAX_POINTS];
int points_count = 0;

String time = "00:00";

// For reference, the screen is 320 by 240 pixels;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  // Setting the button function:
  claw_button.setPressHandler(claw_button_pressed_function);
  led_button.setPressHandler(led_button_pressed_function);
  led_button.setReleaseHandler(led_button_release_function);

  // Initializing the screen as a black screen that is rotated 90 degrees:
  screen.begin(screen.readID());
  screen.setRotation(1);
  screen.fillScreen(TFT_BLACK);

  // Initializing potentiometer and joystick:
  pinMode(eixoX, INPUT);
  pinMode(eixoY, INPUT);
  pinMode(potentiometer, INPUT);

  // Drawing the direction indicator screen:

  // Drawing a circle and hiding the bottom half:
  screen.drawCircle(direction_screen_start_x, direction_screen_start_y, direction_screen_radius, TFT_WHITE);
  screen.fillRect(direction_screen_start_x - direction_screen_radius - 1, direction_screen_start_y, 2 * direction_screen_radius + 2, direction_screen_radius + 2, TFT_BLACK);

  // Drawing a white line to close the half circle:
  screen.drawLine(direction_screen_start_x - direction_screen_radius, direction_screen_start_y, direction_screen_start_x + direction_screen_radius, direction_screen_start_y, TFT_WHITE);

  // Drawing the left and right text:
  screen.setCursor(direction_screen_start_x - direction_screen_radius, direction_screen_start_y + 5);
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.print("DIR");
  screen.setCursor(direction_screen_start_x + direction_screen_radius - 15, direction_screen_start_y + 5);
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.print("ESQ");
}

void update_points_array(int new_point) {

  // Saving the current points before inserting a new one:
  memcpy(previous_points, current_points, sizeof(current_points));
	
	// Just adding the point if there is space:
  if (points_count < MAX_POINTS) {
    current_points[points_count] = new_point;
    points_count++;
		
	// Removing a point to add the new one if the array is full:
  } else {
    for (int i = 0; i < MAX_POINTS - 1; i++) {
      current_points[i] = current_points[i + 1];
    }

    current_points[MAX_POINTS - 1] = new_point;
  }
}

// Function to draw the speed and time text on top of the screen:
void draw_text(int start_x, int start_y, float speed, String time) {
  char speed_text[40];
  char time_text[40];
  char speed_str[8];

  dtostrf(speed, 0, 2, speed_str);

  // Appending the integer values to strings:
  sprintf(speed_text, "Velocidade: %s km/h", speed_str);
  sprintf(time_text, "Tempo:      %s", time.c_str());

  // Erasing the previous text to write the new one:
  screen.fillRect(start_x + 65, start_y, 80, 12, TFT_BLACK);
  screen.fillRect(start_x + 65, start_y + 12, 65, 12, TFT_BLACK);

  // Drawing the speed text:
  screen.setCursor(start_x, start_y);
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.print(speed_text);

  // Drawing the time text:
  screen.setCursor(start_x, start_y + 15);
  screen.print(time_text);
  //screen.setCursor(start_x + 65, start_y + 15);
  //screen.print(time);
}

void draw_speed_plot(int start_x, int start_y) {
  int end_x;
  int end_y;
  int lines_start_x;
  int lines_start_y;
  int lines_end_x;
  int lines_end_y;
  int length = sizeof(current_points);

  // Erasing the plot to draw the new one:
  //screen.fillRect(start_x - 4, start_y - 6, 120, 115, TFT_BLACK);

  // Drawing the line of the Y axis:
  end_x = start_x;
  end_y = start_y + 100;
  screen.drawLine(start_x, start_y, end_x, end_y, TFT_WHITE);

  // Drawing the arrow of the Y axis:
  screen.fillTriangle(start_x, start_y - 4, start_x + 2, start_y, start_x - 2, start_y, TFT_WHITE);

  // Drawing the text of Y axis:
  screen.setCursor(start_x + 6, start_y - 9);
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.print("V");

  // Drawing the line of the X axis:
  end_x = start_x + 100;
  end_y = start_y + 100;
  screen.drawLine(start_x, start_y + 100, end_x, end_y, TFT_WHITE);

  // Drawing the arrow of the X axis:
  screen.fillTriangle(end_x + 4, end_y, end_x, end_y + 2, end_x, end_y - 2, TFT_WHITE);

  // Drawing the text of X axis:
  screen.setCursor(end_x + 7, end_y - 2);
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.print("T");

  // Erasing the previous line:
  lines_start_x = start_x;
  lines_start_y = start_y + 100;
  for (int i = 0; i < points_count; i++) {
    lines_end_y = start_y + 100 - previous_points[i];
    screen.drawLine(lines_start_x, lines_start_y, lines_start_x + 1, lines_end_y, TFT_BLACK);
    lines_start_x++;
    lines_start_y = lines_end_y;
  }

  // Drawing the line with the points:
  lines_start_x = start_x;
  lines_start_y = start_y + 100;
  for (int i = 0; i < points_count; i++) {
    lines_end_y = start_y + 100 - current_points[i];
    screen.drawLine(lines_start_x, lines_start_y, lines_start_x + 1, lines_end_y, TFT_YELLOW);
    lines_start_x++;
    lines_start_y = lines_end_y;
  }
}

void draw_direction_indicator_arrow(int start_x, int start_y, int angle, int js_y_pos_mapped, uint16_t color) {
  int end_x;
  int end_y;
  int line_length;
  int arrow_tip_x = start_x;
  int arrow_tip_y = start_y - direction_screen_radius + 1;
  int arrow_left_x = start_x - 3;
  int arrow_left_y = start_y - direction_screen_radius + 5;
  int arrow_right_x = start_x + 3;
  int arrow_right_y = start_y - direction_screen_radius + 5;
  float dx;
  float dy;
  float arrow_height = 5.0;           // Distance from the tip of the arrow to its base (the height);
  float arrow_base_half_width = 3.0;  // Half of the arrow's base width;
  float arrow_base_x;                 // Base x coordinate of the arrow;
  float arrow_base_y;                 // Base y coordinate of the arrow;
  double angle_rad;

  // Checking if the value of the joystick is larger than zero:
  if (js_y_pos_mapped < 0) {
    js_y_pos_mapped = 0;
  }

  line_length = direction_screen_radius * ((float)js_y_pos_mapped / (float)max_map_value) - 2;

  // Erasing the plot to draw the new one:
  //screen.fillRect(start_x - radius - 5, start_y - radius - 5, 120, 115, TFT_BLACK);

  // Calculating the end points of the line based on the angle:
  angle_rad = (angle / 180.0) * M_PI;
  dx = cos(angle_rad);
  dy = -sin(angle_rad);
  arrow_tip_x = (line_length * dx) + start_x;
  arrow_tip_y = (line_length * dy) + start_y;

  // Calculating base of the arrow:
  arrow_base_x = arrow_tip_x - arrow_height * dx;
  arrow_base_y = arrow_tip_y - arrow_height * dy;

  // Calculating the arrow's tip, left and right points:
  arrow_left_x = arrow_base_x + arrow_base_half_width * dx;
  arrow_left_y = arrow_base_y + arrow_base_half_width * (-dy);
  arrow_right_x = arrow_base_x - arrow_base_half_width * dx;
  arrow_right_y = arrow_base_y - arrow_base_half_width * (-dy);

  // Drawing the line with an arrow to indicate direction:
  // The arrow is only drawed if the value from the joystick is larger than zero;
  screen.drawLine(start_x, start_y, arrow_tip_x, arrow_tip_y, color);
  if (js_y_pos_mapped > 0) {
    screen.fillTriangle(arrow_tip_x, arrow_tip_y, arrow_left_x, arrow_left_y, arrow_right_x, arrow_right_y, color);
  }

  // Updating the variables to store the previous angles and joystick Y:
  previous_js_y_pos_mapped = js_y_pos_mapped;
  previous_angle = angle;
}

void draw_information_screen(int start_x, int start_y, int pot_value_mapped, int js_x_pos, int js_y_pos, int angle, int speed_mapped) {
  char joystick_x_text[40];
  char joystick_y_text[40];
  char potentiometer_text[40];
  char angle_text[40];
  char led_text[40];
  char claw_text[40];
  char speed_text[40];
  char led_state[4];
  char claw_state[10];

  if (led_on == true) {
    strcpy(led_state, "ON");
  } else {
    strcpy(led_state, "OFF");
  }

  if (claw_open == true) {
    strcpy(claw_state, "OPEN");
  } else {
    strcpy(claw_state, "CLOSED");
  }

  // Appending the input values to strings:
  sprintf(joystick_x_text, "Joystick X: %d", js_x_pos);
  sprintf(joystick_y_text, "Joystick Y: %d", js_y_pos);
  sprintf(potentiometer_text, "Potentiometer: %d", pot_value_mapped);
  sprintf(angle_text, "Angle: %d (degrees)", angle);
  sprintf(led_text, "LED State: %s", led_state);
  sprintf(claw_text, "Claw State: %s", claw_state);
  sprintf(speed_text, "Speed (Mapped): %d", speed_mapped);

  // Printing the title in the information screen:
  screen.setTextColor(TFT_WHITE);
  screen.setTextSize(1);
  screen.setCursor(start_x, start_y);
  screen.print("INFORMATION SCREEN");
  screen.drawLine(start_x, start_y + 10, start_x + 108, start_y + 10, TFT_WHITE);

  // Printing joystick information:
  screen.setCursor(start_x, start_y + 20);
  screen.fillRect(start_x + 65, start_y + 20, 40, 12, TFT_BLACK);
  screen.print(joystick_x_text);
  screen.setCursor(start_x, start_y + 32);
  screen.fillRect(start_x + 65, start_y + 32, 40, 12, TFT_BLACK);
  screen.print(joystick_y_text);

  // Printing potentiometer information:
  screen.setCursor(start_x, start_y + 44);
  screen.fillRect(start_x + 85, start_y + 44, 40, 12, TFT_BLACK);
  screen.print(potentiometer_text);

  // Printing angle information:
  screen.setCursor(start_x, start_y + 56);
  screen.fillRect(start_x + 33, start_y + 56, 90, 12, TFT_BLACK);
  screen.print(angle_text);

  // Printing LED information:
  screen.setCursor(start_x, start_y + 68);
  screen.fillRect(start_x + 68, start_y + 68, 40, 12, TFT_BLACK);
  screen.print(led_text);

  // Printing claw information:
  screen.setCursor(start_x, start_y + 80);
  screen.fillRect(start_x + 75, start_y + 80, 70, 12, TFT_BLACK);
  screen.print(claw_text);

  // Printing speed information:
  screen.setCursor(start_x, start_y + 92);
  screen.fillRect(start_x + 90, start_y + 92, 70, 12, TFT_BLACK);
  screen.print(speed_text);

  // TO DO: More of the debug screen;
}

void update_screen(float speed, int speed_mapped, int pot_value_mapped, int js_x_pos_mapped, int js_y_pos_mapped, int angle, String time) {
  int text_start_x = 10;
  int text_start_y = 10;
  int speed_plot_start_x = 10;
  int speed_plot_start_y = 50;
  int debug_start_x = 190;
  int debug_start_y = 10;

  // Drawing the white line to close the half circle of the direction screen in case it is erased:
  screen.drawLine(direction_screen_start_x - direction_screen_radius, direction_screen_start_y, direction_screen_start_x + direction_screen_radius, direction_screen_start_y, TFT_WHITE);

  // Drawing the line that separates both sides of the screen:
  screen.drawLine(160, 0, 160, 240, TFT_WHITE);

  // Drawing the contents of the screen:
  draw_text(text_start_x, text_start_y, speed, time);
  draw_speed_plot(speed_plot_start_x, speed_plot_start_y);
  draw_direction_indicator_arrow(direction_screen_start_x, direction_screen_start_y, previous_angle, previous_js_y_pos_mapped, TFT_BLACK);
  draw_direction_indicator_arrow(direction_screen_start_x, direction_screen_start_y, angle, js_y_pos_mapped, TFT_YELLOW);
  draw_information_screen(debug_start_x, debug_start_y, pot_value_mapped, js_x_pos_mapped, js_y_pos_mapped, angle, speed_mapped);
}

void claw_button_pressed_function() {
  //Serial.println("Claw button pressed!");

  if (claw_open == true) {
    Serial2.println("FECHAR");
    claw_open = false;
  } 
  else {
    Serial2.println("ABRIR");
    claw_open = true;
  }
}

void led_button_release_function() {
  Serial2.println("ACENDER");
}

void led_button_pressed_function() {
  //Serial.println("LED button pressed!");
  Serial2.println("APAGAR");
}

void loop() {

  // Starting the button process:
  led_button.process();
  claw_button.process();

  // Reading the value of the potentiometer and mapping it to a certain range:
  int pot_value_read = analogRead(potentiometer);
  int pot_value_mapped = map(pot_value_read, 0, 1023, min_map_value, max_map_value);
  int angle = map(pot_value_read, 0, 1023, 0, 180);

  // Reading the value of the joystick:
  int js_x_pos = analogRead(eixoX);
  int js_y_pos = analogRead(eixoY);
  int js_x_pos_mapped = map(js_x_pos, 0, 1023, min_map_value, max_map_value);
  int js_y_pos_mapped = map(js_y_pos, 0, 1023, min_map_value, max_map_value);

  // Variables to store comma positions of received message:
  int c1, c2, c3, c4;

  // Variables for received values:
  float velocidade_dir, velocidade_esq;
  float rpm_dir, rpm_esq;

  unsigned long instanteAtual;

  // Receiving information from the other arduino:
  if (Serial2.available()) {
    String msg = Serial2.readStringUntil('\n');
    Serial.println(msg);
    msg.trim();

    // Getting the positions of all three commas to get the values between them:
    c1 = msg.indexOf(',');
    c2 = msg.indexOf(',', c1 + 1);
    c3 = msg.indexOf(',', c2 + 1);
    c4 = msg.indexOf(',', c3 + 1);

    // Getting the values between the commas of the received message:
    velocidade_dir = msg.substring(0, c1).toFloat();
    velocidade_esq = msg.substring(c1 + 1, c2).toFloat();
    rpm_dir = msg.substring(c2 + 1, c3).toFloat();
    rpm_esq = msg.substring(c3 + 1, c4).toFloat();
    time = msg.substring(c4 + 1);
  
    // Total speed is the average of the sum of right and left wheels' speeds:
    speed = (velocidade_dir + velocidade_esq) / 2;
    speed_mapped = (int) (((velocidade_dir + velocidade_esq) / 2) * 100);
    speed_mapped = constrain(map(speed_mapped, 0, 120, 0, 100), 0, 100);
  }

  // Sending the control information every fraction of second:
  instanteAtual = millis();
  if (instanteAtual > instanteAnterior + 100) {

    // Sending the X and Y values:
    Serial2.print("ANDAR ");
    Serial2.println(js_y_pos_mapped);
    Serial2.print("DIRECAO ");
    Serial2.println(pot_value_mapped);

    //instanteAnterior1 = instanteAtual;
  }

  // Updating the screen information every half second:
  instanteAtual = millis();
  if (instanteAtual > instanteAnterior + 500) {
    update_points_array(speed_mapped);
    update_screen(speed, speed_mapped, pot_value_mapped, js_x_pos_mapped, js_y_pos_mapped, angle, time);
    //instanteAnterior2 = instanteAtual;
    instanteAnterior = instanteAtual;
  }
}
