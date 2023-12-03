#!/bin/bash

for i in {1..100}; do
    # Run curl command in the background, discarding output
    curl -s -o /dev/null http://localhost:9999 > /dev/null 2>&1 &
done
