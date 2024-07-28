#!/bin/bash

# Compile and time multithreaded.cpp
g++ -o multithread multithread.cpp -pthread -lcurl

start_time=$(date +%s%N)
./multithread
end_time=$(date +%s%N)

compile_time_multithreaded=$((($end_time - $start_time) / 1000000))
echo "multithread: $compile_time_multithreaded ms"

# Compile and time single_threaded.cpp
g++ -o singlethread singlethread.cpp -pthread -lcurl

start_time=$(date +%s%N)
./singlethread
end_time=$(date +%s%N)

compile_time_single_threaded=$((($end_time - $start_time) / 1000000))
echo "singlethread: $compile_time_single_threaded ms"

