#include <Wire.h>
#include <MPU6050.h>
#include <PID_v1.h>

MPU6050 mpu;

// L298N pins
#define ENA 5    
#define IN1 2    
#define IN2 3    

#define ENB 6    
#define IN3 10   
#define IN4 11   

// PID variables
double setpoint = 0;
double input, output;
double Kp = 60, Ki = 0.0, Kd = 60;

PID balancePID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

float accAngle = 0, gyroAngle = 0, currentAngle = 0;
float elapsedTime, previousTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 Error!");
    while (1);
  }

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  balancePID.SetMode(AUTOMATIC);
  balancePID.SetOutputLimits(-120, 120);
}

void loop() {

  // ========= MPU6050 DATA =========
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accAngleY = atan2(-ax, az) * 180 / PI;
  float gyroRateY = gy / 131.0;

  float currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  if (elapsedTime <= 0) elapsedTime = 0.001;

  // ========= COMPLEMENTARY FILTER =========
  currentAngle = 0.98 * (currentAngle + gyroRateY * elapsedTime) +
                 0.02 * accAngleY;

  input = currentAngle;

  // ========= PID =========
  balancePID.Compute();

  double finalOutput = output;   // ← NO BLUETOOTH CONTROL

  // ========= MOTOR CONTROL =========
  moveMotors(finalOutput);

  // Debug
  Serial.print("AngleY: ");
  Serial.print(currentAngle);
  Serial.print("   Output: ");
  Serial.println(finalOutput);
}

void moveMotors(double pidOutput) {

  if (pidOutput > 5) {
    backward((int)pidOutput);
  }
  else if (pidOutput < -5) {
    forward((int)(-pidOutput));
  }
  else {
    stopMotors();
  }
}

void forward(int speedVal) {
  speedVal = constrain(speedVal, 0, 120);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speedVal);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speedVal);
}

void backward(int speedVal) {
  speedVal = constrain(speedVal, 0, 120);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speedVal);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speedVal);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
