#!/bin/bash
set -e

# Function to handle termination signals
term_handler() {
  echo "Received termination signal. Exiting..."
  exit 0
}

# Trap termination signals
trap 'term_handler' INT TERM

# Generate hashed users before login
/app/generate_hashed_users

while true; do
  /app/login
done