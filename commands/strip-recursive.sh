#!/bin/sh

function stripeach {
    while read line; do
        for f in "$@"; do
            echo "stripping $line with $1";
            strip $1 $line
        done
    done
}

find . -regex '.*\.\(dll\|exe\|pyd\)' | stripeach "$@"