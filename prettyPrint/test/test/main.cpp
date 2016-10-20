#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
class ImageProcessing{
    
private:
    int maxVal;
    int minVal;
    int** imgAry;
    int** MirrorframedAry;
    int** tempAry;
    int numRows;
    int numCols;
    int* histogram;
    
    
public:
    int getRows(){
        return numRows;
    }
    
    int getCols(){
        return numCols;
    }
    
    void setRows(int row){
        numRows=row;
    }
    void setCols(int col){
        numCols=col;
    }
    
    void readInput(ifstream& myReadFile){
        int count=0;
        int contents;
        if (myReadFile.is_open()) {
            while (!myReadFile.eof()) {
                count++;
                myReadFile >> contents;
                if(count==1){
                    numRows=contents;
                }else if(count==2){
                    numCols=contents;
                }else if(count==3){
                    minVal=contents;
                }else if(count==4){
                    maxVal=contents;
                    histogram= new int[maxVal+1];
                    for(int m=0; m<maxVal+1;m++){
                        histogram[m]=0;
                    }
                    break;
                }
            }
            MirrorframedAry= new int*[numRows+2];
            for(int i = 0; i < numRows+2; ++i){
                MirrorframedAry[i] = new int[numCols+2];
            }
            
            tempAry= new int*[numRows+2];
            for(int i = 0; i < numRows+2; ++i){
                tempAry[i] = new int[numCols+2];
            }
            
            
            imgAry = new int*[numRows];
            for(int i = 0; i < numRows; ++i){
                imgAry[i] = new int[numCols];
            }
            
            for(int i=0; i<numRows; i++){
                for(int j=0; j<numCols; j++){
                    while(!myReadFile.eof()){
                        myReadFile>>contents;
                        computeHistogram(contents);
                        imgAry[i][j]=contents;
                        break;
                    }
                }
            }
            myReadFile.close();
        }
    }
    
    void printMirrorframedAry(){
        for(int i=0; i<numRows+2; i++){
            cout<<"\n";
            for(int j=0; j<numCols+2; j++){
                cout<<MirrorframedAry[i][j]<<" ";
            }
        }
    }
    
