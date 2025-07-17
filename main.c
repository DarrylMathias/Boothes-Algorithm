#include <stdio.h>
#include <stdlib.h>
#define SIZE 8

void filler(int *arr)
{
    for (int i = 0; i < SIZE; i++)
    {
        *(arr + i) = 0;
    }
}

void reverse(int *arr)
{
    for (int i = 0; i < SIZE / 2; i++)
    {
        int temp = *(arr + i);
        *(arr + i) = *(arr + SIZE - i - 1);
        *(arr + SIZE - i - 1) = temp;
    }
}

void decimalToBinary(int num, int *numArr)
{
    filler(numArr);
    for (int i = 0; num > 0; i++)
    {
        *(numArr + i) = num % 2;
        num /= 2;
    }
    reverse(numArr);
}

int binaryToDecimal(int *number){
    int decimal = 0;
    for(int i=0; i<SIZE*2; i++){
        decimal = decimal*2 + *(number+i);
    }
    return decimal;
}

void printArray(int *arr)
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", *(arr + i));
    }
    printf("\n");
}

void complement1(int *target, int *num)
{
    for (int i = 0; i < SIZE; i++)
    {
        *(target + i) = *(num + i) == 0 ? 1 : 0;
    }
}

void binaryAdder(int *ans, int *num1, int *num2)
{
    int carry = 0;
    filler(ans);
    for (int i = SIZE - 1; i >= 0; i--)
    {
        int a = *(num1 + i);
        int b = *(num2 + i);

        int case000 = (a == 0 && b == 0 && carry == 0);                         // 00 with carry 0
        int case001 = (a == 0 && b == 0 && carry == 1);                         // 00 with carry 1
        int case010 = ((a == 0 && b == 1) || (a == 1 && b == 0)) && carry == 0; // 01 or 10 with carry 0
        int case011 = ((a == 0 && b == 1) || (a == 1 && b == 0)) && carry == 1; // 01 or 10 with carry 1
        int case110 = (a == 1 && b == 1 && carry == 0);                         // 11 with carry 0
        int case111 = (a == 1 && b == 1 && carry == 1);                         // 11 with carry 1

        if (case000)
        {
            *(ans + i) = 0;
            carry = 0;
        }
        else if (case001 || case010)
        {
            *(ans + i) = 1;
            carry = 0;
        }
        else if (case110 || case011)
        {
            *(ans + i) = 0;
            carry = 1;
        }
        else if (case111)
        {
            *(ans + i) = 1;
            carry = 1;
        }
        else
        {
            printf("Error\n");
        }
    }
}

void rightShift(int *a, int *q, int *q_1)
{
    *(q_1) = *(q + SIZE - 1);
    for (int i = SIZE - 2; i >= 0; i--)
    {
        *(q + i + 1) = *(q + i);
    }
    *q = *(a + SIZE - 1);
    for (int i = SIZE - 2; i >= 0; i--)
    {
        *(a + i + 1) = *(a + i);
    }
}

int main()
{
    // Take q and m
    int qInt, mInt;
    printf("Enter Q ");
    scanf("%d", &qInt);
    printf("Enter M ");
    scanf("%d", &mInt);

    // Convert to binary arrays
    int q[SIZE], m[SIZE];
    decimalToBinary(qInt, &q[0]);
    decimalToBinary(mInt, &m[0]);

    // Compute 1s complement
    int q1scomp[SIZE], m1scomp[SIZE];
    complement1(&q1scomp[0], &q[0]);
    complement1(&m1scomp[0], &m[0]);

    // Compute 2s complement
    int q2scomp[SIZE], m2scomp[SIZE];
    int binary1[] = {0, 0, 0, 0, 0, 0, 0, 1};
    binaryAdder(&q2scomp[0], &q1scomp[0], &binary1[0]);
    binaryAdder(&m2scomp[0], &m1scomp[0], &binary1[0]);

    // Initialise accumulator and q-1
    int a[SIZE];
    int q_1 = 0;
    filler(&a[0]);

    // Main logic
    for (int i = 0; i < SIZE; i++)
    {
        printf("---------\n");
        printf("Cycle %d\n", i + 1);
        printf("A is ");
        printArray(&a[0]);
        printf("q is ");
        printArray(&q[0]);
        printf("Q-1 is %d\n", q_1);
        printf("M is ");
        printArray(&m[0]);
        printf("---------\n");

        int bit1 = q[SIZE - 1], bit2 = q_1;
        if (bit1 == 1 && bit2 == 0)
        {
            // Using temp to prevent override
            int temp[SIZE];
            binaryAdder(&temp[0], &a[0], &m2scomp[0]);
            for (int i = 0; i < SIZE; i++)
            {
                a[i] = temp[i];
            }
            rightShift(&a[0], &q[0], &q_1);
        }
        else if (bit1 == 0 && bit2 == 1)
        {
            // Using temp to prevent override
            int temp[SIZE];
            binaryAdder(&temp[0], &a[0], &m[0]);
            for (int i = 0; i < SIZE; i++)
            {
                a[i] = temp[i];
            }
            rightShift(&a[0], &q[0], &q_1);
        }
        else
        {
            rightShift(&a[0], &q[0], &q_1);
        }
    }

    // Result
    printf("Final product in binary is ");
    int product[SIZE*2];
    for (int i = 0; i < 2 * SIZE; i++)
    {
        (i < SIZE) ? printf("%d ", a[i]) : printf("%d ", q[i - SIZE]);
        product[i] = (i<SIZE) ? a[i] : q[i-SIZE];
    }
    printf("\n");

    int decimal = binaryToDecimal(&product[0]);
    printf("In decimal, %d", decimal);

    return 0;
}