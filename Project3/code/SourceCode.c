#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxStrSize (30)
#define Infinity (1000000)
#define Invalid (-1)

struct AdjVer {
    int ver;//adjacent vertex (aka calling spots)
    int dist;//the distance to ver
    struct AdjVer *next;
};
struct Vertex {
    int *path;//path[i] stores the last step of the nearest way to A-i
    int *dist;//dist[i] stores the distance to A-i
    int *known;//used in dijkstra algorithm
    struct AdjVer *adjVerList;//
};
struct Graph {
    int numADC;//number of ambulance dispatch centers
    int numVer;//number of vertex
    int *avlAmb;//number of available ambulances in each center
    struct Vertex *verList;
    /*
     * ambulance dispatch centers are verList[1]~verList[numADC]
     * calling spots are verList[numADC+1]~verList[numADC+numVer]
     */
};

//tool function that convert a string which only contains '0'~'9' to a decimal integer
int StrToDec(char *str) {
    int rst = 0, len = strlen(str);
    for (int i = 0; i < len; i++) {
        rst *= 10;
        rst += str[i] - '0';
    }
    return rst;
}

//add a path v1->v2 with distance "dist" into a certain graph
void PathToMap(struct Graph *Map, int v1, int v2, int dist) {
    struct AdjVer *newNode, *ptr;
    //malloc new space
    newNode = calloc(1, sizeof(struct AdjVer));
    newNode->ver = v2;
    newNode->dist = dist;
    //insert new path
    if (Map->verList[v1].adjVerList == NULL) {
        Map->verList[v1].adjVerList = newNode;
    } else {
        ptr = Map->verList[v1].adjVerList;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = newNode;
    }
}

//change a string in certain format into an integer
int MyGetVer(struct Graph *Map) {
    char str[MaxStrSize];
    int temp = Invalid;
    scanf("%s", str);
    if (str[0] == 'A')
        temp = StrToDec(str + 2);
    else
        temp = StrToDec(str) + Map->numADC;

    return temp;
}

//load a path in a certain format, the details of input format are in my report
void LoadPath(struct Graph *Map) {
    int v1, v2, dist;

    v1 = MyGetVer(Map);
    v2 = MyGetVer(Map);
    dist = MyGetVer(Map) - Map->numADC;
    //undirected graph, path a mutual for vertexes
    PathToMap(Map, v1, v2, dist);
    PathToMap(Map, v2, v1, dist);
};

//return the nearest unknown vertex for ambulance dispatch center i
int NearestVertex(struct Graph *Map, int index) {
    int rst = Invalid, temp = Infinity;
    for (int i = 1; i <= Map->numADC + Map->numVer; i++) {
        if (Map->verList[i].dist[index] < temp && Map->verList[i].known[index] != 1) {
            rst = i;
            temp = Map->verList[i].dist[index];
        }
    }
    return rst;
}

//main algorithm
void Dijkstra(struct Graph *Map) {
    struct AdjVer *ptr;
    int tempVer, tempDist, heapSize;
    //there are i dispatch centers and each of them owns private parameters
    //so we need to run Dijkstra algorithm i times in all
    //and store the result in i different places
    for (int i = 1; i <= Map->numADC; i++) {
        //initialize A-i
        Map->verList[i].dist[i] = 0;
        Map->verList[i].path[i] = Invalid;
        //find the nearest vertex
        while ((tempVer = NearestVertex(Map, i)) != Invalid) {
            //update info of the nearest
            Map->verList[tempVer].known[i] = 1;
            tempDist = Map->verList[tempVer].dist[i];
            //update info of adjacent vertexes of the nearest one
            ptr = Map->verList[tempVer].adjVerList;
            while (ptr) {
                if (Map->verList[ptr->ver].known[i] != 1) {
                    if (Map->verList[ptr->ver].dist[i] > ptr->dist + tempDist) {
                        Map->verList[ptr->ver].dist[i] = ptr->dist + tempDist;
                        Map->verList[ptr->ver].path[i] = tempVer;
                    }
                }
                ptr = ptr->next;
            }
        }
    }
}

