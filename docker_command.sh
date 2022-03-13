#!/bin/bash

if [ ! $# -eq 0 ]; then
    docker run --rm -v "$(PWD)":/current-workspace -w "/current-workspace" cross-compiler $@
else
    echo "Error: need a command"
fi
