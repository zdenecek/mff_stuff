

# Solution to gRPC assignment

## How to run

0. setup python environment

1. install deps
    ```
    pip install -r requirements.txt
    ```
2. compile proto files
   ```
   python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. *.proto
   ```
3. run script
   ```
   python 03.py
   ```

## Description of the solutions

### 1 - trivial

Solution is trivial. My UCO is 78002598, that is why I chose to send that number.

### 2 - groceries

Again somewhat trivial. I implemented the sequential program to do step by step what it was asked to do.

### 3 - guessing game

Implemented bi directional client using asyncio with `await call.done_writing()`

### 4 - window average

Implemented a basic client which in the finally clause explicitly closes the channel.