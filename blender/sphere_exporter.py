import bpy
import math

with open("spheres.txt", "w") as f:
    print("x, y, z, R")
    for i in bpy.context.selected_objects:
        # debug
        print(str(round(i.location.x, 4)) + ", " +
              str(round(i.location.y, 4)) + ", " +
              str(round(i.location.z, 4)) + ", " +
              str(round(i.scale.x,4)))
        # write to file
        f.write(str(round(i.location.x, 4)) + ", " +
              str(round(i.location.y, 4)) + ", " +
              str(round(i.location.z, 4)) + ", " +
              str(round(i.scale.x,4))+ "\n")