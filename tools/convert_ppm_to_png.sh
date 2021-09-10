#!/bin/bash

for img in ../renders/*.ppm; do
    filename=${img%.*}
    echo ${filename/renders/renders\/converted}".png"
    convert "$filename.ppm" ${filename/renders/renders\/converted}".png"
done