#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "controller.h"

int main(void)
{
    int front, left, right, back, dust;
    printf("Enter Front Left Right Back Dust (0 or 1).\n");
    printf("One input line = one Tick. Enter -1 to quit.\n");
    while (1) {
        printf("\nFront Left Right Back Dust: ");
        if (scanf("%d", &front) != 1) break;
        if (front == -1) break;
        if (scanf("%d %d %d %d", &left, &right, &back, &dust) != 4) {
            printf("Input error: numbers required.\n");
            break;
        }
        if ((front != 0 && front != 1) || (left != 0 && left != 1)
            || (right != 0 && right != 1) || (back != 0 && back != 1) || (dust != 0 && dust != 1)) {
            printf("Use only 0 or 1. Tick was not advanced.\n");
        } else {
            Controller(front, left, right, back, dust);
        }
    }
    return 0;
}
