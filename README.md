# Building a scalable chat application in c++ Using async io

## 📝 Project Overview

`chatRoomCpp` is a simple yet powerful chat room application implemented in **C++**, leveraging the **Boost.Asio** library for asynchronous network communication and **threading** for managing concurrent client sessions.

**What it can do:-**

- Handle multiple clients simultaneously  
- Enable non-blocking communication  
- Efficiently manage memory and concurrency

## ✨ Key Features

- **Asynchronous Communication:** Non-blocking I/O with Boost.Asio.  
- **Multi-threading:** Separate threads to manage IO and user input.  
- **Room Management:** Centralized management of participants.  
- **Message Handling:** Efficient encoding and decoding of messages.  

---

## 🏛️ Architecture Overview

The project is structured around a few key classes:

| **Component** | **Responsibility** |
|---------------|--------------------|
| **Session**   | Represents a client session. Manages socket communication. |
| **Room**      | Manages multiple client sessions. Broadcasts messages. |
| **Message**   | Handles encoding/decoding of chat messages. |
| **Server**    | Accepts incoming client connections. |
| **Client**    | Connects to server, handles user input/output. |

Each client connects to the server, joins a room, and can send and receive broadcasted messages asynchronously.

## 🔥 Some Functionalities Explained

### 1. Asynchronous I/O

- Boost.Asio's `async_read` and `async_write` functions enable non-blocking reads and writes over sockets.
- Operations are **initiated but completed later**, triggering a **completion handler** (callback).
- Server continues handling other clients while one client’s I/O is pending.

**Example Principle:**
```cpp
boost::asio::async_read(socket, buffer, completion_handler);
boost::asio::async_write(socket, buffer, completion_handler);
```

### 2. TCP Sockets

- A **TCP socket** represents a single network connection between server and client.
- **Server** listens on a TCP port → accepts new sockets for incoming connections.
- **Client** connects to server socket.
- Communication happens by reading/writing raw bytes through the socket.

**Example Principle:**
```cpp
boost::asio::ip::tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
acceptor.async_accept(new_socket, accept_handler);
```

### 3. Threading Model

- Server runs the `io_context.run()` loop on the **main thread**.
- Client spawns a **dedicated thread** for handling **user input** while I/O operations are managed asynchronously on another thread.

---

### 4. Message Management

- Each message includes a **header** and a **body**.
- Encoding/decoding ensures proper message framing during TCP communication (since TCP is a continuous stream).

---

## 🧠 In-Depth: Boost.Asio Concepts Used

| **Boost.Asio Concept**   | **Usage**                                                                 |
|--------------------------|---------------------------------------------------------------------------|
| `async_read_until()`     | Read data asynchronously until a delimiter or length is reached.         |
| `async_write()`          | Write buffers asynchronously.                                            |
| `io_context.post()`      | Post functions to run in the I/O context's event loop.                   |
| **Completion Handlers**  | Custom callbacks that execute after I/O operations complete.             |

### Important Practice:
- Always use `shared_from_this()` inside handlers to extend session object's lifetime during async operations.






## 🔥 Message Flow Explained

Client takes user input (e.g., a chat message).

Client asynchronously writes to socket.

Server reads data from socket asynchronously.

Server parses the incoming data into a Message object.

Server uses Room to broadcast Message to all Sessions.

Each Session queues the Message and asynchronously writes it to the respective client's socket.



## 📐 Low-Level Design (LLD)

                                        
                                        
                                        +---------------------+           +---------------------+
                                        |      Client 1       |           |      Client 2       |
                                        |---------------------|           |---------------------|
                                        | - User Input        |           | - User Input        |
                                        | - Display Messages  |           | - Display Messages  |
                                        | - Send Messages     |           | - Send Messages     |
                                        +----------+----------+           +----------+----------+
                                                   |                                 |
                                                   |                                 |
                                                   v                                 v
                                              +----+---------------------------------+----+
                                              |           Chat Server (chatApp)           |
                                              |-------------------------------------------|
                                              | - Accept Connections                      |
                                              | - Manage Sessions                         |
                                              | - Broadcast Messages                      |
                                              +----------------+--------------------------+
                                                               |
                                                               v
                                                       +-------+-------+
                                                       |     Room      |
                                                       |---------------|
                                                       | - Manage      |
                                                       |   Active      |
                                                       |   Sessions    |
                                                       | - Broadcast   |
                                                       |   Messages    |
                                                       +-------+-------+
                                                               |
                                                +--------------+--------------+
                                                |                             |
                                                v                             v
                                        +---------------+           +---------------+
                                        |   Session 1   |           |   Session 2   |
                                        |---------------|           |---------------|
                                        | - Read/Write  |           | - Read/Write  |
                                        |   Messages    |           |   Messages    |
                                        | - Handle      |           | - Handle      |
                                        |   Client I/O  |           |   Client I/O  |
                                        +---------------+           +---------------+
                                        
                                        
                                        
---

## 🛠️ How to Build and Run

1. **Clone the repository:**

    ```bash
    git clone https://github.com/singhdevhub-lovepreet/chatRoomCpp.git
    cd chatRoomCpp
    ```

2. **Build the project:**

    ```bash
    make
    ```

3. **Start the server:**

    ```bash
    ./chatApp <port>
    ```

4. **Start the client (in separate terminal):**

    ```bash
    ./clientApp <port>
    ```
## ✖️ Advanced Coding Practices Highlighted

- **Lifetime Safety with `std::enable_shared_from_this`:**
  - Ensures that async operations keep the `Session` object alive safely.

- **Efficient Message Encoding:**
  - First 4 bytes represent message size, followed by the actual message.

- **Proper Socket Management:**
  - Clean shutdowns of sockets when client disconnects.

- **Thread-Safe Queue Handling:**
  - Message queues are carefully managed per Session to avoid race conditions.

- **Scalable Server Design:**
  - Easy to scale further by adding multiple I/O threads or load balancing connections.



### 🧩 Conclusion

Boost.Asio provides a powerful abstraction for handling asynchronous I/O using TCP sockets. With concepts like non-blocking operations, completion handlers, and threading models, developers can build efficient, responsive networked applications that scale well under load.
