#include <fstream>
#include <bitset>
#include <iostream>

#define clockInterrupt 3
#define frameSize 4

using namespace std;

ofstream outputFile("out1.txt");

int main() {
    ifstream inputFile("in1.txt");
    int num_referenced_pages;
    string str;
    if(inputFile.is_open()){
        getline(inputFile, str);
        num_referenced_pages = stoi(str);
        cout << "Number of referenced pages: " << num_referenced_pages << endl;
    }
    else{
        cout << "Unable to open file" << endl;
        return 0;
    }
    int referenced_pages[num_referenced_pages];
    cout << "Virtual page numbers: ";
    for(int i = 1; inputFile >> str ; i++){
        referenced_pages[i] = stoi(str);
        cout << referenced_pages[i] << " ";
    }
    cout << endl;
    int physicalMemory[frameSize];
    unsigned int freqArr[num_referenced_pages];
    int referencedBit[num_referenced_pages];
    int time[num_referenced_pages];
    for(int i = 1; i <= frameSize; i++)physicalMemory[i] = -1;
    for(int i = 1; i <= num_referenced_pages; i++)referencedBit[i] = freqArr[i] = 0;
    int flag, k, minimum_time, temporaryPos = 0;
    for(int i = 1; i <= num_referenced_pages; i++){
        flag = 1;
        k = physicalMemory[1];
        time[referenced_pages[i]] = i;
        for(int j = 1; j <= frameSize; j++){
            if(physicalMemory[j] == -1 || physicalMemory[j] == referenced_pages[i]){
                flag = 0;
                physicalMemory[j] = referenced_pages[i];
                referencedBit[physicalMemory[j]] = 1;
                break;
            }
            if(freqArr[k] > freqArr[physicalMemory[j]]){
                k = physicalMemory[j];
            }
        }
        if(flag){
            minimum_time = num_referenced_pages;
            for(int j = 1; j <= frameSize; j++){
                if(freqArr[physicalMemory[j]] == freqArr[k] && time[physicalMemory[j]] < minimum_time){
                    temporaryPos = j;
                    minimum_time = time[physicalMemory[j]];
                }
            }
            physicalMemory[temporaryPos] = referenced_pages[i];
            referencedBit[physicalMemory[temporaryPos]] = 1;
        }

        if(i % clockInterrupt == 0){
            for(int j = 1; j <= frameSize; j++){
                freqArr[physicalMemory[j]] >>= 1;
                if(referencedBit[physicalMemory[j]] == 1){
                    referencedBit[physicalMemory[j]] = 0;
                    freqArr[physicalMemory[j]] |= (1 << 7);
                }
            }
        }
        outputFile << referenced_pages[i] << endl;
        outputFile << "VPN\tPPN\tR bit\tCOUNTER" << endl;
        for(int j = 1; j <= frameSize; j++){
            if(physicalMemory[j] >= 0){
                bitset<8> count = freqArr[physicalMemory[j]];
                outputFile << physicalMemory[j] << "\t" << j << "\t" << referencedBit[physicalMemory[j]]
                                                                 << "\t" << count << endl;
            }
            else{
                bitset<8> count = 0;
                outputFile << physicalMemory[j] << "\t" << j << "\t" << 0 << "\t" << count << endl;
            }
        }
        outputFile << endl;
    }
}
