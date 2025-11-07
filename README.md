# CS744 Project: C++ RESTful Key-Value Store

This project is a high-performance, REST-based key-value store server built in C++. It features a three-tier architecture with an HTTP API layer, a thread-safe in-memory LRU cache, and a persistent PostgreSQL database with PgBouncer for connection pooling.

## Architecture

The system follows a three-tier design to ensure fast reads via caching and reliable persistence via PostgreSQL.

---

## Core Components

The architecture is broken down into the following layers:

### 1. HTTP Layer (API Server)
* **Description:** Handles all incoming client requests and HTTP routing.
* **Library:** Built using the C++ [**httplib**](https://github.com/yhirose/cpp-httplib) library to expose REST API endpoints.
* **Concurrency:** Uses `httplib::ThreadPool` with 20 worker threads for concurrent request handling.
* **Routing:** Routes each endpoint to its specific handler function (`handle_create`, `handle_read`, `handle_update`, `handle_delete`).

### 2. Cache Layer (In-Memory LRU)
* **Description:** Speeds up read operations by storing frequently accessed data in memory.
* **Implementation:** A custom LRU (Least Recently Used) cache built using `std::list` and `std::unordered_map`.
* **Thread Safety:** All cache operations (`read_key`, `create_key`, `delete_key`, `update_key`) are thread-safe (assuming `kvcache.h` implements this, e.g., with `std::mutex`).
* **Eviction Policy:** When the cache exceeds its capacity, the least recently used entry is evicted.

### 3. PgBouncer (Connection Pool)
* **Description:** Manages a pool of connections to the PostgreSQL database.
* **Purpose:** Efficiently reuses connections from multiple clients, reducing the overhead of establishing new connections for every request and improving performance. The server connects to port 6432 to interface with the pooler.

### 4. Database Layer (Persistent Storage)
* **Description:** Provides durable storage and consistency for all key-value pairs.
* **Database:** PostgreSQL.
* **Library:** Uses the [**pqxx**](https://github.com/libpqxx/libpqxx) library for C++ to interface with PostgreSQL.
* **Transactions:** All database operations are wrapped in transactional units (`pqxx::work`) to ensure atomic operations.

---

## API Endpoints

The server exposes the following RESTful endpoints:

* `POST /kvstore/create`
    * **Body:** `"key value"` (space-separated string)
    * **Action:** Adds a new key-value pair.
    * **Workflow:** Checks if the key already exists (returns `409 Conflict` if true). Otherwise, the pair is first written to the PostgreSQL database. If successful, it is then added to the in-memory cache.

* `GET /kvstore/<key>`
    * **Action:** Reads the value associated with a given `<key>`.
    * **Workflow:** The server first attempts to read the value from the cache. If a cache hit occurs, it returns the value. If a cache miss occurs, it fetches the value from the database. If found, it updates the cache and returns the value (returns `404 Not Found` if not in DB).

* `PUT /kvstore/<key>`
    * **Body:** `"new_value"`
    * **Action:** Updates the value for an existing key.
    * **Workflow:** The server updates the value in the database. If the key is not found, it returns `404 Not Found`. If the DB update is successful, it updates the corresponding entry in the cache (or creates it if it wasn't already in the cache).

* `DELETE /kvstore/<key>`
    * **Action:** Removes a key-value pair from the system.
    * **Workflow:** The entry is first deleted from the database. If the key is not found, it returns `404 Not Found`. If the DB deletion is successful, the key is also evicted from the cache.

---

## How to Build and Run

### Prerequisites
* A C++ compiler (e.g., g++, clang)
* CMake (v3.10 or higher)
* PostgreSQL
* PgBouncer
* `libpqxx` (PostgreSQL C++ API)
* `httplib.h` (included)
* `kvcache.h` (your custom cache implementation)
