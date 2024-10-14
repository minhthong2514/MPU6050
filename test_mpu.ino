#include <Arduino.h>
#include "MyMPU6050.h"

MyMPU6050 mpu;

void setup() {
  Serial.begin(115200);
  mpu.begin();
  mpu.calibrate();
}

void loop() {
  mpu.update();
  float roll = mpu.getRoll();
  float pitch = mpu.getPitch();
  float yaw = mpu.getYaw();

  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print("  |  Pitch: ");
  Serial.print(pitch);
  Serial.print("  |  Yaw: ");
  Serial.println(yaw);
}
