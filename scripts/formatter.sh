#!/bin/bash

FORMATTER=clang-format

if ! command -v $FORMATTER
then
    echo "$FORMATTER could not be found"
    exit
fi

find ../client/ ../server/ ../libs | grep .*[hc]pp | xargs $FORMATTER --style=Google -i
