#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std ;

const string META_DATA = "metaData.txt" ;
char *fileName = new char [100];
int RRN = 5 ;
int blockIndex = 0 ;

int firstEmptyBlock ;
int firstNonEmpty;
int numOfBlocks  ;
int numOfRecords  ;
struct Record {
    int key ;
    int value ;

    bool operator >(Record & n) const{
        return key>n.key ;
    }
    bool operator <(Record & n) const{
        return key<n.key ;
    }
};

class Block{
public:
    Record* arr ;
    int size_ ;
    int index = 0 ;
    int maxKey  ;
    int next ;
    Block(int size ){
        size_ = size ;
        arr=new Record[size] ;
        next = -1 ;
        maxKey = -1 ;
    }

    Block() {
        next = -1 ;
        maxKey = -1 ;
    }

    void sort() const{
        Record key ;
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

    bool deleteRecord(int key){
        int i;
        bool flag = false ;
        for (i = 0; i < size_; ++i) {
            if (arr[i].key==key){
                int j ;
                for (j = i; j <size_-1 ; ++j) {
                    arr[j].key = arr[j+1].key ;
                    arr[j].value = arr[j+1].value ;
                }
                arr[j] .key = -1 ;
                arr[j] .value = -1 ;
                flag = true ;
                index -- ;
                updateMax() ;
                break;
            }
        }
        return flag ;
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
        return out ;
    }

    void updateMax() {
        for (int i = 0; i < size_ &&arr[i].key!=-1 ; ++i) {
            maxKey = arr[i].key ;
        }
    }
};
int search(char *cIndexFile, int iBlock, int key){
    int i = -1 ;
    if (iBlock==-1)
        return -1 ;
    fstream file;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    file.seekg((iBlock * numOfRecords * RRN * 2) + 12 + (iBlock * 2) + RRN) ;
    int counter = 0 ,tmp ;
    while (counter<numOfRecords-1){
        file>>tmp ;
        if (tmp==key){
            i=counter+1 ;
            break;
        }
        counter++ ;
    }
    file.close() ;
    return i ;
}
void updateIndexes(char *cIndexFile);
int GetBlockIndex (char *cIndexFile, int iToken){
    fstream file;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    int index = -1, max, next;
    do {
        index++;
        file.seekg((index * numOfRecords * RRN * 2) + 12 + (index * 2));
        file >> next;
        if (next==-1)
            break;
        file.seekg((numOfRecords - 1) * RRN, ios::cur);
        file >> max;
        if (max>=iToken)
            break;
    } while (true);
    file.close() ;
    if (search(cIndexFile,index , iToken)!=-1)
        return index+1 ;
    return -1 ;
}
int getEmpty(char *file);

void addToMetaData(char *name, int blocks, int records);

int indexOfFile(char *name);

void getFile(int i);

bool CreateRecordFile(char *cIndexFile, int m, int n){
    fstream file ;
    file.open(cIndexFile,ios::binary | ios::out) ;
    if (file){
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
        return true;
    }
    return false ;
}

Block loadBlock(char* fileName){
    fstream file ;
    file.open(fileName , ios::binary | ios::out | ios::in) ;
    file.seekg((blockIndex*numOfRecords*RRN*2)+12+(blockIndex*2),ios::beg) ;
    Block cur (numOfRecords-1);
    file >> cur.next ;
    for (int i = 0; i < numOfRecords-1; ++i) {
        file >> cur.arr[i].key ;
        if (cur.arr[i].key!=-1)
            cur.index = i ;
    }
    file >> cur.maxKey ;
    for (int i = 0; i < numOfRecords-1; ++i) {
        file >> cur.arr[i].value ;
    }
    file.close() ;
    return cur ;
}
int InsertVal(char *cIndexFile, int iToken, int iKey){

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
    updateIndexes(cIndexFile);
    file.close() ;
    return GetBlockIndex(cIndexFile,iKey) ;
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

int FirstEmptyBlock(char *cIndexFile){
    fstream file;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    int x ;
    file.seekg(RRN+1) ;
    file >> x ;
    file.close() ;
    return x ;
}


int GetRecordIndex (char *cIndexFile, int iToken){
    // Get Block index
    int index = GetBlockIndex(cIndexFile,iToken) ;
    return search(cIndexFile,index-1,iToken) ;
}
int GetKey(char *cIndexFile, int iBlock, int iRecord){
    fstream file;
    iBlock-- ;
    int x ;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    file.seekg((iBlock * numOfRecords * RRN * 2) + 12 + (iBlock * 2)) ;
    file.seekg(iRecord*RRN,ios::cur) ;
    file>>x ;
    file.close() ;
    return x ;
}
int GetVal(char *cIndexFile, int iBlock, int iRecord){
    fstream file;
    iBlock-- ;
    int x ;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    file.seekg((iBlock * numOfRecords * RRN * 2) + 12 + (iBlock * 2)) ;
    file.seekg((iRecord*RRN)+ (numOfRecords*RRN)+iRecord,ios::cur) ;
    file>>x ;
    file.close() ;
    return x ;
}
bool isAbelToMerge(char *cIndexFile,int index) {
    if (index==-1)
        return false ;
    fstream file;
    bool flag = false ;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);
    int x ;
    file.seekg((index*numOfRecords*RRN*2)+12+(index*2) + RRN,ios::beg) ;
    file.seekg((numOfRecords-1)/2 * RRN,ios::cur) ;
    file>> x ;
    if (x==-1)
        flag = true ;
    file.close() ;
    return flag ;
}
void DeleteKey (char *cIndexFile, int iToken) {
    fstream file;
    file.open(cIndexFile, ios::binary | ios::in | ios::out);

    blockIndex = GetBlockIndex(cIndexFile,iToken)-1;
    if (blockIndex==-1){
        cout << "Key Not Found \n" ;
        return;
    }
    Block cur = loadBlock(cIndexFile);
    if (cur.deleteRecord(iToken)) {
        if (cur.index+1 < cur.size_ / 2) {
            // merge
            cout << "Now We need Merge\n" ;

            if (isAbelToMerge(cIndexFile,blockIndex-1)){
                blockIndex -- ;
                Block bre = loadBlock(cIndexFile);
                for (int i = 0; i < (numOfRecords-1)/2 - 1; ++i) {
                    bre.insert(cur.arr[i].key,cur.arr[i].value);
                    cur.arr[i].key = -1 ;cur.arr[i].value = -1 ;
                    cur.index -- ;
                }
                bre.next = cur.next ;
                bre.updateMax() ;
                cur.maxKey = -1 ;
                cur.next = -1 ;
                file.seekg((blockIndex * numOfRecords * RRN * 2) + 12 + (blockIndex * 2));
                file << bre ;
                blockIndex ++ ;
            }else if (isAbelToMerge(cIndexFile , blockIndex+1)){
                blockIndex ++ ;
                Block bre = loadBlock(cIndexFile);
                for (int i = 0; i < (numOfRecords-1)/2 - 1; ++i) {
                    cur.insert(bre.arr[i].key,bre.arr[i].value);
                    bre.arr[i].key = -1 ;bre.arr[i].value = -1 ;
                }
                cur.next = bre.next ;
                cur.maxKey = bre .maxKey;
                bre.maxKey = -1 ;
                bre.next = -1 ;
                file.seekg((blockIndex * numOfRecords * RRN * 2) + 12 + (blockIndex * 2));
                file << bre ;
                blockIndex -- ;
            }else
                cout << "Unable to Merge \n" ;
        }
    } else {
        cout << "Key : " << iToken << " Not Found !\n";
    }
    file.seekg((blockIndex * numOfRecords * RRN * 2) + 12 + (blockIndex * 2));
    file << cur ;
    file.close() ;
    updateIndexes(cIndexFile);
}

int main(){

    int choice = 0 ;
    do {
        cout << "************ Welcome TO my Sequence Set main *****************\n"
            << "Choose Operation  : " << endl
            << "1- Open Existing Sequence Set " << endl
            << "2- Create New Sequence Set " << endl
            << "3- Insert New Record " << endl
            << "4- Delete Record " << endl
            << "5- Search about Record " << endl
            << "6- Exit " << endl ;
        cin >> choice ;
        if (choice==1){
            cout << "Enter Data File name : " << endl ;
            cin >> fileName ;
            int i = indexOfFile(fileName) ;
            if (i!=-1){
                getFile(i) ;
                cout << "Switched To This Sequence Set" << endl ;
            }else
                cout << "File With this Name Not Found " << endl ;
        }else if (choice==2){
            cout << "Enter Data File name : " << endl ;
            cin >> fileName ;
            int i = indexOfFile(fileName) ;
            if (i!=-1){
                cout << "This Sequence Set Already Exists" << endl ;
                continue;
            }
            firstEmptyBlock  = 1 ;
            firstNonEmpty = -1 ;
            cout << "Enter Number Of Blocks : " << endl ;
            cin >> numOfBlocks ;
            cout << "Enter Number Of Record in one Block : " << endl ;
            cin >> numOfRecords ;
            if (CreateRecordFile(fileName,numOfBlocks,numOfRecords)){
                addToMetaData(fileName , numOfBlocks , numOfRecords) ;
                cout << "New Sequence Set Created Successfully With Name : "<< fileName << endl ;
                cout << "Switched To This Sequence Set" << endl ;
            }else
                cout << "Sorry Can't Create This Set ''( " << endl ;
        }else if (choice==3){
            int key , value ;
            cout << "Enter Record Key : "<<endl ;
            cin >> key ;
            cout << "Enter Record Value : "<<endl ;
            cin >> value ;
            if (InsertVal(fileName,value,key))
                cout << "Record Inserted Successfully " << endl ;
            else
                cout << "Record Can't Insert" << endl ;
        }else if (choice==4){
            int key  ;
            cout << "Enter Record Key : "<<endl ;
            cin >> key ;
            DeleteKey(fileName,key) ;
        }else if (choice==5){
            int key  ;
            cout << "Enter Record Key : "<<endl ;
            cin >> key ;
            int i = GetBlockIndex(fileName , key) ;
            int ii = GetRecordIndex(fileName , key) ;
            if (ii==-1)
                cout << "Record Not Found!!" << endl ;
            else {
                cout << "Record Key : " << key << endl ;
                cout << "Record Value : " << GetVal(fileName,i,ii) << endl ;
            }
        }else if (choice==6){
            break;
        }
    } while (true) ;
}

void getFile(int i) {
    fstream file ;
    file.open(META_DATA , ios::out | ios::in | ios::binary) ;
    file.seekg(i) ;
    file >> fileName ;
    file >> numOfBlocks >> numOfRecords ;
    file.close() ;
}

int indexOfFile(char *name) {
    int index = -1  , byteOff;
    int size ;
    string temp ;
    bool isFound = false ;
    fstream file ;
    file.open(META_DATA , ios::out | ios::in | ios::binary) ;
    file >> size ;
    file.seekg( 1, ios::cur) ;
    for (int i = 0; i < size; ++i) {
        byteOff = file.tellg() ;
        file >> temp ;
        if (temp==name){
            isFound = true ;
            break;
        }
        file.seekg( 11, ios::cur) ;
    }
    if (isFound)
        index = byteOff ;
    file.close() ;
    return index;
}

void addToMetaData(char *name, int blocks, int records) {
    fstream file ;
    file.open(META_DATA , ios::out | ios::in | ios::binary) ;
    int num  ;
    file>>num ;
    file.seekg( 0, ios::end) ;
    file << name << " " ;
    file << setw(RRN) << blocks ;
    file << setw(RRN) << records ;
    file << endl ;
    num++ ;
    file.seekg( 0, ios::beg) ;
    file << num ;
    file.close() ;
}
