#!/bin/bash

if [ ! $# -eq 0 ]; then
    docker run --rm -v "$(PWD)":/tmp -w "/tmp" cross_compiler $@
else
    echo "Error: need a command"
fi
