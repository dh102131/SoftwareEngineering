#include <stdio.h>
#include "controller.h"

/* State: 0=Forward, 1=Left, 2=Right, 3=Stop, 4=Backward, 5=Final Stop */
int state = 0;
int turnTick = 0;
int powerTick = 0;
int tick = 0;
int frontObstacle = 0;
int leftObstacle = 0;
int rightObstacle = 0;
int backObstacle = 0;

int FrontSensorInterface(int value)
{
    printf("Front sensor: %d\n", value);
    return value;
}
int LeftSensorInterface(int value)
{
    printf("Left sensor: %d\n", value);
    return value;
}
int RightSensorInterface(int value)
{
    printf("Right sensor: %d\n", value);
    return value;
}
int BackSensorInterface(int value)
{
    printf("Back sensor: %d\n", value);
    return value;
}
int DustSensorInterface(int value)
{
    printf("Dust sensor: %d\n", value);
    return value;
}
void DetermineObstacleLocation(int front, int left, int right, int back)
{
    printf("Determine obstacle location\n");
    frontObstacle = FrontSensorInterface(front);
    leftObstacle = LeftSensorInterface(left);
    rightObstacle = RightSensorInterface(right);
    backObstacle = BackSensorInterface(back);
}
int DetermineDustExistence(int dust)
{
    printf("Determine dust existence\n");
    return DustSensorInterface(dust);
}

/* Motor command: 0=Stop, 1=Forward, 2=Left, 3=Right, 4=Backward */
void MotorInterface(int command)
{
    if (command == 0) printf("Motor: Stop\n");
    else if (command == 1) printf("Motor: Forward\n");
    else if (command == 2) printf("Motor: Left\n");
    else if (command == 3) printf("Motor: Right\n");
    else if (command == 4) printf("Motor: Backward\n");
}
void MoveForward(int enable)
{
    if (enable == 1) {
        printf("Move forward: Enable\n");
        MotorInterface(1);
    } else {
        printf("Move forward: Disable\n");
    }
}
void MoveBackward(int enable)
{
    if (enable == 1) {
        printf("Move backward: Enable\n");
        MotorInterface(4);
    } else {
        printf("Move backward: Disable\n");
    }
}
void TurnLeft(void)
{
    printf("Turn left\n");
    MotorInterface(2);
}
void TurnRight(void)
{
    printf("Turn right\n");
    MotorInterface(3);
}
void Stop(void)
{
    printf("Stop\n");
    MotorInterface(0);
}

// 커맨드가 바뀔 때만 print
void CleanerInterface(int command)
{
    static int previousCommand = -1;
    if (command != previousCommand) {
        if (command == 0) printf("Cleaner: Off\n");
        else if (command == 1) printf("Cleaner: On\n");
        else if (command == 2) printf("Cleaner: Up\n");
        previousCommand = command;
    }
}
void PowerUp(void)
{
    printf("Power up (%d / 5 Tick)\n", powerTick + 1);
    CleanerInterface(2);
    powerTick = powerTick + 1;
    if (powerTick == 5) powerTick = 0;
}
void CleanControl(int enable, int dust)
{
    if (enable == 0) {
        printf("Clean control: Disable\n");
        if (powerTick > 0) printf("Power up cancelled\n");
        powerTick = 0;
        CleanerInterface(0);
    } else {
        printf("Clean control: Enable\n");
        if (dust == 1 || powerTick > 0) {
            PowerUp();
        } else {
            CleanerInterface(1);
        }
    }
}

void Controller(int front, int left, int right, int back, int dust)
{
    int previousState = state;
    int dustExists;
    tick = tick + 1;
    printf("\n--- Tick %d ---\n", tick);
    DetermineObstacleLocation(front, left, right, back);
    dustExists = DetermineDustExistence(dust);

// Turn 후 전진 여부 재계산 위해 초기화
    if ((state == 1 || state == 2) && turnTick == 5) {
        state = 0;
        turnTick = 0;
    }

    if (state == 0) {
        // dust나 Power up 전에 Turn
        if (frontObstacle == 1) {
            if (leftObstacle == 0) {
                state = 1;
                turnTick = 0;
                printf("Turn left: Trigger\n");
            } else if (rightObstacle == 0) {
                state = 2;
                turnTick = 0;
                printf("Turn right: Trigger\n");
            } else {
                state = 3;
            }
        }
    } else if (state == 3 || state == 4) {
        // 뒤 장애물 감지되거나 양옆 뚫리기 전까지 계속 후진 유지
        if (backObstacle == 1) {
            state = 5;
        } else if (leftObstacle == 0) {
            state = 1;
            turnTick = 0;
            printf("Turn left: Trigger\n");
        } else if (rightObstacle == 0) {
            state = 2;
            turnTick = 0;
            printf("Turn right: Trigger\n");
        } else {
            state = 4;
        }
    }

    if (previousState == 0 && state != 0) MoveForward(0);
    if (previousState == 4 && state != 4) MoveBackward(0);
    printf("State: %d -> %d\n", previousState, state);

    if (state == 0) {
        MoveForward(1);
    } else if (state == 1) {
        TurnLeft();
        turnTick = turnTick + 1;
    } else if (state == 2) {
        TurnRight();
        turnTick = turnTick + 1;
    } else if (state == 4) {
        MoveBackward(1);
    } else {
        if (state == 5) printf("Final stop\n");
        Stop();
    }

    if (state == 0) CleanControl(1, dustExists);
    else CleanControl(0, dustExists);
}
