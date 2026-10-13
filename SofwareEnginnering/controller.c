#define _CRT_SECURE_NO_WARNINGS
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
int PowerUp(void)
{
    int i;
    int front, left, right, back, dust;

    printf("Power Up 시작\n");

    for (i = 1; i <= 5; i++) {
        if (i > 1) {
            printf("\nPower Up Tick %d 센서 입력", i);
            printf(" (앞 왼쪽 오른쪽 뒤 먼지): ");

            if (scanf("%d %d %d %d %d",
                &front, &left, &right, &back, &dust) != 5) {
                printf("입력 오류: Power Up 중단\n");
                CleanerInterface(0);
                return -1;
            }

            if ((front != 0 && front != 1)
                || (left != 0 && left != 1)
                || (right != 0 && right != 1)
                || (back != 0 && back != 1)
                || (dust != 0 && dust != 1)) {
                printf("센서 값은 0 또는 1이어야 합니다.\n");
                CleanerInterface(0);
                return -1;
            }

            DetermineObstacleLocation(front, left, right, back);
            DetermineDustExistence(dust);
        }

        // 매 틱마다 중간에 장애물을 만날 수 있는 상황을 고려하기 위함
        if (frontObstacle == 1) {
            printf("앞 장애물 감지: Power Up 취소\n");
            CleanerInterface(0);
            return 1;
        }

        MoveForward(1);
        CleanerInterface(2);

        printf("Power Up 중... (%d/5 Tick)\n", i);
    }

    printf("Power Up 완료\n");
    return 0;
}

int CleanControl(int enable, int dust)
{
    int result;

    if (enable == 0) {
        CleanerInterface(0);
        return 0;
    }

    if (dust == 1) {
        result = PowerUp();

        if (result != 0) {
            return result;
        }
    }

    CleanerInterface(1);
    return 0;
}

void Controller(int front, int left, int right, int back, int dust)
{
    int dustExists;
    int result;

    DetermineObstacleLocation(front, left, right, back);
    dustExists = DetermineDustExistence(dust);

    // state 0 = 전진, 1 = 후진, 2 = 최종 정지
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
            printf("양옆과 뒤가 막힘: Final Stop\n");

        }
        else {
            MoveBackward(1);
        }

        return;
    }

    // 전진 중일 때
    if (frontObstacle == 0) {
        if (dustExists == 0) {
            MoveForward(1);
        }

        result = CleanControl(1, dustExists);

        if (result == 0) {
            return;
        }

        if (result == -1) {
            MoveForward(0);
            Stop();
            state = 2;
            return;
        }
    }

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