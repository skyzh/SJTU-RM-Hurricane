#!/bin/bash

set -e # Exit with nonzero exit code if anything fails
cd "${0%/*}"

./blueutil --connect 98-d3-33-81-3b-01
./blueutil --paired
minicom

