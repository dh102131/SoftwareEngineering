#include <stdio.h>
#include "controller.h"

// 상태: 0 = 전진, 1 = 후진, 2 = 최종 정지
int state = 0;

int frontObstacle = 0;
int leftObstacle = 0;
int rightObstacle = 0;
int backObstacle = 0;


int FrontSensorInterface(int value)
{
    return value;
}

int LeftSensorInterface(int value)
{
    return value;
}

int RightSensorInterface(int value)
{
    return value;
}

int BackSensorInterface(int value)
{
    return value;
}

int DustSensorInterface(int value)
{
    return value;
}

void DetermineObstacleLocation(int front, int left, int right, int back)
{
    frontObstacle = FrontSensorInterface(front);
    leftObstacle = LeftSensorInterface(left);
    rightObstacle = RightSensorInterface(right);
    backObstacle = BackSensorInterface(back);
}

int DetermineDustExistence(int dust)
{
    return DustSensorInterface(dust);
}

// 모터 명령: 0 = 정지, 1 = 전진, 2 = 좌회전, 3 = 우회전, 4 = 후진
void MotorInterface(int command)
{
    if (command == 0) {
        printf("Motor: Stop\n");
    }
    else if (command == 1) {
        printf("Motor: Forward\n");
    }
    else if (command == 2) {
        printf("Motor: Left\n");
    }
    else if (command == 3) {
        printf("Motor: Right\n");
    }
    else if (command == 4) {
        printf("Motor: Backward\n");
    }
}

void MoveForward(int enable)
{
    if (enable == 1) {
        printf("Move Forward\n");
        MotorInterface(1);
    }
    else {
        printf("Move Forward: Disable\n");
    }
}

void MoveBackward(int enable)
{
    if (enable == 1) {
        printf("Move Backward\n");
        MotorInterface(4);
    }
    else {
        printf("Move Backward: Disable\n");
    }
}

void TurnLeft(void)
{
    int i;

    printf("Turn Left: Trigger\n");
    MotorInterface(2);

    for (i = 1; i <= 5; i++) {
        printf("Turn Left 중... (%d/5 Tick)\n", i);
    }

    printf("좌회전 완료\n");
}

void TurnRight(void)
{
    int i;

    printf("Turn Right: Trigger\n");
    MotorInterface(3);

    for (i = 1; i <= 5; i++) {
        printf("Turn Right 중... (%d/5 Tick)\n", i);
    }

    printf("우회전 완료\n");
}

void Stop(void)
{
    printf("Stop\n");
    MotorInterface(0);
}

// 청소 명령: 0 = Off, 1 = On, 2 = Up
void CleanerInterface(int command)
{
    static int previousCommand = -1;

    // 커맨드 바뀔 때만 출력
    if (command != previousCommand) {
        if (command == 0) {
            printf("Cleaner: Off\n");
        }
        else if (command == 1) {
            printf("Cleaner: On\n");
        }
        else if (command == 2) {
            printf("Cleaner: Up\n");
        }

        previousCommand = command;
    }
}

void PowerUp(void)
{
    int i;

    printf("Power Up: Trigger\n");
    CleanerInterface(2);

    for (i = 1; i <= 5; i++) {
        printf("Power Up 중... (%d/5 Tick)\n", i);
    }

    printf("Power Up 완료\n");
}

void CleanControl(int enable, int dust)
{
    if (enable == 0) {
        CleanerInterface(0);
    }
    else {
        if (dust == 1) {
            PowerUp();
        }

        // 일반 청소, Power Up 완료 후 기본 출력으로 복귀
        CleanerInterface(1);
    }
}

void Controller(int front, int left, int right, int back, int dust)
{
    int dustExists;

    DetermineObstacleLocation(front, left, right, back);
    dustExists = DetermineDustExistence(dust);

    if (state == 2) {
        Stop();
        CleanControl(0, 0);
        printf("Final Stop 상태입니다.\n");
        return;
    }

    // 후진 중일 때
    if (state == 1) {
        CleanControl(0, 0);

     
        if (leftObstacle == 0) {
            MoveBackward(0);
            TurnLeft();

            state = 0;
            MoveForward(1);
            CleanControl(1, 0);

        }
        else if (rightObstacle == 0) {
            MoveBackward(0);
            TurnRight();

            state = 0;
            MoveForward(1);
            CleanControl(1, 0);

        }
        else if (backObstacle == 1) {
            MoveBackward(0);
            Stop();

            state = 2;
            printf("후진 중 모든 방향이 막힘. : Final Stop\n");
        }
        else {
            MoveBackward(1);
        }

        return;
    }

    //전진 중 앞이 열려 있으면 전진 유지
    if (frontObstacle == 0) {
        MoveForward(1);
        CleanControl(1, dustExists);

    }
    else {
        MoveForward(0);
        CleanControl(0, 0);

        if (leftObstacle == 0) {
            TurnLeft();

            state = 0;
            MoveForward(1);
            CleanControl(1, 0);

        }
        else if (rightObstacle == 0) {
            TurnRight();

            state = 0;
            MoveForward(1);
            CleanControl(1, 0);

        }
        else {
            // 앞,양옆이 막히면 정지 후 뒤를 확인.
            Stop();

            if (backObstacle == 1) {
                state = 2;
                printf("모든 방향이 막힘: Final Stop\n");
            }
            else {
                state = 1;
                MoveBackward(1);
            }
        }
    }
}