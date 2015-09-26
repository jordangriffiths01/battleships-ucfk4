#!/bin/sh

# File: build_project.sh

rm Makefile Makefile.test
cd ../../etc
make
cd ../assignment/final
make program
