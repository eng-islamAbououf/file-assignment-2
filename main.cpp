#include <iostream>
#include <fstream>
#include <list>
using namespace std ;

struct node {
    int key ;
    int value ;

    void print (){
        cout << key << " " << value << endl ;
    }

    bool operator >(node & n){
        return key>n.key ;
    }
    bool operator <(node & n){
        return key<n.key ;
    }
};

class Block{
public:
    node* arr ;
    int size_ ;
    int index = 0 ;
    Block(int size ){
        size_ = size ;
        arr=new node[size] ;
    }

    void sort(){
        node key ;
        int i, j;
        for (i = 1; i < index; i++)
        {
            key = arr[i];
            j = i - 1;

            while (j >= 0 && arr[j] > key)
            {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
            arr[j + 1] = key;
        }
    }

    void insert(int key , int value){
        node * newNode = new node ;
        newNode->value = value ;
        newNode->key = key ;
        if (index<size_){
            arr[index] = *newNode;
            index++ ;
            sort() ;
            cout << "Node Inserted Successfully\n" ;
        }
        else
            cout << "Node can not Inserted because the size is full\n" ;
    }


    void printAll(){
        for (int i = 0; i < size_; ++i) {
            arr[i].print() ;
        }
    }

};

int main(){
    Block block(4) ;
    block.insert(1,5) ;
    block.insert(4,18) ;
    block.insert(2,88) ;
    block.insert(5,27) ;
    block.insert(7,28) ;
    block.printAll() ;
}