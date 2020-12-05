#!/bin/bash
set -euo pipefail
apt install -y mesa-common-dev
git submodule init
git submodule sync
git submodule update
