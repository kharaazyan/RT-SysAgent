#!/usr/bin/env bash
set -euo pipefail
./sync.sh
read -p "Enter commit name: " x
git add .
git commit -m "$x"
git push
