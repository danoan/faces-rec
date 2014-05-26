import os
import random
import sys

from treinador import windowgen as wg

def selectImages(d,n):
    filenames = random.sample( os.listdir(d), int(n))
    for f in filenames:
        print "mv %s/%s %s/%s/%s" %(d,f,d,"select",f)

def crop(folder_scenes,n):
    for scene_window in wg.get_next_random_image_window(folder_scenes, int(n)):
        print scene_window

if __name__=='__main__':
    if( len(sys.argv)==4 ):
        if( sys.argv[1]=="select"):
            selectImages(sys.argv[2],sys.argv[3])
        elif ( sys.argv[1]=="crop"):
            crop(sys.argv[2],sys.argv[3])

# python src/imageSelector.py select /home/daniel/Projects/faces-rec/dataset/training_images/faces 2600
# python src/imageSelector.py crop /home/daniel/Projects/faces-rec/dataset/training_images/test_non_faces 100
# os.listdir("/home/daniel/Projects/faces-rec/dataset/training_images/faces")