    void zeroFramed (){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                tempAry[i+1][j+1]=imgAry[i][j];
            }
        }
        
    }
    
    void mirrorFramed (){
        for(int i=0; i<numRows+1; i++){
            for(int j=0; j<numCols+1; j++){
                if(i==1){
                    MirrorframedAry[i-1][j+1]=tempAry[i][j+1];
                    MirrorframedAry[0][0]=tempAry[1][1];
                }
                if(i==numRows){
                    MirrorframedAry[i+1][j+1]=tempAry[i][j+1];
                    MirrorframedAry[numRows+1][0]=tempAry[numRows][1];
                }
                if(j==1){
                    MirrorframedAry[i][j-1]=tempAry[i][j];
                    MirrorframedAry[0][numCols+1]=tempAry[i][numCols];
                }
                if(j==numCols){
                    MirrorframedAry[i][j+1]=tempAry[i][j];
                    MirrorframedAry[numRows+1][numCols+1]=tempAry[numRows][numCols];
                }
                MirrorframedAry[i][j]=tempAry[i][j];
            }
        }
    }
    
    void computeHistogram(int n){
        histogram[n]++;
        
    }
    
    void printary(){
        for(int i=1; i<maxVal+1; i++){
            cout<<histogram[i]<<" ";
        }
    }
    
    void computeThreshold(int threshold){
        cout<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal;
        for(int i=0; i<numRows; i++){
            cout<<"\n";
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j] < threshold){
                    imgAry[i][j]=0;
                    cout<<"0";
                }else{
                    imgAry[i][j]=1;
                    cout<<"1";
                }
            }
        }
        cout<<"\n";
    }
    
    void prettyPrint(ofstream& out){
        out<<"\n";
        out<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal;
        for(int i=0; i<numRows; i++){
            out<<"\n";
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j] <= 0){
                    out<<" ";
                }else{
                    out<<imgAry[i][j]<<" ";
                }
            }
        }
        out.close();
    }
    
    void printStart(int value){
        for(int i=0; i<value; i++){
            cout<<"+";
        }
    }
    
    void printHis(){
        cout<<endl;
        for(int i=1; i <= maxVal; i++){
            cout<<"("<< i <<")"<<" : ";
            printStart(histogram[i]);
            cout<<"\n";
        }
    }
    
    void AVG3X3(){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                int avg=computeAvg(i,j);
                MirrorframedAry[i][j]=avg;
            }
        }
    }
    
    void Median3X3(){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                int med=getMedian(i,j);
                MirrorframedAry[i][j]=med;
            }
        }
    }
    
    void assignToTempAry(){
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                tempAry[i][j]=MirrorframedAry[i][j];
            }
        }
    }
    
    void printAvgAry(ofstream& out){
        out<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal;
        for(int i=1; i<numRows+1; i++){
            out<<"\n";
            for(int j=1; j<numCols+1; j++){
                out<<tempAry[i][j]<<" ";
            }
        }
    }
    
    void printMedAry(ofstream& out){
        out<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal;
        for(int i=1; i<numRows+1; i++){
            out<<"\n";
            for(int j=1; j<numCols+1; j++){
                out<<tempAry[i][j]<<" ";
            }
        }
    }
				
    int computeAvg(int i, int j){
        int a=MirrorframedAry[i-1][j-1];
        int b=MirrorframedAry[i-1][j];
        int c=MirrorframedAry[i-1][j+1];
        int d=MirrorframedAry[i][j-1];
        int e=MirrorframedAry[i][j];
        int f=MirrorframedAry[i][j+1];
        int g=MirrorframedAry[i+1][j-1];
        int h=MirrorframedAry[i+1][j];
        int k=MirrorframedAry[i+1][j+1];
        int avg =(a+b+c+d+e+f+g+h+k)/9;
        return avg;	
    }
    
    int getMedian(int i, int j){
        int a=MirrorframedAry[i-1][j-1];
        int b=MirrorframedAry[i-1][j];
        int c=MirrorframedAry[i-1][j+1];
        int d=MirrorframedAry[i][j-1];
        int e=MirrorframedAry[i][j];
        int f=MirrorframedAry[i][j+1];
        int g=MirrorframedAry[i+1][j-1];
        int h=MirrorframedAry[i+1][j];
        int k=MirrorframedAry[i+1][j+1];
        int tempArry[9] = {a,b,c,d,e,f,g,h,k};
        bubble_sort(tempArry, 9);
        return tempArry[4];
    }
    
    void bubble_sort (int arr[], int n)
    {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n - i - 1; ++j)
                if (arr[j] > arr[j + 1])
                {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
    } 		
};

int main(int argc, char** argv) {
    ImageProcessing img;
    ifstream myReadFile;
    
    myReadFile.open(argv[1]);
    ofstream out (argv[2]);
    
    string threshold = (argv[3]);
    int value = atoi(threshold.c_str());
    
    img.readInput(myReadFile);
    /*	img.zeroFramed();
     img.mirrorFramed();
     img.AVG3X3();
     img.assignToTempAry();
     img.printAvgAry(out);*/
    
    img.zeroFramed();
    img.mirrorFramed();
    img.Median3X3();
    img.assignToTempAry();
    img.printMedAry(out);
    
    argv[1]=argv[2];
    
    ifstream myReadFile1;
    myReadFile1.open(argv[1]);
    
    img.readInput(myReadFile1);
    img.printHis();
    img.computeThreshold(value);
    img.prettyPrint(out);
    
    
    //	img.printHis();
    //	img.computeThreshold(argc,argv);
    //	img.prettyPrint(argc,argv);
    //	img.printary();
    //	img.computeThreshold(35);
    //	img.prettyPrint();
    
    /*	img.zeroFramed();
     img.mirrorFramed();
     img.printMirrorframedAry();
     img.Median3X3();
     img.assignToTempAry();
     img.printMedAry();
     */
    //	img.printArray();
    //	img.AVG3X3();
    //	img.assignToTempAry();
    //	img.printArray();
    //	img.printAvgAry();
    
}
