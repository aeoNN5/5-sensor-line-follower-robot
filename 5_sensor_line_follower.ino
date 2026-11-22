// ===== 5-Sensor Line Follower - DYNAMIC SURFACE & AGGRESSIVE SMOOTH TURN (FINAL VERSION) =====

// Sensor Pins
const int S1 = 2; // Far left
const int S2 = 3; // Inner left
const int S3 = 4; // Center
const int S4 = 5; // Inner right
const int S5 = 6; // Far right

int s1, s2, s3, s4, s5;

// L298N Motor Driver Pins
const int enA = 11; // Left Motor Speed (PWM)
const int in1 = 12; // Left Motor Direction
const int in2 = 13; // Left Motor Direction

const int enB = 10; // Right Motor Speed (PWM)
const int in3 = 8;  // Right Motor Direction
const int in4 = 9;  // Right Motor Direction

// Surface Polarity Memory
bool IS_SURFACE_WHITE = true;

// ===== ALL SPEED PARAMETERS =====
const int NORMAL_STRAIGHT_SPEED = 140;
const int TURBO_STRAIGHT_SPEED  = 180;

// Gentle Curve Speeds
const int GENTLE_TURN_OUTER = 140;
const int GENTLE_TURN_INNER = 50;

// Sharp Curve Speeds (Aggressive Outer, Smooth Inner)
const int SHARP_TURN_OUTER = 230;
const int SHARP_TURN_INNER = 15;

// Active Speed & Straightaway Counter
int current_speed = NORMAL_STRAIGHT_SPEED;
int straight_counter = 0;
const int STRAIGHT_THRESHOLD_TIME = 250; // Reduce to 120-150 for shorter tracks

int last_direction = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(enA, OUTPUT); pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  pinMode(S1, INPUT); pinMode(S2, INPUT);
  pinMode(S3, INPUT); pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  // Startup safety: Stop motors immediately
  stopMotors();

  // Calibrate initial surface polarity and start immediately
  autoDetectSurface();
}

void loop()
{
  int raw_s1 = digitalRead(S1);
  int raw_s2 = digitalRead(S2);
  int raw_s3 = digitalRead(S3);
  int raw_s4 = digitalRead(S4);
  int raw_s5 = digitalRead(S5);

  // ===== DYNAMIC (REAL-TIME) SURFACE DETECTION =====
  if (raw_s1 == 0 && raw_s5 == 0) {
    IS_SURFACE_WHITE = false;
  }
  else if (raw_s1 == 1 && raw_s5 == 1) {
    IS_SURFACE_WHITE = true;
  }

  // ===== SOFTWARE SIGNAL INVERTER =====
  s1 = raw_s1; s2 = raw_s2; s3 = raw_s3; s4 = raw_s4; s5 = raw_s5;

  if (IS_SURFACE_WHITE == false) {
    s1 = !s1; s2 = !s2; s3 = !s3; s4 = !s4; s5 = !s5;
  }

  // ===== LINE FOLLOWING ALGORITHM (BLACK LINE = 0 LOGIC) =====

  // 0. INTERSECTION / HORIZONTAL LINE CROSSING GUARD
  if (s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0 && s5 == 0)
  {
    moveForward(NORMAL_STRAIGHT_SPEED);
  }
  // 1. STRAIGHT PATH & TURBO BOOST
  else if (s3 == 0 && s1 == 1 && s2 == 1 && s4 == 1 && s5 == 1)
  {
    straight_counter++;
    if (straight_counter >= STRAIGHT_THRESHOLD_TIME) current_speed = TURBO_STRAIGHT_SPEED;
    else current_speed = NORMAL_STRAIGHT_SPEED;

    moveForward(current_speed);
    last_direction = 0;
  }
  // 2. SHARP LEFT TURN
  else if (s1 == 0)
  {
    straight_counter = 0; turnSharpLeft(); last_direction = 11;
  }
  // 3. GENTLE LEFT TURN
  else if (s2 == 0 && s1 == 1)
  {
    straight_counter = 0; turnGentleLeft(); last_direction = 1;
  }
  // 4. SHARP RIGHT TURN
  else if (s5 == 0)
  {
    straight_counter = 0; turnSharpRight(); last_direction = 22;
  }
  // 5. GENTLE RIGHT TURN
  else if (s4 == 0 && s5 == 1)
  {
    straight_counter = 0; turnGentleRight(); last_direction = 2;
  }
  // 6. LINE LOST RECOVERY (MEMORY-BASED)
  else if (s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1 && s5 == 1)
  {
    straight_counter = 0;
    if (last_direction == 11) turnSharpLeft();
    else if (last_direction == 1) turnGentleLeft();
    else if (last_direction == 22) turnSharpRight();
    else if (last_direction == 2) turnGentleRight();
    else moveForward(NORMAL_STRAIGHT_SPEED);
  }

  delay(5);
}

// ===== AUTONOMOUS SURFACE DETECTION FUNCTION =====
void autoDetectSurface()
{
  delay(2000);

  int outer_left = digitalRead(S1);
  int outer_right = digitalRead(S5);

  if (outer_left == 0 && outer_right == 0) {
    IS_SURFACE_WHITE = false;
  } else {
    IS_SURFACE_WHITE = true;
  }
}

// ===== MOTOR ACTUATION FUNCTIONS =====

void moveForward(int speed) {
  analogWrite(enA, speed); analogWrite(enB, speed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turnSharpLeft() {
  analogWrite(enA, SHARP_TURN_OUTER);
  analogWrite(enB, SHARP_TURN_INNER);

  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turnGentleLeft() {
  analogWrite(enA, GENTLE_TURN_OUTER); analogWrite(enB, GENTLE_TURN_INNER);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turnSharpRight() {
  analogWrite(enA, SHARP_TURN_INNER);
  analogWrite(enB, SHARP_TURN_OUTER);

  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turnGentleRight() {
  analogWrite(enA, GENTLE_TURN_INNER); analogWrite(enB, GENTLE_TURN_OUTER);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void stopMotors() {
  analogWrite(enA, 0); analogWrite(enB, 0);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}