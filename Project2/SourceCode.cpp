#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define MISSING (-1)

//only used to print level order traversal
struct tree
{
    int element;
    struct tree *left;
    struct tree *right;
};

int N, traversalCount, postOrderCount;
vector<int> inOrder, preOrder, postOrder, tempPostOrder;
//to store the result
vector<int> inResult, preResult, postResult;
//numbers that not being used in each traversal, for node is numbered from 1 to N
vector<int> inMissingNumber, preMissingNumber, postMissingNumber;
//index where '-' is at in each traversal
vector<int> inMissingIndex, preMissingIndex, postMissingIndex;

//read a non-negative number from buffer separated by any character other than '0'~'9'
//return #define MISSING when encounter '-' with no figure in front of it
//exit(1) if no more figure 'till EOF
int ReadNumber()
{
    char temp, flag = 0, num = 0;
    while (true)
    {
        temp = getchar(); //get a new char
        if (temp >= '0' && temp <= '9')
        { //encounter a figure
            num *= 10;
            num += temp - '0';
            flag = 1;
        }
        else if (!flag && temp == '-')
        { //encounter a '-' with no figure before
            return MISSING;
        }
        else if (flag)
        { //encounter a separator
            return num;
        }
        else if (!flag && temp == EOF)
        { //no more figure
            printf("NO MORE FIGURE!");
            exit(1);
        }
    }
}

//load given traversal into corresponding vector
void Load()
{
    for (int i = 0; i < N; i++)
        inOrder.push_back(ReadNumber());
    for (int i = 0; i < N; i++)
        preOrder.push_back(ReadNumber());
    for (int i = 0; i < N; i++)
        postOrder.push_back(ReadNumber());
}

//find out which number is absent in three traversal vector
//store result into vector<int> xxxMissing
void GetMissingNumber()
{
    int inFlag, preFlag, postFlag;

    for (int i = 1; i <= N; i++)
    {
        inFlag = 0;
        preFlag = 0;
        postFlag = 0;
        for (int j = 0; j < N; j++)
        { //traverse from 1 to N, for node is numbered from 1 to N
            if (inOrder[j] == i && !inFlag)
                inFlag = 1;
            if (preOrder[j] == i && !preFlag)
                preFlag = 1;
            if (postOrder[j] == i && !postFlag)
                postFlag = 1;
        }
        //if not present, push i
        if (!inFlag)
            inMissingNumber.push_back(i);
        if (!preFlag)
            preMissingNumber.push_back(i);
        if (!postFlag)
            postMissingNumber.push_back(i);
    }
}

//store the index of element MISSING of each traversal into vector<int> xxxMissingIndex
void GetMissingIndex()
{
    for (int i = 0; i < N; i++)
    {
        if (inOrder[i] == MISSING)
            inMissingIndex.push_back(i);
        if (preOrder[i] == MISSING)
            preMissingIndex.push_back(i);
        if (postOrder[i] == MISSING)
            postMissingIndex.push_back(i);
    }
}

//check if tempPostOrder correspond with postOrder. if so, traversalCount++ and the traversal will be stored in vector<int> xxxResult
//once traversalCount>1, print "Impossible" and exit(1) because it means the result is not unique
void CheckPostOrder()
{
    for (int i = 0; i < N; i++)
    { //check if correspond
        if (postOrder[i] != MISSING && postOrder[i] != tempPostOrder[i])
            return;
    }

    if (traversalCount)
    { //not unique, Impossible
        printf("Impossible");
        exit(1);
    }
    else
    { //available result, save it
        traversalCount++;
        inResult.assign(inOrder.begin(), inOrder.end());
        preResult.assign(preOrder.begin(), preOrder.end());
        postResult.assign(tempPostOrder.begin(), tempPostOrder.end());
    }
}

//try to restore the post-order traversal according to in-order and pre-order traversal if it's possible
//temporary result will be stored in tempPostOrder
int GeneratePostOrder(int in_begin, int pre_begin, int post_begin, int length)
{
    int rootIndex, i, flag = 1, leftCnt, rightCnt;
    for (i = 0; i < length; i++)
    { //preOrder[pre_begin] must be the root element
        if (inOrder[i + in_begin] == preOrder[pre_begin])
        {
            rootIndex = i + in_begin;
            break;
        }
    }
    if (i >= length)
    { //cannot find root element in inOrder traversal, cannot be a tree
        return 0;
    }
    else
    { //root element is available in inOrder traversal
        postOrderCount++;
        tempPostOrder[post_begin + length - 1] = preOrder[pre_begin]; //save the result

        //try restore left sub-tree
        leftCnt = rootIndex - in_begin; //node number of left sub-tree
        if (leftCnt > 0)
            flag = GeneratePostOrder(in_begin, pre_begin + 1, post_begin, leftCnt);

        //try restore right sub-tree
        rightCnt = in_begin + length - rootIndex - 1; //node number of right sub-tree
        if (flag && rightCnt > 0)
            flag = GeneratePostOrder(rootIndex + 1, pre_begin + leftCnt + 1, post_begin + leftCnt, rightCnt);

        if (postOrderCount == N)
        { //have traversed all node and get a possible result
            CheckPostOrder();
            postOrderCount = 0; //don't check a same result multiple times in different levels of recursion
        }
        return flag;
    }
}

