#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <math.h>

using namespace std;

int visit =1;
int unvisit =0;

class Edge{
public:
    int from,to,weight;
    Edge(){};
    Edge(int f,int t,int w){
        from=f;
        to=t;
        weight=w;
    }
    bool operator == (Edge &e1){
        if(e1.from == this->from && e1.to == this->to && e1.weight == this->weight){
            return true;
        }else{
            return false;
        }
    }
};

bool operator == (const Edge &e1, Edge &e2){
    if(e1.from == e2.from && e1.to == e2.to && e1.weight == e2.weight){
        return true;
    }else{
        return false;
    }
}

bool operator< (const Edge &e1, const Edge &e2){
    if(e1.from == e2.from){
        return e1.to < e2.to;
    }else{
        return e1.from < e2.from;
    }
}

class Graph{
public:
    int vertexNumber;
    int edgeNumber;
    int *Mark;
    int *indegree;
    vector< set<Edge> > edge;
    vector<int> Vertex;
    Graph(int _vertexNumber){
        edgeNumber=0;
        vertexNumber=_vertexNumber;
        Mark = new int[vertexNumber];
        indegree = new int[vertexNumber];
        for(int i=0;i<vertexNumber;i++){
            indegree[i]=0;
        }
    }
    void insertV(int v){
        Vertex.push_back(v);
        edge.push_back(set<Edge>());
    }
    void insertE(int from, int to, int weight){
        if(edge[from].insert(Edge(from,to,weight)).second){//用set防止重複的邊
            edgeNumber++;
            indegree[to]++;
        }
    }
    int ToVertex(Edge e){
        return e.to;
    }
    bool isEdge(Edge e){
        if(e.from==-1 && e.to==-1){
            return false;
        }else{
            return true;
        }
    }
    Edge firstEdge(int v){
        if(edge[v].empty()){
            return Edge(-1,-1,-1);
        }else{
            return *(edge[v].begin());
        }
    }
    Edge nextEdge(Edge e){
        int v;
        v = e.from;
        for(set<Edge>::iterator i=edge[v].begin();i!=(--edge[v].end());i++){
            if( *i == e ){
                return *(++i);
            }
        }
        return Edge(-1,-1,-1);
    }
};


//建立最小堆
template <class T>
class minHeap {
    T *heapArray;
    int currentSize=0;
    int maxSize;
    void BuildHeap();
    
public:
    minHeap(int n);
    T &RemoveMin();
    int Parent(int position);
    bool Insert(T newNode);
    bool Remove(int position, T &node);
    void SiftUp(int position);
    void SiftDown(int position);
    bool isEmpty();
};

template <class T>
minHeap<T>::minHeap(int n){
    heapArray = new T[n];
    maxSize = n;
}

template <class T>
int minHeap<T>::Parent(int position){
    return (position-1)/2;
}

template <class T>
void minHeap<T>::SiftDown(int position){
    int i = position;
    int j = 2*i+1;
    T temp = heapArray[i];
    while(j<currentSize){
        if((j+1)<currentSize && ( (heapArray[j]) > (heapArray[j+1]) )){
            j++;  //找出較小的child
        }
        //parent不能比任一child大
        if(temp > (heapArray[j]) ){
            heapArray[i] = heapArray[j];
            i=j;
            j=2*i+1;
        }else{
            break;
        }
    }
    heapArray[i] = temp;
}

template <class T>
void minHeap<T>::SiftUp(int position){
    int tempPos = position;
    T temp = heapArray[position];
    while(tempPos>0){
        if(temp < (heapArray[Parent(tempPos)]) ){
            heapArray[tempPos] = heapArray[Parent(tempPos)];
            tempPos = Parent(tempPos);
        }else{
            break;
        }
    }
    heapArray[tempPos] = temp;
}

template<class T>
void minHeap<T>::BuildHeap(){
    for(int i=currentSize/2-1;i>=0;i--){
        SiftDown(i);
    }
}

template<class T>
bool minHeap<T>::Insert(T newNode){
    if(currentSize==maxSize){
        return false;
    }
    heapArray[currentSize] = newNode;
    SiftUp(currentSize);
    currentSize++;
    return true;
}

template<class T>
bool minHeap<T>::Remove(int position,T &Node){
    if(position<0 || position>(currentSize-1)){
        return false;
    }
    Node = heapArray[position];
    //將最後一個元素覆蓋要刪除的元素，然後判斷他的新位置
    heapArray[position] = heapArray[--currentSize];
    if((heapArray[position]) < (heapArray[Parent(position)]) ){
        SiftUp(position);
    }else{
        SiftDown(position);
    }
    return true;
}

template<class T>
bool minHeap<T>::isEmpty(){
    return currentSize==0;
}

//拓墣排序後輸出的每個有向邊ＸＹ，Ｘ都會排在Ｙ之前
void TopSortByQueue(Graph &G){
    minHeap<int> Q(G.vertexNumber); //用最小堆結構取代陣列，讓輸出從小到大
    for(int i=0;i<G.vertexNumber;i++){
        if(G.indegree[i]==0){   //先找出那些入邊為0的點
            Q.Insert(i);
        }
    }
    int v;
    while(!Q.isEmpty()){    //將入邊為0點X輸出，並且將Ｘ出邊所到達的點Ｙ入邊數-1，並且判斷若是Ｙ入邊變為0則放入最小堆中
        Q.Remove(0, v);
        cout << 'v' << G.Vertex[v] << ' ';
        for(Edge e=G.firstEdge(v);G.isEdge(e);e=G.nextEdge(e)){
            G.indegree[G.ToVertex(e)]--;
            if(G.indegree[G.ToVertex(e)]==0){
                Q.Insert(G.ToVertex(e));
            }
        }
    }
}

int main(){
    
    int v,n;
    cin >> v >> n;
    Graph G(v);
    
    for(int i=1;i<=v;i++){
        G.insertV(i);
    }
    
    for(int i=1;i<=n;i++){
        int from,to;
        cin >> from >> to;
        G.insertE(from-1, to-1, 0);
    }
    
    TopSortByQueue(G);
    
    
    return 0;
    
}

