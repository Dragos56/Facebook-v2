# VirtualSoc

VirtualSoc is an academic client-server social network application developed in C. It demonstrates core software development concepts including networking, multithreading, database management, and user interface design. The project simulates basic social network functionality, allowing multiple clients to interact with the server concurrently.

## Features

* Separate server and client applications
* User account creation and authentication
* Friend requests and connections
* Post feed with basic user interaction
* Interactive graphical interface (UI)
* Persistent data storage using SQLite
* Network communication via sockets
* Multithreaded server handling multiple clients simultaneously

## Technologies Used

* C programming language
* Client–server communication with sockets
* Multithreading with `pthread`
* SQLite3 database
* Graphical interface (UI) using GLFW 3 and ImGui
* File handling and structured data management

## How to Use (Linux)

### Requirements

You need glfw3 installed on your machine, to install it use:

```bash
sudo apt-get update
sudo apt-get install libglfw3-dev 
```

### Server

1. Navigate to the `server` folder:

   ```bash
   cd server
   ```
2. Build the server using Makefile:

   ```bash
   make
   ```
3. Run the server with the desired number of threads:

   ```bash
   ./server <number_of_threads>
   ```

### Client

1. Navigate to the `client` folder:

   ```bash
   cd client
   ```
2. Build the client using Makefile:

   ```bash
   make
   ```
3. Run the client application:

   ```bash
   ./app
   ```

## What I Learned

* Implementing client-server architecture using sockets
* Managing multiple clients concurrently with multithreading
* Handling user data and relationships using SQLite3
* Designing and implementing a graphical interface with GLFW3 and ImGui
* Structuring and debugging a complex C project
* Building and running C projects wit
