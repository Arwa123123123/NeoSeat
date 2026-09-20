#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>

Adafruit_MPU6050 mpu;
Servo servo;

const int SERVO_PIN = 18;
const int SERVO_CENTER = 90;
const int SERVO_MIN = 85;
const int SERVO_MAX = 95;
const int N = 10;

const float CENTROIDS[3][3] = {
  {0.093551, 0.041713, 0.100590},
  {0.143035, 0.055275, 0.374566},
  {1.656409, 0.388346, 4.720284}
};

const float SCALE[3] = {0.60, 0.14, 1.80};

const char* CLASS_NAMES[3] = {
  "STABLE",
  "MILD",
  "STRONG"
};

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!mpu.begin()) {
    Serial.println("MPU6050 NOT FOUND");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 500, 2500);
  servo.write(SERVO_CENTER);

  Serial.println("NeoSeat Adaptive ML READY");
}

void loop() {

  float gyroMag[N];
  float accMag[N];

  float sumG = 0;
  float sumA = 0;
  float maxG = 0;
  float sumTilt = 0;

  for (int i = 0; i < N; i++) {

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    gyroMag[i] = sqrt(
      g.gyro.x * g.gyro.x +
      g.gyro.y * g.gyro.y +
      g.gyro.z * g.gyro.z
    );

    accMag[i] = sqrt(
      a.acceleration.x * a.acceleration.x +
      a.acceleration.y * a.acceleration.y +
      a.acceleration.z * a.acceleration.z
    );

    float rawRoll =
      atan2(a.acceleration.y, a.acceleration.z)
      * 180.0 / PI;

    float tilt;

    if (rawRoll > 0)
      tilt = rawRoll - 180.0;
    else
      tilt = rawRoll + 180.0;

    sumTilt += tilt;

    sumG += gyroMag[i];
    sumA += accMag[i];

    if (gyroMag[i] > maxG)
      maxG = gyroMag[i];

    delay(50);
  }

  float meanG = sumG / N;
  float meanA = sumA / N;
  float tiltAngle = sumTilt / N;

  float varianceA = 0;

  for (int i = 0; i < N; i++) {
    float diff = accMag[i] - meanA;
    varianceA += diff * diff;
  }

  float stdA = sqrt(varianceA / N);

  float features[3] = {
    meanG,
    stdA,
    maxG
  };

  int bestClass = 0;
  float bestDistance = 999999;

  for (int c = 0; c < 3; c++) {

    float distance = 0;

    for (int f = 0; f < 3; f++) {

      float diff =
        (features[f] - CENTROIDS[c][f])
        / SCALE[f];

      distance += diff * diff;
    }

    if (distance < bestDistance) {
      bestDistance = distance;
      bestClass = c;
    }
  }

  int responseAmount = 0;

  if (bestClass == 0)
    responseAmount = 0;
  else if (bestClass == 1)
    responseAmount = 2;
  else
    responseAmount = 5;

  int servoAngle = SERVO_CENTER;

  if (abs(tiltAngle) > 1.5 &&
      responseAmount > 0) {

    if (tiltAngle > 0)
      servoAngle =
        SERVO_CENTER - responseAmount;
    else
      servoAngle =
        SERVO_CENTER + responseAmount;
  }

  servoAngle = constrain(
    servoAngle,
    SERVO_MIN,
    SERVO_MAX
  );

  servo.write(servoAngle);

  Serial.print("ML: ");
  Serial.print(CLASS_NAMES[bestClass]);

  Serial.print(" | Tilt: ");
  Serial.print(tiltAngle, 1);

  Serial.print(" | Servo: ");
  Serial.print(servoAngle);

  Serial.print(" | MeanG: ");
  Serial.print(meanG, 3);

  Serial.print(" | AccVar: ");
  Serial.print(stdA, 3);

  Serial.print(" | MaxG: ");
  Serial.println(maxG, 3);
}
