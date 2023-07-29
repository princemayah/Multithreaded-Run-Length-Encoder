#include <iostream>
#include <vector>
#include <pthread.h>

using namespace std;

// Struct to hold RLE data
struct RLEData {
  string inputString;
  string rleString;
  vector<int> rleFreq;
};

// Thread function for RLE encoding
void* RLE(void* arg) {
  RLEData* rleData = static_cast<RLEData*>(arg);
  const string& inputString = rleData->inputString;
  string& rleString = rleData->rleString;
  vector<int>& rleFreq = rleData->rleFreq;
  
  int count = 1;

  // Perform RLE encoding
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
  vector<string> input;

  // Read input strings until an empty line is entered
  while (getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    input.push_back(line);
  }

  // Create RLEData structures to hold RLE data for each input string
  vector<RLEData> rleData(input.size());
  // Create thread IDs
  vector<pthread_t> threads(input.size());

  // Iterate over each input string
  for (int i = 0; i < input.size(); i++) {
    rleData[i].inputString = input[i];

    // Create a thread for each input string and call the RLE function
    if (pthread_create(&threads[i], NULL, &RLE, &rleData[i])) {
      printf("Error creating thread for input line %d\n", i + 1);
      return 1;
    }
  }

  // Wait for all threads to finish
  for (int i = 0; i < input.size(); i++) {
    if (pthread_join(threads[i], NULL)) {
      printf("Error joining thread for input line %d\n", i + 1);
      return 2;
    }
  }

  // Print the RLE data for each input string
  for (int i = 0; i < input.size(); i++) {
    cout << "\nInput string: " << rleData[i].inputString << endl;

    cout << "RLE String: " << rleData[i].rleString << endl;

    cout << "RLE Frequencies: ";
    for (int numbers : rleData[i].rleFreq) {
      cout << numbers << " ";
    }
    cout << endl;
  }

  return 0;
}
