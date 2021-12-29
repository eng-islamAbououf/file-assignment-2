#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std ;

int RRN = 5 ;
int blockIndex = 0 ;

int firstEmptyBlock ;
int firstNonEmpty;
int numOfBlocks  ;
int numOfRecords  ;
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
    int maxKey  ;
    int next ;
    Block(int size ){
        size_ = size ;
        arr=new node[size] ;
    }

    Block() {}

    void sort(){
        node key ;
        int i, j;
        for (i = 1; i <= index; i++)
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

    bool insert(int key , int value){
        index = 0 ;
        for (int i = 0; i < size_; ++i) {
            if (arr[i].key==-1)
                break;
            index ++ ;
        }

        if (index == size_){
            if (next==-1)
                next = blockIndex+2 ;
            return false ;
        }
        if (index==0)
            maxKey = key ;
        else {
            if (key>maxKey)
                maxKey = key ;
        }
        arr[index].key = key ;
        arr[index].value = value ;
        sort() ;
        return true ;
    }

    friend ostream& operator<< (ostream& out , Block& block){
        out << setw(RRN) << block.next ;
        for (int i = 0; i < block.size_; ++i) {
            out << setw(RRN) << block.arr[i].key ;
        }
        out << endl ;
        out << setw(RRN) << block.maxKey ;
        for (int i = 0; i < block.size_; ++i) {
            out << setw(RRN) << block.arr[i].value ;
        }
    }

    void updateMax() {
        for (int i = 0; i < size_ &&arr[i].key!=-1 ; ++i) {
            maxKey = arr[i].key ;
        }
    }
};

void updateIndexes(char *cIndexFile);

int getEmpty(char *file);

void CreateRecordFile(char *cIndexFile, int m, int n){
    fstream file ;
    file.open(cIndexFile,ios::binary | ios::in | ios::out) ;
    file << setw(RRN) << firstNonEmpty << endl ;
    file << setw(RRN) << firstEmptyBlock << endl ;
    for (int i = 0; i < m-1; ++i) {
        file << setw(RRN) << -1 ;
        for (int j = 0; j < n-1; ++j) {
            file << setw(RRN) << -1 ;
        }
        file << endl ;
        if (i+1 < m-1)
            file << setw(RRN) << i+2 ;
        else
            file << setw(RRN) << -1 ;
        for (int j = 0; j < n-1; ++j) {
            file << setw(RRN) << -1 ;
        }
        file << endl ;
    }
    file.close() ;
}

Block loadBlock(char* fileName){
    fstream file ;
    file.open(fileName , ios::binary | ios::out | ios::in) ;
    file.seekg((blockIndex*numOfRecords*RRN*2)+12+(blockIndex*2),ios::beg) ;
    Block cur (numOfRecords-1);
    file >> cur.next ;
    for (int i = 0; i < numOfRecords-1; ++i) {
        file >> cur.arr[i].key ;
    }
    file >> cur.maxKey ;
    for (int i = 0; i < numOfRecords-1; ++i) {
        file >> cur.arr[i].value ;
    }
    file.close() ;
    return cur ;
}
void InsertVal(char *cIndexFile, int iToken, int iKey){

    fstream file ;
    file.open(cIndexFile,ios::binary | ios::in | ios::out) ;
    blockIndex=0;
    Block cur = loadBlock(cIndexFile) ;
    Block next ;
    while (cur.next!=-1){
        blockIndex++ ;
        next = loadBlock(cIndexFile) ;
        if (iKey < next.arr[0].key && cur.index!=cur.size_)
            break ;
        else
            cur = loadBlock(cIndexFile);
    }
    if (!cur.insert(iKey,iToken)) {
        blockIndex++;
        next = loadBlock(cIndexFile);
        for (int i = numOfRecords / 2; i < numOfRecords - 1; ++i) {
            next.insert(cur.arr[i].key, cur.arr[i].value);
            cur.arr[i].key = -1;
            cur.arr[i].value = -1;
        }
        cur.updateMax();
        if (iKey < next.arr[0].key)
            cur.insert(iKey, iToken);
        else
            next.insert(iKey, iToken);
        file.seekg((blockIndex * numOfRecords * RRN * 2) + 12 + (blockIndex * 2), ios::beg);
        file << next;
        blockIndex--;
    }
    else
        firstNonEmpty = 1 ;
    file.seekg((blockIndex*numOfRecords*RRN*2)+12+(blockIndex*2),ios::beg) ;
    file << cur ;
    file.close() ;
}

void updateIndexes(char *cIndexFile) {
    firstEmptyBlock = getEmpty(cIndexFile) ;
    fstream file ;
    file.open(cIndexFile,ios::binary | ios::in | ios::out) ;
    file.seekg(0) ;
    file << setw(RRN) << firstNonEmpty << endl ;
    file << setw(RRN) << firstEmptyBlock << endl ;
    file.close() ;
}

int getEmpty(char *cIndexFile) {
    int index = -1 ;
    int t ;
    fstream file ;
    file.open(cIndexFile,ios::binary | ios::in | ios::out) ;

    for (int i = 0; i < numOfBlocks-1; ++i) {
        file.seekg((i*numOfRecords*RRN*2)+12+(i*2)+RRN,ios::beg) ;
        file >> t ;
        if (t==-1){
            index = i+1 ;
            break;
        }
    }
    file.close() ;
    return index ;
}


int main(){

    char * fileName = "islam.txt" ;

    firstEmptyBlock  = 1 ;
    firstNonEmpty = -1 ;
    numOfBlocks = 4 ;
    numOfRecords = 5 ;
    CreateRecordFile(fileName , numOfBlocks , numOfRecords) ;
    InsertVal(fileName , 5 ,1 ) ;
    InsertVal(fileName ,  18, 4) ;
    InsertVal(fileName , 27 ,5 ) ;
    InsertVal(fileName , 88 ,2 ) ;
    InsertVal(fileName , 20 ,3 ) ;
    InsertVal(fileName , 4 ,8) ;
    InsertVal(fileName , 11,9) ;
    InsertVal(fileName , 15 ,7) ;
    updateIndexes(fileName);

}