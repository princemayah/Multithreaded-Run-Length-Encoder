#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <vector>

using namespace std;

// Struct to hold RLE data
struct Data {
  string inputString;
  string rleString;
  vector<int> rleFreq;
};

// Function for RLE encoding
void RLE(Data* rleData) {
  string& inputString = rleData->inputString;
  string& rleString = rleData->rleString;
  vector<int>& rleFreq = rleData->rleFreq;
  
  int count = 1;
  
  // Perform RLE encoding
  for (int i = 0; i < inputString.size(); i++) {
    if (inputString[i] == inputString[i + 1]) {
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
}

// Signal handler for reaping child processes
void fireman(int) {
  while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  signal(SIGCHLD, fireman);
  
  // Check if port number is provided
  if (argc < 2) {
    std::cerr << "ERROR, no port provided\n";
    exit(1);
  }
  
  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "ERROR opening socket";
    exit(1);
  }
  
  // Initialize server address structure
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  
  // Bind the socket to the specified address and port
  if (bind(sockfd, (struct sockaddr *)&serv_addr,
    sizeof(serv_addr)) < 0) {
      std::cerr << "ERROR on binding";
      exit(1);
    }
    
    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    // Accept and handle client connections
    while (true) {
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
      if (fork() == 0) {
        // Child process
        
        if (newsockfd < 0) {
          std::cerr << "ERROR on accept";
          exit(1);
        }
          
        while (true) {
          int size;
          // Read the size of the input string from the client
          n = read(newsockfd, &size, sizeof(int));
          if (n < 0) {
            std::cerr << "ERROR reading from socket";
            exit(1);
          }
          if (size == 0) {
            break;
          }

          // Allocate memory for the input string
          char *buffer = new char[size + 1];
          bzero(buffer, size + 1);
          // Read the input string from the client
          n = read(newsockfd, buffer, size);
          if (n < 0) {
            std::cerr << "ERROR reading from socket";
            exit(1);
          }

          // Print the input string
          std::cout << "Input string: " << buffer << std::endl;

          // RLE encoding
          Data rleData;
          rleData.inputString = buffer;
          RLE(&rleData);

          // Print the RLE string
          std::cout << "RLE String: " << rleData.rleString << std::endl;

          // Print the RLE frequencies
          std::cout << "RLE Frequencies: ";
          for (int freq : rleData.rleFreq) {
            std::cout << freq << " ";
          }
          std::cout << std::endl;
          
          // Send the RLE string back to the client
          n = write(newsockfd, &(rleData.rleString), rleData.rleString.size());
          if (n < 0) {
            std::cerr << "ERROR writing to socket";
            exit(1);
          }
          
          // Deallocate memory for the buffer
          delete[] buffer;
        }
        
        // Close the client socket and exit the child process
        close(newsockfd);
        _exit(0);
      }
  }
  
  // Close the server socket
  close(sockfd);
  return 0;
}
