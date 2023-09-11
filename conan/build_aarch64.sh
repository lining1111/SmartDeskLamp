#!/bin/bash
echo "aarch64 conan build"
rm -rf build/*
conan install . -pr:b=default  -pr:h=aarch64 --build=missing --output-folder=build