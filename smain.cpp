#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;

// Struct for holding data for each input string and its RLE result
struct Data {
  string inputString;
  string rleString;
  vector<int> rleFreq;
  pthread_mutex_t mutex; // Mutex semaphore for synchronization
};

// Function for preforming Run-Length Encoding for a given input string
void *RLE(void *arg) {
  Data *rleData = static_cast<Data *>(arg);
  
  string &inputString = rleData->inputString;
  string &rleString = rleData->rleString;
  vector<int> &rleFreq = rleData->rleFreq;
  
  int i = 0;
  int count = 1;
  
  // Loop through the input string to preform RLE
  for (int i = 0; i < inputString.size(); i++) {
    // If the current character is the same as the next one, increase the count
    if (inputString[i] == inputString[i + 1]) {
      count++;
    }
    else {
      // If the current character is different from the next one, update the RLE result
      pthread_mutex_lock(&rleData->mutex); // Lock the mutex before modifying shared data
      
      if (count > 1) {
        // Append the character and its count to the RLE result
        rleString.push_back(inputString[i]);
        rleString.push_back(inputString[i]);
        rleFreq.push_back(count);
        count = 1;
      }
      else {
        // If count is 1, append only the character to the RLE result
        rleString.push_back(inputString[i]);
      }
      
      pthread_mutex_unlock(&rleData->mutex); // Unlock the mutex after modifying shared data
    }
  }
  return nullptr;
}

int main() {
  string line;
  vector<string> input;
  
  // Read input string from the user until an empty line is encountered
  while (getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    input.push_back(line);
  }
  
  // Create a seperate Data struct for each input string
  vector<Data> rleData(input.size());
  vector<pthread_t> threads(input.size());
  
  // Initialize and create threads for each input string
  for (int i = 0; i < input.size(); i++) {
    rleData[i].inputString = input[i];
    pthread_mutex_init(&rleData[i].mutex, NULL); // Initialize the mutex
    
    // Create a thread for each input string to preform RLE
    if (pthread_create(&threads[i], NULL, &RLE, &rleData[i])) {
      printf("Error creating thread for input line %d\n", i + 1);
      return 1;
    }
  }
  
  // Wait for all threads to finish their tasks
  for (int i = 0; i < input.size(); i++) {
    pthread_join(threads[i], NULL); // Wait for the thread to complete
    pthread_mutex_destroy(&rleData[i].mutex); // Destroy the mutex after thread execution
  }
  
  // Print the results of RLE for each input string
  for (int i = 0; i < input.size(); i++) {
    // Print the origional input string
    cout << "\nInput string: " << rleData[i].inputString << endl;
    
    // Print the RLE-encoded string
    cout << "RLE String: ";
    for (char letters : rleData[i].rleString) {
      cout << letters;
    }
    cout << endl;
    
    // Print the frequencies of the encoded characters
    cout << "RLE Frequencies: ";
    for (int numbers : rleData[i].rleFreq) {
      cout << numbers << " ";
    }
    cout << endl;
  }
  
  return 0;
}