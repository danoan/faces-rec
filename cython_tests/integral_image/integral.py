#coding: utf-8

import integral_image as ii

wb1 = [0,0,1,1,2,2,3,3,5,5]
wb2 = [10,10,11,11,12,12,13,13,10,10]

bb1 = [20,20,21,21,22,22,23,23,15,15]
bb2 = [30,30,31,31,32,32,33,33,20,20]

args = []
args.extend(wb1)
args.extend(bb1)

print ii.setImage("/home/daniel/Projects/faces-rec/dataset/test_images/seinfeld_384_288.pgm");
print ii.integral(10,10,1,1,*args);

# wb1 = [0, 1, 0, 1, 1, 1, 1, 1, 1, 2]
# bb1 = [0, 0, 0, 0, 1, 0, 1, 0, 1, 2]

# args = []
# args.extend(wb1)
# args.extend(bb1)

# print ii.integral("/home/daniel/Projects/faces-rec/dataset/test_images/elaine.pgm",0,0,1,1,*args)
