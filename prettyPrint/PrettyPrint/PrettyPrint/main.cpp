#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class threshold{
private:
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int* thr_ary;
    int** img;
    int thr_value;
    
    threshold(int maxVal, int numRow, int numCol){
        numRows = numRow;
        numCols = numCol;
        this->maxVal = maxVal;
        
        thr_ary = new int[maxVal];
        for(int i=0; i<maxVal; i++){
            thr_ary[i] = 0;
        }//for
        
        img = new int*[numRows];
        for(int i=0; i<numRows; i++){
            img[i] = new int[numCols];
        }//for i
        
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                img[i][j] = 0;
            }//for j
        }//for i
    }//constructor
    
    void loadImage(ifstream& inFile){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                while(!inFile.eof()){
                    inFile >> data;
                    img[i][j] = data;
                    break;
                }//while
            }//for j
        }//for i
    }//loadImage
    
    void prettyPrint(ofstream& outFile){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(img[i][j] > 0){
                    outFile << img[i][j];
                }//if
                else{
                    outFile << " ";
                }//else
            }//for j
            outFile << "\n";
        }//for i
    }//prettyPrint
};

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile;
    
    inFile.open(argv[1]);
    
    int data;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thr_value;
    
    inFile >> data;
    numRows = data;
    cout << "numRows: " << data << "\n";
    inFile >> data;
    numCols = data;
    cout << "numCols: " << data << "\n";
    inFile >> data;
    minVal = data;
    cout << "minVal: " << data << "\n";
    inFile >> data;
    maxVal = data;
    cout << "maxVal: " << data << "\n";
    
    cout << "enter thr_value: ";
    cin >> thr_value;
    
    threshold myThreshold(maxVal,numRows,numCols);
    
    string array = argv[1];
    ostringstream convert;
    string substring = array.substr(0,array.rfind("."));
    convert << array;
    string outName = substring + "_PP";
    outFile.open(outName);
    
    myThreshold.loadImage(inFile);
    myThreshold.prettyPrint(outFile);
    
    inFile.close();
    
    outFile.close();
    
    return 0;
}
