#!/usr/bin/env bash
set -euo pipefail

UPSTREAM_URL="https://github.com/kharaazyan/nexus.git"   
UPSTREAM_BRANCH="master"                             

LOCAL_BRANCH=$(git rev-parse --abbrev-ref HEAD)

git remote get-url upstream > /dev/null 2>&1 || \
  git remote add upstream "$UPSTREAM_URL"

git fetch upstream "$UPSTREAM_BRANCH"

git merge --no-ff "upstream/$UPSTREAM_BRANCH" -m "Merge upstream/$UPSTREAM_BRANCH into $LOCAL_BRANCH"

git push origin "$LOCAL_BRANCH"
