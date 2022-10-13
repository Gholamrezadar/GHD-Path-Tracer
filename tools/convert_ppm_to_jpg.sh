#!/bin/bash

for img in ../renders/*.ppm; do
    filename=${img%.*}
    echo ${filename/renders/renders\/converted}".jpg"
    convert "$filename.ppm" ${filename/renders/renders\/converted}".jpg"
done