import requests
import threading
import time

URL = "http://127.0.0.1:8080/kvstore/create"

TOTAL_REQUESTS = 10000
NUM_THREADS = 100  # number of concurrent threads
REQUESTS_PER_THREAD = TOTAL_REQUESTS // NUM_THREADS

# Generate dummy key-value pairs
def generate_request_data(start, count):
    return [f"key{start + i} value{start + i}" for i in range(count)]

# Single user worker function
def worker(requests_data):
    for line in requests_data:
        try:
            requests.post(URL, data=line, headers={"Content-Type": "text/plain"}, timeout=5)
        except Exception as e:
            print(f"Request error: {e}")

threads = []
start_time = time.time()

# Start threads
for i in range(NUM_THREADS):
    start_index = i * REQUESTS_PER_THREAD
    data = generate_request_data(start_index, REQUESTS_PER_THREAD)
    t = threading.Thread(target=worker, args=(data,))
    t.start()
    threads.append(t)

# Wait for all threads to finish
for t in threads:
    t.join()

end_time = time.time()
print(f"Completed {TOTAL_REQUESTS} requests in {end_time - start_time:.2f} seconds")
print(f"Requests/sec: {TOTAL_REQUESTS / (end_time - start_time):.2f}")
