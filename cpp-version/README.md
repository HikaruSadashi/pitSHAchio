# PitSHAchio: A Distributed Password Cracking Program

## Introduction

PitSHAchio is a distributed password cracking program designed to brute force password hashes using a distributed computing approach. The project utilizes a main client written in full-stack JavaScript to provide a user-friendly web-based UI, communicating with a server built in C++ via IPC sockets. The server, in turn, interacts with compute clients also built in C++ using TCP sockets. This README provides an overview of the project's goals, approach, and framework.

## Goal

The primary goal of PitSHAchio is to crack password hashes efficiently through distributed computing. The client inputs a password consisting of numbers and letters, which is then hashed and sent to the server. The server, utilizing a bijective mapping from ASCII characters to numbers, computes potential passwords and responds with a list of possible matches

## Approach

1. Encoding: All strings are converted into encoded numbers using a bijective mapping.
2. Distribution: Compute clients are assigned portions of integers mod N to compute, skipping over every N strings to test them against the hash.
3. Matching: If a client finds a matching hash, it sends the result back to the server.

## Framework

1. Main Client: Full-stack JavaScript providing a web-based UI for user interaction.
2. Server: Built in C++, handling communication with the main client and compute clients via IPC and TCP sockets, respectively.
3. Compute Clients: Also built in C++, tasked with computing portions of the password space and testing against the hash.

## Usage

1. Start the web server
```bash
cd webserver
npm run build
npm run start & disown
```
2. Start the compute server using the provided C++ executable.
```bash
cd ../computeserver
./ComputeServer & disown
```
3. Make sure there is at least one compute client
```bash
cd  ../computeclient
./Client
```
  - if you are running the client on a different device, please provide the IP address of the server
  - `./Client 1.2.3.4`
4. Access the web-based UI provided by the webserver at port 3000
5. Enter a hash to be reversed, or use the passworder converter to get a hash first.
6. View the results and potential password match provided by the server.

## Dependencies

- Node.js
  - express
  - js-sha256
  - net
  - react
- g++
  - openssl



