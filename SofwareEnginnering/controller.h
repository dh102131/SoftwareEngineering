#ifndef CONTROLLER_H
#define CONTROLLER_H

void Controller(int front, int left, int right, int back, int dust);
int FrontSensorInterface(int value);
int LeftSensorInterface(int value);
int RightSensorInterface(int value);
int BackSensorInterface(int value);
int DustSensorInterface(int value);
void DetermineObstacleLocation(int front, int left, int right, int back);
int DetermineDustExistence(int dust);
void MoveForward(int enable);
void MoveBackward(int enable);
void TurnLeft(void);
void TurnRight(void);
void Stop(void);
void MotorInterface(int command);
void CleanControl(int enable, int dust);
void PowerUp(void);
void CleanerInterface(int command);

#endif
