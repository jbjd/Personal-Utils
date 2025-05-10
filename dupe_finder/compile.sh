#!/bin/sh

rustc ./src/main.rs --crate-type bin --edition 2021 -o dupe_finder -C "opt-level=3" -C "target-cpu=native" -C embed-bitcode=no -C strip=debuginfo
