/* Author: Liu Sirui 刘思锐; ID: 3200102708; No.1 */

#include <stdio.h>
#include <stdlib.h>

//load a sequence with M elements
//return 1 if the sequence is available from stack arr
//return 0 if not
int checkSeq(int M, int *arr)
{
    int newArr[M], numArr[M], front = 0, flag = 1, top = M - 1;

    //creat a same new array
    for (int i = 0; i < M; i++)
        newArr[i] = arr[i];

    //load the sequence to check
    for (int i = 0; i < M; i++)
        scanf("%d", &numArr[i]);

    //start check
    for (int i = 0; i < M; i++)
    {
        if (newArr[0] == numArr[i])
        { //pop from stack base
            for (int k = 0; k < top; k++)
                newArr[k] = newArr[k + 1]; //move the elements behind
            top--;                         //move top
            if (top < front)
                front = top; //move front
        }
        else
        {
            for (int j = top; j >= front; j--)
            {
                if (newArr[j] == numArr[i])
                {                                //pop from stack top
                    front = (j > 0 ? j - 1 : 0); //move front
                    for (int k = j; k < top; k++)
                        newArr[k] = newArr[k + 1]; //move the elements behind
                    top--;                         //move top
                    break;
                }
                else if (j == front)
                { //canot pop
                    flag = 0;
                    break;
                }
            }
        }
        if (!flag)
            return 0;
    }
    return 1;
}

int main()
{
    int M, repeat;
    scanf("%d %d ", &M, &repeat);
    int arr[M];

    //initialize
    for (int i = 0; i < M; i++)
        scanf("%d", &arr[i]);

    //start to check input sequences
    for (int i = 0; i < repeat; i++)
    {
        if (checkSeq(M, arr))
        {
            printf("yes\n");
        }
        else
        {
            printf("no\n");
        }
    }

    return 0;
}