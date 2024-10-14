#include <Arduino.h>
#include "MyMPU6050.h"

MyMPU6050::MyMPU6050() {
  roll = 0;
  pitch = 0;
  yaw = 0;

  prevTime = micros();

  gyroX_Offset = gyroY_Offset = gyroZ_Offset = 0;
  gyroX_Total = gyroY_Total = gyroZ_Total = 0;
}

void MyMPU6050::begin() {
  Wire.begin();
  Wire.setClock(400000);        // Set tốc độ xung clock I2C là 400KHz
  Wire.beginTransmission(MPU);  // Bắt đầu truyền tín hiệu tới MPU6050
  Wire.write(0x6B);             // Truy cập vào thanh ghi nguồn
  Wire.write(0x00);             // Đặt thanh ghi về 0 để bật MPU6050
  Wire.endTransmission(true);   // Kết thúc truyền tín hiệu

  calibrate();  // Hiệu chỉnh cảm biến
}

void MyMPU6050::calibrate() {
  c = 0;
  int count = 2000;
  while (c < count) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);  // Thanh ghi bắt đầu của con quay hồi chuyển
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);

    gyroXRaw = (Wire.read() << 8) | Wire.read();
    gyroYRaw = (Wire.read() << 8) | Wire.read();
    gyroZRaw = (Wire.read() << 8) | Wire.read();

    gyroX_Total += gyroXRaw;
    gyroY_Total += gyroYRaw;
    gyroZ_Total += gyroZRaw;
    c++;
  }

  // Tính giá trị hiệu chỉnh
  gyroX_Offset = -gyroX_Total * 1.0 / count;
  gyroY_Offset = -gyroY_Total * 1.0 / count;
  gyroZ_Offset = -gyroZ_Total * 1.0 / count;
}

void MyMPU6050::update() {
  prevTime = currentTime;
  currentTime = micros();
  dt = (currentTime - prevTime) / 1000000.0;

  // Get data from MPU
  MPU6050DATA();

  // Calculating angle
  calculateAngles();
}

void MyMPU6050::MPU6050DATA() {
  // Read accel data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  accXRaw = (Wire.read() << 8) | Wire.read();
  accYRaw = (Wire.read() << 8) | Wire.read();
  accZRaw = (Wire.read() << 8) | Wire.read();

  accX = accXRaw / 16384.0;
  accY = accYRaw / 16384.0;
  accZ = accZRaw / 16384.0;

  // Read gyro data
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  gyroXRaw = (Wire.read() << 8) | Wire.read();
  gyroYRaw = (Wire.read() << 8) | Wire.read();
  gyroZRaw = (Wire.read() << 8) | Wire.read();

  gyroX = (gyroXRaw + gyroX_Offset) / 131.0;
  gyroY = (gyroYRaw + gyroY_Offset) / 131.0;
  gyroZ = (gyroZRaw + gyroZ_Offset) / 131.0;
}

void MyMPU6050::calculateAngles() {
  float accelX = atan(accY / sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / PI;
  float accelY = atan(-1 * accX / sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / PI;

  roll = 0.98 * (roll + gyroX * dt) + 0.02 * accelX;
  pitch = 0.98 * (pitch + gyroY * dt) + 0.02 * accelY;
  yaw += gyroZ * dt;
}


float MyMPU6050::getRoll() {
  return roll;
}

float MyMPU6050::getPitch() {
  return pitch;
}

float MyMPU6050::getYaw() {
  return yaw;
}