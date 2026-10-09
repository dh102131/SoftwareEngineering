#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "controller.h"


/* 1. Power Up 중 장애물 마주쳤을 때의 처리 필요 (Critical!!)
   2. Move Backward 우선 순위 고민 필요 -> 방문 경로를 기억해야함 (추가 사항)
   3. 회전 방향 우선순위를 왼쪽으로 하면 반복이 일어날 수밖에 없음 -> 이래서 지아 누나의 Direction Random이 생긴 듯 (추가 사항)

   우선 1번은 중요 문제라 꼭 해결해야 함
   Power Up할 때는 Tick 마다 상태를 다시 받도록 하는 방법도 있음
   => Turn Left / Turn Right 할 때는 지금처럼 유지할지도 같이 고민
   */
int main(void)
{
    int front, left, right, back, dust;

    printf("입력 순서: 앞 왼쪽 오른쪽 뒤 먼지\n");
    printf("장애물·먼지 있음 = 1, 없음 = 0\n");
    printf("종료하려면 -1을 입력하세요.\n");

    while (1) {
        printf("\n센서 입력: ");

        if (scanf("%d", &front) != 1) {
            printf("숫자를 입력해야 합니다.\n");
            break;
        }

        if (front == -1) {
            break;
        }

        if (scanf("%d %d %d %d",
            &left, &right, &back, &dust) != 4) {
            printf("숫자 다섯 개를 입력해야 합니다.\n");
            break;
        }

        if ((front != 0 && front != 1)
            || (left != 0 && left != 1)
            || (right != 0 && right != 1)
            || (back != 0 && back != 1)
            || (dust != 0 && dust != 1)) {
            printf("0 또는 1만 입력하세요.\n");
        }
        else {
            Controller(front, left, right, back, dust);
        }
    }

    return 0;
}

