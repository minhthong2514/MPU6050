#ifndef MyMPU6050_h
#define MyMPU6050_h
#include <Wire.h>

// Create construction 
class MyMPU6050{
  public:
  MyMPU6050();
  // create variables, functions
  void begin();
  void calibrate();
  void update();
  float getRoll();
  float getPitch();
  float getYaw();

  private:
  void MPU6050DATA();
  void calculateAngles();
  const int MPU = 0x68;

  unsigned long prevTime, currentTime;
  double dt;

  int16_t accXRaw, accYRaw, accZRaw;
  int16_t gyroXRaw, gyroYRaw, gyroZRaw;
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;


  float roll, pitch, yaw;

  float gyroX_Offset, gyroY_Offset, gyroZ_Offset;
  float gyroX_Total, gyroY_Total, gyroZ_Total;
  int c;
};

#endif