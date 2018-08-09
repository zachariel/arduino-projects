/*
 * Zachariel Norzagaray
 * zachariel.norzagaray@gmail.com
 */
#include <Servo.h>
Servo myservo;

// Right motor
#define ENB 6  // Controll speed
#define IN3 9  // Motor right 1
#define IN4 11 // Motor right 2

// Left motor
#define ENA 5
#define IN1 7
#define IN2 8

#define SERVO_PIN 3
#define SERVO_90 90

#define BLUETOOTH_LED 13
#define BLUETOOTH_RATE 9600

#define SPEED_MIN 0
#define SPEED_DEFAULT 150
#define SPEED_MAX 255
#define SPEED_FRAME 10
#define DISTANCE_MIN

bool bluetooth_state = LOW;
char bluetooth_command;
int current_speed = SPEED_DEFAULT;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

int Echo = A4;  
int Trig = A5; 

void servo_setup() {
  myservo.attach(SERVO_PIN);  
}

void servo_center() {
  myservo.write(SERVO_90);// move servos to center position -> 90° 
}

int distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance = Fdistance / 58; // Distance in centimeters
  return (int)Fdistance;  
}

void motors_rotate() {
  analogWrite(ENA, current_speed);
  analogWrite(ENB, current_speed);
  //Serial.println("Moving motors!");
}

void speed_increase() {
  if( current_speed < SPEED_MAX ) {
    current_speed += SPEED_FRAME;
  }
  if( current_speed > SPEED_MAX ) {
    current_speed = SPEED_MAX;
  }
  Serial.println("Speed increase");
  motors_rotate();
}

void speed_decrease() {
  if( current_speed > SPEED_MIN ) {
    current_speed -= SPEED_FRAME;
  }

  if( current_speed < SPEED_MIN ) {
    current_speed = SPEED_MIN;
  }
  Serial.println("Speed decrease");
  motors_rotate();
}
void bluetooth_setup() {
  pinMode(BLUETOOTH_LED, OUTPUT);
  Serial.begin(BLUETOOTH_RATE);
}

void bluetooth_led_toogle() {
  if( bluetooth_state == LOW ) {
    bluetooth_state = HIGH;
  } else {
    bluetooth_state = LOW;
  }
  
  digitalWrite(BLUETOOTH_LED, bluetooth_state);
}

void right_wheel_forward() {
  digitalWrite(IN3, LOW);      
  digitalWrite(IN4, HIGH);//Right wheel turning forwards
}

void right_wheel_backward() {
  digitalWrite(IN3, HIGH);      
  digitalWrite(IN4, LOW); //Right wheel turning backwards  
}

void right_wheel_stop() {
  digitalWrite(IN3, LOW);      
  digitalWrite(IN4, LOW); //Right wheel stoped  
}

void right_motor_enable() {
  pinMode(IN3, OUTPUT); //set IO pin mode OUTPUT
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void left_motor_enable() {
  pinMode(IN1, OUTPUT);   //set IO pin mode OUTPUT
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA, HIGH);//Enable left motor       
}

void left_wheel_forward() {
  digitalWrite(IN1, HIGH);//Right wheel turning forwards
  digitalWrite(IN2, LOW);      
}

void left_wheel_backward() {
  digitalWrite(IN1, LOW); //Right wheel turning backwards  
  digitalWrite(IN2, HIGH);      
}

void left_wheel_stop() {
  digitalWrite(IN1, LOW);      
  digitalWrite(IN2, LOW); //Right wheel stoped  
}

void stop_moving() {
  left_wheel_stop();
  right_wheel_stop();
  Serial.println("Stop!");
}

void move_forward() {
  right_wheel_forward();
  left_wheel_forward();
  motors_rotate();
  Serial.println("Forward");
}

void move_backward() {
  right_wheel_backward();
  left_wheel_backward();
  motors_rotate();
  Serial.println("Backward");
}

void turn_left() {
  right_wheel_forward();
  left_wheel_backward();
  motors_rotate();
  Serial.println("Turn left");
}

void turn_right() {
  right_wheel_backward();
  left_wheel_forward();
  motors_rotate();
  Serial.println("Turn right");
}

void distance_left() {
  myservo.write(180);              
  delay(1000); 
  return distance_test();
}

void distance_right() {
  my_servo.write(10);
  delay(1000);
  return distance_test();
}

void setup() {
  // put your setup code here, to run once:
  bluetooth_setup();
  servo_setup();
}

void loop() {
  servo_center();
  delay(500); 
  middleDistance = distance_test();

  if( distance_test() <= DISTANCE_MIN ) {
    choose_left_or_right();
  }

  bluetooth_command = Serial.read();
  switch( bluetooth_command ) {
    case 'a': bluetooth_led_toogle(); break;
    case 'i': speed_increase(); break;
    case 'd': speed_decrease(); break;
    case 'f': move_forward(); break;
    case 'b': move_backward(); break;
    case 'l': turn_left(); break;
    case 'r': turn_right(); break;
    case 's': stop_moving(); break;
    case 'c': servo_center(); break;
  }
}
