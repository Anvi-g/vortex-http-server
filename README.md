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
- [x] **Phase 3: Static File Serving & Security**: Binary file streaming from `./public`, MIME-type mapping, and path traversal protection (`403 Forbidden`).
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

The server starts by default on port `8080` and serves files from `./public`.

### 3. Test

In another terminal:

```bash
# Static HTML file (defaults '/' to '/index.html')
curl -i http://localhost:8080/

# Static CSS stylesheet with MIME type detection
curl -i http://localhost:8080/style.css

# Health check (JSON)
curl -i http://localhost:8080/health

# Path traversal prevention test (403 Forbidden)
curl -i --path-as-is http://localhost:8080/../../etc/passwd

# Not found (404)
curl -i http://localhost:8080/unknown

# Method not allowed (405)
curl -i -X POST http://localhost:8080/
```
