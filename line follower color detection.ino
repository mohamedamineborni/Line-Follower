// Pin configurations
const int in1 = 3;
const int in2 = 4;
const int in3 = 5;
const int in4 = 6;
const int s0 = 7;
const int s1 = 8;
const int s2 = 9;
const int s3 = 10;
const int out = 11;
const int c1 = 12;
const int c2 = 13;
const int c3 = A0;
const int c4 = A1;

// Color detection variables
int red_val = 0;
int green_val = 0;
int blue_val = 0;
int data = 0;

// Motor control variables
int v1 = 0;
int v2 = 0;
int v3 = 0;
int v4 = 0;
int start = 0;
int turn = 0;

// Function to set the motor speeds for forward movement
void forward(int left_speed, int right_speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in3, left_speed);
  digitalWrite(in4, LOW);
  analogWrite(in2, right_speed);
}

// Function to set the motor speeds for right turn
void right(int left_speed, int right_speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in3, left_speed);
  digitalWrite(in4, HIGH);
  analogWrite(in2, right_speed);
}

// Function to set the motor speeds for left turn
void left(int left_speed, int right_speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(in3, left_speed);
  digitalWrite(in4, LOW);
  analogWrite(in2, right_speed);
}

// Function to detect the color of the object
void color() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  red_val = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  green_val = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  blue_val = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  if (red_val > green_val && red_val > blue_val) {
    data = 1; // Red detected
  }
}

// Function to detect the position of the object on the track
void detect() {
  v1 = digitalRead(c1); // wide left
  v2 = digitalRead(c2); // left
  v3 = digitalRead(c3); // right
  v4 = digitalRead(c4); // wide right

  // Serial monitor output
  Serial.print(v1);
  Serial.print(v2);
  Serial.print(v3);
  Serial.println(v4);

  if (data == 1) { // Detection of color red
    forward(0, 0); // Stop
    turn = 1;
    delay(5000); // Wait 5 seconds
    do {
      right(90, 90); // Flip to the right until touch v2==1 black
      v2 = digitalRead(c2); // left
    } while (v2 == 0);
  } else if (v1 == 0 && v2 == 0 && v3 == 1 && v4 == 0) { // Slightly turn to the right
    right(60, 60);
    forward(90, 90);
  } else if (v1 == 0 && v2 == 1 && v3 == 0 && v4 == 0) { // Hard turn to the right
    right(90, 90);
    forward(90, 90);
  } else if (v1 == 0 && v2 == 1 && v3 == 0 && v4 == 0) { // Slightly turn to the left
    left(60, 60);
    forward(90, 90);
  } else if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 1) { // Hard turn to the left
    left(90, 90);
    forward(90, 90);
  } else if (v1 == 1 && v2 == 1 && v3 == 1 && v4 == 1) { // All sensors on white
    forward(90, 90);
  } else if (v1 == 0 && v2 == 1 && v3 == 1 && v4 == 0) { // Intersection detected
    forward(0, 0); // Stop
    turn = 1;
    delay(5000); // Wait 5 seconds
    do {
      left(90, 90); // Flip to the left until touch v3==1 black
      v3 = digitalRead(c3); // right
    } while (v3 == 0);
  }
}

void setup() {
// Initialize serial communication
Serial.begin(9600);

// Set pin modes
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(s0, OUTPUT);
pinMode(s1, OUTPUT);
pinMode(s2, OUTPUT);
pinMode(s3, OUTPUT);
pinMode(out, INPUT);
pinMode(c1, INPUT);
pinMode(c2, INPUT);
pinMode(c3, INPUT);
pinMode(c4, INPUT);
}

void loop() {
if (turn == 0) { // Normal movement along the track
forward(90, 90);
detect(); // Detect the position of the object
} else { // Turning to avoid the robot
forward(0, 0); // Stop
delay(1000); // Wait for 1 second
turn = 0;
color(); // Detect the color of the object
}
}
