# Multithreaded Run-Length Encoder

This program implements a multithreaded Run-Length Encoder (RLE) that processes multiple-input strings concurrently using POSIX threads.


## Table of Contents

- [Introduction](#introduction)
- [Example](#example)
- [Client-Server Communication](#client-server-communication)
- [Interprocess Synchronization](#interprocess-synchronization)

## Introduction

The Run-Length Encoding (RLE) algorithm compresses consecutive repeated characters in a string by representing them as two characters followed by the count of repetitions. The program reads multiple input strings from the standard input (STDIN). It then creates a separate POSIX thread for each input string to perform the RLE encoding concurrently. Each thread receives the input string to compress, applies the RLE algorithm, and stores the RLE string and frequency array in a memory location accessible by the main thread. After all threads have finished their tasks, the program prints the RLE codes for all input strings along with their respective frequency arrays.

[Click here for main.cpp](main.cpp)

## Example

```plaintext
Input:

WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW
00000000001000000000001110000000000000001100000000000000000001
100000000000000000000000000000001
00000000000000010000000000000000100000000000001000000000000010000000000000


Output:

Input string: WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW
RLE String: WWBWWBBWWBWW
RLE Frequencies: 12 12 3 24 14

Input string: 00000000001000000000001110000000000000001100000000000000000001
RLE String: 00100110011001
RLE Frequencies: 10 11 3 15 2 19

Input string: 100000000000000000000000000000001
RLE String: 1001
RLE Frequencies: 31

Input string: 00000000000000010000000000000000100000000000001000000000000010000000000000
RLE String: 00100100100100
RLE Frequencies: 15 16 13 13 13

```

## Client-Server Communication

This version of the program uses interprocess communication mechanisms in UNIX using sockets. The client program reads multiple input strings from the user and sends them to the server over a network connection. The server performs RLE encoding on the received input strings and sends the RLE results back to the client.

- Client
  - The client program takes multiple input strings and sends them to the server for RLE encoding. It uses a TCP socket to communicate with the server.

[Click here for client.cpp](client.cpp)

- Server
  - The server program listens for incoming client connections on a specified port using sockets. Upon accepting a connection, it reads the size and the input string from the client and performs RLE encoding. The RLE string and frequencies are then sent back to the client.

[Click here for server.cpp](server.cpp)

## Interprocess Synchronization

This version of the program uses mutex semaphores to ensure proper synchronization when accessing shared data. The program uses mutex semaphores to lock and unlock access to shared data (RLE result) when multiple threads try to modify it simultaneously, ensuring proper synchronization.

[Click here for main.cpp](smain.cpp)