//initialize my Map
void initMap(struct Graph *Map) {
    //to simplify the index
    //each array are numbered from 1 to n
    //so we need n+1 spaces and XXX[0] is always unavailable in this project
    scanf("%d %d", &Map->numVer, &Map->numADC);
    //allocate new space
    Map->avlAmb = calloc(Map->numADC + 1, sizeof(int));
    Map->verList = calloc((Map->numVer + Map->numADC + 1), sizeof(struct Vertex));
    for (int i = 1; i < Map->numVer + Map->numADC + 1; i++) {
        Map->verList[i].path = calloc(Map->numADC + 1, sizeof(int));
        Map->verList[i].dist = calloc(Map->numADC + 1, sizeof(int));
        Map->verList[i].known = calloc(Map->numADC + 1, sizeof(int));
        //initialize
        for (int j = 1; j <= Map->numADC; j++) {
            Map->verList[i].path[j] = Invalid;
            Map->verList[i].dist[j] = Infinity;
        }
        Map->verList[i].adjVerList = NULL;
    }
    //load the number of available ambulance of each ADC
    for (int i = 1; i <= Map->numADC; i++)
        scanf("%d", &Map->avlAmb[i]);
}

//print the nearest way from a certain ADC to the streets
void PrintPath(struct Graph *Map, int ADC, int street) {
    //buff[] works as a simple stack
    int buff[Map->numADC + Map->numVer], size = Map->numADC + Map->numVer;
    int nowVer = street;
    //push
    buff[--size] = nowVer;
    //push
    while (Map->verList[nowVer].path[ADC] != Invalid) {
        buff[--size] = Map->verList[nowVer].path[ADC];
        nowVer = buff[size];
    }
    //pop
    while (size < Map->numADC + Map->numVer) {
        if (buff[size] <= Map->numADC)
            printf("A-%d ", buff[size]);
        else
            printf("%d ", buff[size] - Map->numADC);
        size++;
    }
    printf("\n");
}

//if two ADC have the same distance, judge which one owns a less number of edge
int LessEdge(struct Graph *Map, int street, int ADC1, int ADC2) {
    int cnt1 = 0, cnt2 = 0, temp1 = Map->verList[street].path[ADC1], temp2 = Map->verList[street].path[ADC2];
    while (temp1 != ADC1) {
        temp1 = Map->verList[temp1].path[ADC1];
        ++cnt1;
    }
    while (temp2 != ADC2) {
        temp2 = Map->verList[temp2].path[ADC2];
        ++cnt2;
    }
    return cnt1 < cnt2 ? ADC1 : ADC2;
}

//read a number as the destination and output the dispatch info
void Dispatch(struct Graph *Map) {
    int street, ADC = Invalid, tempDist = Infinity;
    scanf("%d", &street);
    street += Map->numADC;
    //find the nearest ADC
    for (int i = 1; i <= Map->numADC; i++) {
        if (Map->verList[street].dist[i] < tempDist && Map->avlAmb[i] > 0) {
            ADC = i;
            tempDist = Map->verList[street].dist[i];
        } else if (Map->verList[street].dist[i] == tempDist && Map->avlAmb[i] > 0)//if the dist is the same
            if (Map->avlAmb[ADC] == Map->avlAmb[i])//if the avlAmb is the same
                ADC = LessEdge(Map, street, i, ADC);
            else {
                ADC = Map->avlAmb[ADC] > Map->avlAmb[i] ? ADC : i;
            }
    }
    //output info
    if (ADC == Invalid) {
        printf("All Busy\n");
    } else {
        Map->avlAmb[ADC]--;
        PrintPath(Map, ADC, street);
        printf("%d\n", tempDist);
    }
}

int main() {
    //1st step: initialize Map
    int numEdge, numCall;
    struct Graph *Map = malloc(sizeof(struct Graph));
    initMap(Map);
    //2nd step: fulfill the Map with paths (aka edges)
    scanf("%d", &numEdge);
    for (int i = 0; i < numEdge; i++)
        LoadPath(Map);
    //3rd step: use Dijkstra algorithm to find the nearest path
    Dijkstra(Map);
    //4th step: handle dispatches and output info
    scanf("%d", &numCall);
    for (int i = 0; i < numCall; i++)
        Dispatch(Map);

    return 0;
}