//use recursion to find all possible pre-order traversal sequence
void FillPreOrder()
{
    if (preMissingNumber.empty())
    { //have filled all MISSING places
        postOrderCount = 0;
        GeneratePostOrder(0, 0, 0, N);
    }
    else
    {
        int tempNumber, tempIndex;
        vector<int>::iterator iter;
        for (iter = preMissingNumber.begin(); iter < preMissingNumber.end(); iter++)
        {
            //preparation of recursion
            tempNumber = *iter;
            preMissingNumber.erase(iter);
            tempIndex = *(--preMissingIndex.end());
            preMissingIndex.pop_back();
            //fill preOrder traversal sequence
            preOrder[tempIndex] = tempNumber;
            //recursion
            FillPreOrder();
            //restore variables
            preMissingNumber.insert(iter, tempNumber);
            preMissingIndex.push_back(tempIndex);
        }
    }
}

//use recursion to find all possible in-order traversal sequence
void FillInOrder()
{
    if (inMissingNumber.empty())
    { //have filled all MISSING placed
        FillPreOrder();
    }
    else
    {
        int tempNumber, tempIndex;
        vector<int>::iterator iter;
        for (iter = inMissingNumber.begin(); iter < inMissingNumber.end(); iter++)
        {
            //preparation of recursion
            tempNumber = *iter;
            inMissingNumber.erase(iter);
            tempIndex = *(--inMissingIndex.end());
            inMissingIndex.pop_back();
            //fill inOrder traversal sequence
            inOrder[tempIndex] = tempNumber;
            //recursion
            FillInOrder();
            //restore
            inMissingNumber.insert(iter, tempNumber);
            inMissingIndex.push_back(tempIndex);
        }
    }
}

//creat a binary tree according to inResult and postResult
struct tree *CreatTree(int begin, int end)
{
    static int offset = 0;

    //exit of recursion
    if (begin > end)
        return 0;

    //postOrder[end] is the root of current tree
    //find the position of root in inorder array
    int rootElement, rootPosition;
    rootElement = postResult[end];
    for (rootPosition = begin + offset; rootPosition <= end + offset; rootPosition++)
        if (inResult[rootPosition] == rootElement)
            break;

    //creat new tree by recursion
    struct tree *root;
    root = new tree;
    root->element = rootElement;

    //rootPosition refer to the index of inOrder
    //begin and end refer to the index of postOrder
    //take care of this! What's the begin and end next time?
    offset++;
    root->right = CreatTree(rootPosition - offset + 1, end - 1);
    offset--;
    root->left = CreatTree(begin, rootPosition - offset - 1);

    return root;
}

//print level order traversal according to inResult and postResult
void PrintLevelOrder()
{
    struct tree *queue[N];
    struct tree *t = CreatTree(0, N - 1); //creat a tree
    int begin = 0, end = 0, tempend;
    queue[end++] = t;
    while (begin < end)
    {
        tempend = end;
        for (int i = begin; i < tempend && i < N; i++)
        {
            //print the element in a same level
            printf("%d ", queue[i]->element);
            //push the child node into queue[]
            //for queue[begin]~queue[end] must be in a same level, the child node must be in a same level as well
            if (queue[i]->left)
                queue[end++] = queue[i]->left;
            if (queue[i]->right)
                queue[end++] = queue[i]->right;
            begin = tempend;
        }
    }
    printf("\n");
}

void PrintResult()
{
    //print in-order traversal
    for (int i = 0; i < N; i++)
        printf("%d ", inResult[i]);
    printf("\n");
    //print pre-order traversal
    for (int i = 0; i < N; i++)
        printf("%d ", preResult[i]);
    printf("\n");
    //print post-order traversal
    for (int i = 0; i < N; i++)
        printf("%d ", postResult[i]);
    printf("\n");
    //print level order traversal
    PrintLevelOrder();
}

int main()
{
    //preparation
    scanf("%d", &N);
    tempPostOrder.resize(N);
    Load();
    GetMissingNumber();
    GetMissingIndex();
    //start to traverse all possible cases
    FillInOrder();

    if (traversalCount)
    {
        PrintResult();
    }
    else
    {
        printf("Impossible\n");
    }

    return 0;
}
