/* Author: Liu Sirui 刘思锐; ID: 3200102708; No.2 */

#include <stdio.h>
#include <stdlib.h>

struct Bonus2
{
    int data;
    int serial;
};

//sort arr[N] accoring to Bonus2.data
void Sort(struct Bonus2 *arr, int N)
{
    struct Bonus2 temp;
    //bubble sort
    //quick sort is better but bubble sort is still within the time limit of PTA
    for (int i = 1; i < N; i++)
    {
        for (int j = 0; j < N - i; j++)
        {
            if (arr[j].data > arr[j + 1].data)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int N, temp;
    scanf("%d", &N);
    struct Bonus2 hat[N], PTAer[N], tempStruct;

    for (int i = 0; i < N; i++)
    { //input hat stack
        scanf("%d", &temp);
        hat[i].data = temp;
        hat[i].serial = N - i - 1; //output order from 0 to N-1, FILO
    }
    for (int i = 0; i < N; i++)
    { //input PTAers' queue
        scanf("%d", &temp);
        PTAer[i].data = temp;
        PTAer[i].serial = i + 1; //position of each PTAer from 1 to N
    }

    Sort(hat, N);
    Sort(PTAer, N);

    //hat[p].serial==k means you need to output PTAer[p].data at the k_th position
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (hat[j].serial == i)
            {
                if (i == N - 1)
                    printf("%d", PTAer[j].serial); //no blank space after the last output
                else
                    printf("%d ", PTAer[j].serial);
                break;
            }
        }
    }

    return 0;
}
