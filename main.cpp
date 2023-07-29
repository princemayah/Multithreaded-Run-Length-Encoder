#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

struct Data {
  string inputString;
  string rleString;
  vector<int> rleFreq;
};

void* RLE(void* arg) {
  Data* rleData = static_cast<Data*>(arg);
  
  string& inputString = rleData->inputString;
  string& rleString = rleData->rleString;
  vector<int>& rleFreq = rleData->rleFreq;
  
  int i = 0;
  int count = 1;
  
  for (int i = 0; i < inputString.size(); i++) {
    if (inputString[i] == inputString[i+1]) {
      count++;
    }
    else {
      if (count > 1) {
        rleString.push_back(inputString[i]);
        rleString.push_back(inputString[i]);
        rleFreq.push_back(count);
        count = 1;
      }
      else {
        rleString.push_back(inputString[i]);
      }
    }
  }
  return nullptr;
}

int main() {
  string line;
  vector <string> input;
  
  while (getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    input.push_back(line);
  }
  
  vector<Data> rleData(input.size());
  vector<pthread_t> threads(input.size());
  
  for (int i = 0; i < input.size(); i++) {
    rleData[i].inputString = input[i];
    
    if (pthread_create(&threads[i], NULL, &RLE, &rleData[i])) {
      printf("Error creating thread for input line %d\n", i+1);
      return 1;
    }
  }
  
  for (int i = 0; i < input.size(); i++) {
    if (pthread_join(threads[i], NULL)) {
      printf("Error joining thread for input line %d\n", i+1);
      return 2;
    }
  }
  
  for (int i = 0; i < input.size(); i++) {
    cout << "\nInput string: " << rleData[i].inputString << endl;
    
    cout << "RLE String: ";
    for (char letters : rleData[i].rleString) {
      cout << letters;
    }
    cout << endl;
    
    cout << "RLE Frequencies: ";
    for (int numbers : rleData[i].rleFreq) {
    cout << numbers << " ";
    }
    cout << endl;
  }
  
  return 0;
}