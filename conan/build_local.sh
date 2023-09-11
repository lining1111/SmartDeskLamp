#!/bin/bash
echo "local conan build"
rm -rf build/*
conan install . --build=missing --output-folder=build