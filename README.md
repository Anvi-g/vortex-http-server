# Vortex HTTP Server

A lightweight, high-performance HTTP/1.1 server built in modern C++17 from scratch using POSIX sockets.

## Features

- **POSIX Sockets**: Low-level TCP network socket handling (`socket`, `bind`, `listen`, `accept`).
- **RAII Resource Management**: Safe socket lifecycle management ensuring deterministic cleanup and `SO_REUSEADDR` port reuse.
- **HTTP/1.1 Protocol**: Baseline HTTP response formatting (`200 OK` with standard headers).
- **Modern C++17**: Clean class design, memory safety, and standard compliance.

## Project Roadmap

- [x] **Phase 1: RAII TCP Server**: Socket initialization, connection accept loop, and basic HTTP response.
- [x] **Phase 2: HTTP Parser & Routing**: Request parsing (method, path, version, headers), structured response serializer, and dynamic endpoint routing.
- [ ] **Phase 3: Static File Serving**: Serving HTML, CSS, JS, and media files with MIME-type handling.
- [ ] **Phase 4: Multithreading & Concurrency**: Thread pool architecture for concurrent client handling.
- [ ] **Phase 5: Performance & Benchmarking**: Keep-alive connection management and load testing.

## Prerequisites

- **C++ Compiler**: GCC or Clang supporting C++17
- **Build System**: CMake >= 3.16
- **Operating System**: Linux / POSIX-compliant OS

## Build & Run

### 1. Build

```bash
cmake -B build
cmake --build build
```

### 2. Run

```bash
./build/vortex
```

The server starts by default on port `8080`.

### 3. Test

In another terminal:

```bash
# Root endpoint (HTML)
curl -i http://localhost:8080/

# Health check (JSON)
curl -i http://localhost:8080/health

# Not found (404)
curl -i http://localhost:8080/unknown

# Method not allowed (405)
curl -i -X POST http://localhost:8080/
```
