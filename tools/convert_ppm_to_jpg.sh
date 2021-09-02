#!/bin/bash

for img in ../renders/*.ppm; do
    echo ${img%.*}
    filename=${img%.*}
    convert "$filename.ppm" "$filename.jpg"
done