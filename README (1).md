# VPN Application — Systems Engineering Documentation

**Author:** David  
**Course:** C++ Object-Oriented Programming  
**Standard:** C++17  
**Compiler:** g++ 10.2.0  
**Last Updated:** April 2026

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [System Architecture](#system-architecture)
3. [Module Reference](#module-reference)
4. [Class Hierarchy](#class-hierarchy)
5. [Data Flow](#data-flow)
6. [State Machine](#state-machine)
7. [Design Patterns Used](#design-patterns-used)
8. [Security Model](#security-model)
9. [Memory Management](#memory-management)
10. [Error Handling Strategy](#error-handling-strategy)
11. [Test Suite](#test-suite)
12. [Build Instructions](#build-instructions)
13. [Known Limitations](#known-limitations)

---

## Project Overview

This project implements a simulated Virtual Private Network (VPN) application in C++17. It demonstrates secure connection lifecycle management, encrypted data tunneling, user authentication, and multi-client server orchestration.

The application is modular, extensible, and built using object-oriented principles including inheritance, polymorphism, templates, composition, and the Strategy design pattern.

All code lives inside the `seneca` namespace.

### Expected System Output

```
[VPN] Server started
[VPN] Client attempting connection...
[AUTH] User authenticated
[TUNNEL] Secure tunnel established
[DATA] Encrypted packet sent
[DATA] Encrypted packet received
[TUNNEL] Tunnel closed
[VPN] Client disconnected
```

---

## System Architecture

```
┌─────────────────────────────────────────────────────┐
│                    VPNManager                        │
│         (High-level system orchestrator)             │
└────────────────────┬────────────────────────────────┘
                     │ owns
┌────────────────────▼────────────────────────────────┐
│                    VPNServer                         │
│         (Manages multiple connections)               │
│         Dynamic array of Connection*                 │
└────────────────────┬────────────────────────────────┘
                     │ stores (via Connection*)
┌────────────────────▼────────────────────────────────┐
│               ConnectionTpl<T>                       │
│         (Templated connection implementation)        │
│         T = cipher strategy (e.g. AES)              │
└──────┬──────────────┬──────────────┬────────────────┘
       │              │              │
  ┌────▼───┐   ┌──────▼──────┐  ┌───▼────────────┐
  │ Socket │   │  T (Cipher) │  │    Protocol    │
  │  I/O   │   │  Encryption │  │  (Strategy)    │
  └────────┘   └─────────────┘  └────────────────┘
       ▲
┌──────┴──────────────────────────────────────────────┐
│                   VPNClient<T>                       │
│         (Concrete client with credentials)           │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│                   VPNTunnel                          │
│         (Secure data tunnel wrapper)                 │
│         Wraps a Connection* for packet routing       │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│                    VPNUser                           │
│         (Authenticated user with access level)       │
└─────────────────────────────────────────────────────┘
```

---

## Module Reference

### `Connection` — Abstract Interface
**File:** `Connection/Connection.h`  
**Role:** Pure abstract base class defining the interface all VPN connections must implement.

| Method | Signature | Description |
|--------|-----------|-------------|
| `connect` | `virtual void connect() = 0` | Establishes network connection |
| `handshake` | `virtual void handshake() = 0` | Performs protocol handshake |
| `authenticate` | `virtual void authenticate() = 0` | Authenticates the connection |
| `sendData` | `virtual void sendData(const std::string&) = 0` | Sends encrypted data |
| `receiveData` | `virtual void receiveData() = 0` | Receives and decrypts data |
| `disconnect` | `virtual void disconnect() = 0` | Tears down the connection |
| `clone` | `virtual Connection* clone() = 0` | Creates a copy (Prototype pattern) |
| `getName` | `virtual std::string getName() const = 0` | Returns connection identifier |

**Design Decision:** Pure virtual destructor is declared in class and defined `inline` outside the class to satisfy the linker without causing multiple definition errors across translation units.

---

### `ConnectionTpl<T>` — Templated Connection
**Files:** `Connection/ConnectionTpl.h`, `Connection/ConnectionTpl.tpp`  
**Role:** Concrete implementation of `Connection`. Template parameter `T` is the cipher strategy.

**Private Members:**

| Member | Type | Description |
|--------|------|-------------|
| `m_socket` | `Socket` | Handles raw I/O |
| `m_cipher` | `T` | Encryption/decryption engine |
| `m_sessionKey` | `std::string` | Server-issued session key |
| `m_protocol` | `std::unique_ptr<Protocol>` | Pluggable protocol strategy |
| `m_state` | `ConnectionState` | Current lifecycle state |

**State Machine:**
```
Disconnected → Connected → Handshaken → Authenticated
```

Each method guards against invalid state transitions using `ConnectionState` enum and throws `std::logic_error` on violations.

**Protected Members:**

| Member | Description |
|--------|-------------|
| `buildCredentials()` | Pure virtual — derived class supplies credentials |
| `getStateName()` | Returns human-readable state for error messages |

---

### `VPNClient<T>` — Concrete Client
**File:** `Client/VPNClient.h`  
**Role:** Extends `ConnectionTpl<T>` with username/password credentials.

**Private Members:**

| Member | Type | Description |
|--------|------|-------------|
| `m_username` | `std::string` | Client username |
| `m_password` | `std::string` | Client password |

**Constructor Validation:**  
Empty username is rejected at construction time via initializer list:
```cpp
m_username(username.empty() ? throw std::invalid_argument("username cannot be empty") : username)
```

**Overrides:**

| Method | Behavior |
|--------|----------|
| `buildCredentials()` | Returns `Credentials{m_username, m_password}` |
| `getName()` | Returns `m_username` |
| `clone()` | Returns `new VPNClient<T>(m_username, m_password)` |

**Design Decision:** `authenticate()` is NOT overridden — the parent handles the full authentication flow using credentials supplied by `buildCredentials()`. This avoids double-sending credentials.

---

### `VPNServer` — Multi-Client Server
**Files:** `Server/VPNServer.h`, `Server/VPNServer.cpp`  
**Role:** Manages a dynamic array of active `Connection*` pointers and an authentication database.

**Private Members:**

| Member | Type | Description |
|--------|------|-------------|
| `m_connections` | `Connection**` | Dynamic array of connection pointers |
| `m_capacity` | `size_t` | Current array capacity (starts at 8, doubles on overflow) |
| `m_size` | `size_t` | Number of active connections |
| `m_authDatabase` | `Credential*` | Dynamic array of valid credentials |
| `m_authDatabaseSize` | `size_t` | Number of stored credentials |

**Rule of 5 Implementation:**

| Method | Strategy |
|--------|----------|
| Copy constructor | Deep clone via `Connection::clone()` |
| Copy assignment | Self-check → delete old → deep clone |
| Move constructor | Transfer pointers → null source |
| Move assignment | Self-check → delete old → transfer → null source |
| Destructor | `delete` each connection, then `delete[]` arrays |

**Capacity Management:**  
When `m_size == m_capacity`, the array doubles in capacity. Existing pointers are transferred (not cloned) — ownership is preserved.

**`removeConnection()` Design:**  
Uses `Connection::getName()` via base pointer — no downcasting required. This works for any connection type regardless of cipher template parameter.

---

### `Protocol` — Abstract Strategy
**File:** `protocol/protocol.h`  
**Role:** Defines the interface for VPN protocol strategies (Strategy Pattern).

| Method | Description |
|--------|-------------|
| `createHandshake()` | Generates opening handshake message |
| `createAuthMessage(Credentials)` | Formats credentials for wire transmission |
| `processAuthResponse(string)` | Validates server response, extracts session key |

---

### `BasicProtocol` — Concrete Protocol
**Files:** `protocol/BasicProtocol.h`, `protocol/BasicProtocol.cpp`  
**Role:** Simple protocol implementation for testing and basic use.

**Wire Format:**
```
Handshake:  "Hello"
Auth:       "AUTH:username:password"
Response:   "OK:SESSION-KEY-ABC123"  (success)
            anything else            (failure)
```

`processAuthResponse()` uses `std::optional<std::string>` — returns session key on success, `std::nullopt` on failure.

---

### `Socket` — Network I/O
**Files:** `Socket/Socket.h`, `Socket/Socket.cpp`  
**Role:** Simulates TCP socket behavior for the VPN connection lifecycle.

**Simulation Behavior:**  
`receiveData()` uses a `static` call counter to alternate between returning handshake acknowledgment and session key responses — simulating a real server conversation.

**Copy semantics:** Copy constructor and copy assignment are deleted. Each `ConnectionTpl` owns its own `Socket` instance — sockets are not shared.

---

### `AES` — Cipher Implementation
**Files:** `AES/AES.h`, `AES/AES.cpp`  
**Role:** Simulated AES cipher using XOR encryption.

| Method | Description |
|--------|-------------|
| `setKey(string)` | Stores session key — throws if empty |
| `encrypt(string)` | XOR plaintext with repeating key |
| `decrypt(string)` | XOR again (XOR is self-inverse) |

**Mathematical Property:**  
XOR encryption is self-inverse: `decrypt(encrypt(x)) == x`.  
This means `decrypt()` simply calls `encrypt()` — mathematically correct.

**Template Compatibility:**  
Copy constructor and copy assignment are defaulted — required because `ConnectionTpl` resets the cipher via `m_cipher = T{}`.

---

### `VPNUser` — Authenticated User
**Files:** `User/VPNUser.h`, `User/VPNUser.cpp`  
**Role:** Represents an authenticated VPN user with access control.

**Access Levels (public enum):**
```cpp
enum class AccessLevel { GUEST, USER, ADMIN };
```

**Session States (private enum):**
```cpp
enum class SessionState { LOGGED_IN, LOGGED_OUT };
```

`SessionState` is private — internal implementation detail.  
`AccessLevel` is public — callers need it to invoke `hasPermission()`.

| Method | Behavior |
|--------|----------|
| `login()` | Returns false if already logged in, else sets state and returns true |
| `logout()` | Sets state to LOGGED_OUT if not already |
| `hasPermission(level)` | Returns `m_access >= required` |

---

### `VPNTunnel` — Secure Data Tunnel
**Files:** `Tunnel/VPNTunnel.h`, `Tunnel/VPNTunnel.cpp`  
**Role:** Wraps a `Connection*` to provide packet-level tunnel abstraction.

**Design Decision:** `VPNTunnel` does NOT encrypt — it delegates to `Connection::sendData()` which already encrypts. Double encryption would corrupt data.

| Method | Guard Condition |
|--------|----------------|
| `openTunnel()` | Silent if already open |
| `closeTunnel()` | Silent if already closed |
| `sendPacket()` | Throws if tunnel closed or connection is nullptr |
| `isOpen()` | Returns `m_isOpen` directly |

---

### `VPNManager` — System Orchestrator
**Files:** `VPNManager/VPNManager.h`, `VPNManager/VPNManager.cpp`  
**Role:** High-level orchestrator. Owns a `VPNServer` by value and manages the full client lifecycle.

**Ownership Model:**  
`VPNManager` owns `VPNServer` by value. `VPNServer` owns `Connection*` pointers. When `VPNManager` is destroyed, the server is destroyed, which deletes all connections.

**`addClient()` Flow:**
```
1. enforcePolicy(level)     — check access level first (fail fast)
2. new VPNClient<AES>       — create client
3. connect()                — establish socket
4. handshake()              — verify server
5. authenticate()           — prove identity
6. acceptConnection()       — transfer ownership to server
```

**`enforcePolicy()`:**  
Private method — internal gate. Currently allows `GUEST` and above. Change minimum level to `USER` to restrict guest access.

---

## Class Hierarchy

```
Connection (abstract)
└── ConnectionTpl<T> (abstract — missing buildCredentials())
    └── VPNClient<T> (concrete)

Protocol (abstract)
└── BasicProtocol (concrete)

VPNServer (concrete, Rule of 5)
VPNTunnel (concrete)
VPNUser   (concrete)
VPNManager (concrete, owns VPNServer by value)
```

---

## Data Flow

### Authentication Flow
```
VPNClient          ConnectionTpl          Socket            Protocol
    │                    │                  │                  │
    │  connect()         │                  │                  │
    │──────────────────► │  m_socket.connect()                 │
    │                    │─────────────────►│                  │
    │  handshake()       │                  │                  │
    │──────────────────► │  createHandshake()                  │
    │                    │────────────────────────────────────►│
    │                    │◄────────────────────────────────────│
    │                    │  sendData(hello)                    │
    │                    │─────────────────►│                  │
    │                    │  receiveData()   │                  │
    │                    │◄─────────────────│                  │
    │  authenticate()    │                  │                  │
    │──────────────────► │  buildCredentials()                 │
    │◄─────────────────  │                  │                  │
    │  {user, pass}      │  createAuthMessage(creds)           │
    │                    │────────────────────────────────────►│
    │                    │◄────────────────────────────────────│
    │                    │  sendData(authMsg)                  │
    │                    │─────────────────►│                  │
    │                    │  receiveData()   │                  │
    │                    │◄─────────────────│                  │
    │                    │  processAuthResponse()              │
    │                    │────────────────────────────────────►│
    │                    │◄────────────────────────────────────│
    │                    │  setKey(sessionKey)                 │
    │                    │  m_state = Authenticated            │
```

### Data Transmission Flow
```
sendData(plaintext)
    │
    ├── Guard: m_state == Authenticated?    → throw if not
    ├── Guard: plaintext.empty()?           → throw if empty
    ├── m_cipher.encrypt(plaintext)         → ciphertext
    └── m_socket.sendData(ciphertext)       → wire
```

---

## State Machine

```
                    ┌─────────────────┐
                    │  Disconnected   │◄──────────────────┐
                    └────────┬────────┘                   │
                             │ connect()                  │
                    ┌────────▼────────┐                   │
                    │   Connected     │                   │
                    └────────┬────────┘                   │
                             │ handshake()                │
                    ┌────────▼────────┐                   │
                    │   Handshaken    │                   │
                    └────────┬────────┘                   │
                             │ authenticate()             │ disconnect()
                    ┌────────▼────────┐                   │
                    │  Authenticated  │───────────────────┘
                    └─────────────────┘
```

**Guard rules:**
- `connect()` — requires `Disconnected`
- `handshake()` — requires `Connected`
- `authenticate()` — requires `Handshaken`
- `sendData()` / `receiveData()` — require `Authenticated`
- `disconnect()` — works from any state, silently returns if already disconnected

---

## Design Patterns Used

| Pattern | Where | Purpose |
|---------|-------|---------|
| **Strategy** | `Protocol` / `BasicProtocol` | Swap protocol behavior without changing connection code |
| **Template Method** | `ConnectionTpl::authenticate()` calls `buildCredentials()` | Parent defines algorithm, child supplies data |
| **Prototype** | `Connection::clone()` | Deep copy connections without knowing concrete type |
| **Polymorphism** | `Connection*` in `VPNServer` | Store any connection type in one array |
| **RAII** | `std::unique_ptr<Protocol>` | Automatic protocol lifetime management |
| **Fail Fast** | Constructor validation, state guards | Catch errors at the earliest possible point |

---

## Security Model

### Encryption
- XOR cipher with session key (simulation)
- Real deployment would use AES-256-GCM or ChaCha20-Poly1305
- Session key is server-issued after authentication

### Authentication
- Credentials formatted by protocol layer
- Server validates and returns session key on success
- Failed auth throws `std::runtime_error`

### Access Control
- Three-tier access: `GUEST < USER < ADMIN`
- `VPNManager::enforcePolicy()` gates client admission
- `VPNUser::hasPermission()` checks per-operation access

### Ownership Safety
- `VPNServer` takes ownership of `Connection*` via `acceptConnection()`
- Callers must not use raw pointer after transfer
- `nullptr` connections are rejected with `std::invalid_argument`

---

## Memory Management

### Dynamic Allocations

| Class | Allocation | Freed By |
|-------|-----------|----------|
| `VPNServer::m_connections` | `new Connection*[capacity]` | `~VPNServer()` |
| `VPNServer::m_authDatabase` | `new Credential[capacity]` | `~VPNServer()` |
| Each `Connection*` in array | `new VPNClient<T>(...)` | `removeConnection()` or `~VPNServer()` |
| `ConnectionTpl::m_protocol` | `make_unique<BasicProtocol>()` | Automatic via `unique_ptr` |

### Capacity Growth Strategy
```
Initial capacity: 8
Growth factor:    2x (doubles when full)
Shrink policy:    None (capacity never reduces)
```

### Copy vs Move
- **Copy** deep-clones all connections via `clone()`
- **Move** transfers raw pointers and nulls the source
- `Socket` is not copyable — each connection owns exactly one socket

---

## Error Handling Strategy

### Exception Types Used

| Exception | When Used |
|-----------|-----------|
| `std::logic_error` | Invalid state transition (wrong order of operations) |
| `std::runtime_error` | Authentication failure, socket errors |
| `std::invalid_argument` | Bad input (empty username, nullptr connection, empty data) |

### Guard Pattern
Every stateful method follows this pattern:
```cpp
void method() {
    if (m_state != RequiredState) {
        throw std::logic_error(getName() + ": message. State: " + getStateName());
    }
    // actual work
}
```

### Fail Fast Principle
- Empty username rejected at constructor time
- nullptr connection rejected before array insertion
- Empty data rejected before encryption attempt
- State violations rejected before any side effects

---

## Test Suite

**File:** `Tests/tests.cpp`

### Unit Tests

| Test | What It Verifies |
|------|-----------------|
| `test_double_disconnect()` | Disconnecting twice does not crash |
| `test_wrong_password()` | Invalid credentials are rejected |
| `test_server_capacity()` | Server correctly grows beyond initial capacity of 8 |
| `test_send_before_auth()` | Cannot send data without authenticating first |

### Edge Case Tests

| Test | What It Verifies |
|------|-----------------|
| `test_empty_username()` | Empty username throws `invalid_argument` at construction |
| `test_double_authenticate()` | Second authenticate() throws — state guard works |
| `test_remove_nonexistent()` | Removing missing client is handled gracefully |
| `test_null_connection()` | nullptr connection throws `invalid_argument` |
| `test_empty_data()` | Sending empty string throws `invalid_argument` |

### Test Pattern (AAA)
Every test follows Arrange → Act → Assert:
```cpp
void test_example() {
    // ARRANGE
    VPNClient<AES> client("user", "pass");
    try {
        // ACT
        client.someMethod();
        // ASSERT — reached if no throw
        std::cout << "PASS/FAIL\n";
    } catch (const std::exception& e) {
        // ASSERT — reached if throw
        std::cout << "PASS/FAIL: " << e.what() << "\n";
    }
}
```

---

## Build Instructions

### Compile (Matrix)
```bash
/usr/local/gcc/10.2.0/bin/g++ -Wall -std=c++17 -g -o vpn \
  -I./AES -I./Client -I./Connection \
  -I./protocol -I./Server -I./Socket \
  -I./Tunnel -I./User -I./VPNManager \
  AES/AES.cpp \
  Client/*.cpp \
  protocol/BasicProtocol.cpp \
  Server/VPNServer.cpp \
  Socket/Socket.cpp \
  Tunnel/VPNTunnel.cpp \
  User/VPNUser.cpp \
  VPNManager/VPNManager.cpp \
  main.cpp
```

### Memory Leak Check
```bash
valgrind --show-error-list=yes \
         --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./vpn
```

### Compile Tests
```bash
/usr/local/gcc/10.2.0/bin/g++ -Wall -std=c++17 -g -o vpn_tests \
  -I./AES -I./Client -I./Connection \
  -I./protocol -I./Server -I./Socket \
  AES/AES.cpp \
  Client/*.cpp \
  protocol/BasicProtocol.cpp \
  Server/VPNServer.cpp \
  Socket/Socket.cpp \
  Tests/tests.cpp
```

---

## Known Limitations

| Limitation | Impact | Real-World Fix |
|-----------|--------|----------------|
| `Socket::receiveData()` uses static call counter | Breaks with multiple simultaneous clients | Real TCP socket per connection |
| XOR cipher is not cryptographically secure | Data not truly encrypted | AES-256-GCM or ChaCha20-Poly1305 |
| No real network I/O | Simulation only | POSIX sockets or Boost.Asio |
| `enforcePolicy()` allows GUEST level | Weakest access can connect | Change minimum to `USER` |
| No rekeying strategy | Session key never rotates | Implement periodic rekeying in `Protocol` |
| No thread safety | Single-threaded only | `std::mutex` on `VPNServer` operations |
| `Packet` module incomplete | `VPNTunnel::sendPacket()` depends on external type | Complete `Packet` module implementation |
