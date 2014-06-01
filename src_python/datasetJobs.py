import os
import shutil
import random
import sys
import Image

from treinador import windowgen as wg

def createDirIfNotExist(save_dest):
    if not os.path.exists(save_dest):
        os.makedirs(save_dest)

def match_images(normal,match):
    if normal.size!=match.size:
        return False

    list_w = [i for i in xrange(0,normal.size[0]-1) ]
    list_h = [i for i in xrange(0,normal.size[1]-1) ]

    w = random.sample( list_w, 20 )
    h = random.sample( list_h, 20 )

    for i in xrange(0,20):
        if normal.getpixel( (w[i],h[i]) ) != match.getpixel( (w[i],h[i]) ):
            return False

    return True


#Seleciona uma amostra aleatoria de arquivos um diretorio
def selectImages(d,n):
    filenames = random.sample( os.listdir(d), int(n) )
    for f in filenames:
        print "mv %s/%s %s/%s/%s" %(d,f,d,"select",f)


#Corta n subjanelas utilizando como base todas as imagens presentes em folder_scenes
def crop(folder_scenes,n):
    for scene_window in wg.get_next_random_image_window(folder_scenes, int(n)):
        print scene_window


#Encontra as imagens em src_to_find que casam com as imagens de src_to_match
def imageMatching(src_to_match, src_to_find):
    img_to_match = []
    for f in os.listdir(src_to_match):

        path = "%s/%s" % (src_to_match,f)
        if os.path.isdir(path): continue

        try:
            img_to_match.append( Image.open(path) )
        except:
            print "Erro ao abrir Imagem: %s" % (path,)

    for f in os.listdir(src_to_find):
        path = "%s/%s" % (src_to_find,f)
        if os.path.isdir(path): continue

        i = None
        try:
            i = Image.open(path)
        except:
            print "Erro ao abrir Imagem: %s" % (path,)            
            continue

        for m in img_to_match:
            if match_images(i,m):
                os.remove(path)
                # print "mv %s %s/encontrados/%s" % (path,src_to_match,f)
                print "rm %s" % (path,)

#Cria um conjunto de teste e validacao utlizando os arquivos em db_source
def createTestSet(scene_source,face_source,face_train_number,scene_train_number,
                  face_valid_number,scene_valid_number,crop_train_number,
                  crop_valid_number,salvar_em):
    #Limpa o database da SUN (db_source), retirando as imagens que contem faces.
    print "LIMPANDO SUN DATABASE"
    # src_to_match = "%s/%s" % ( "/".join( scene_source.split("/")[:-1] ),"com_faces")
    # imageMatching(src_to_match,scene_source)

    face_train_number = int(face_train_number)
    face_valid_number = int(face_valid_number)
    scene_train_number = int(scene_train_number)
    scene_valid_number = int(scene_valid_number)

    face_files = os.listdir( face_source )
    scene_files = os.listdir( scene_source )

    total_face = face_train_number+face_valid_number
    total_scene = scene_train_number+scene_valid_number

    used_face_files = random.sample( face_files, total_face )
    used_scene_files = random.sample( scene_files, total_scene )

    face_train_files = used_face_files[:face_train_number]
    face_valid_files = used_face_files[face_train_number:]

    scene_train_files = used_scene_files[:scene_train_number]
    scene_valid_files = used_scene_files[scene_train_number:]

    #Arquivos de treinamento de face ficam em um unico diretorio
    #Arquivos de validacao de face ficam em um unico diretorio

    print "MONTANDO FACES DE TREINAMENTO"
    for f in face_train_files:
        save_src = "%s/%s" % (face_source,f)
        save_dest_dir = "%s/Instancia SUN/training_images/faces" % (salvar_em,)
        save_dest = "%s/%s" % (save_dest_dir,f)
        createDirIfNotExist(save_dest_dir)
        shutil.copyfile(save_src,save_dest)


    print "MONTANDO FACES DE VALIDACAO"
    for f in face_valid_files:
        save_src = "%s/%s" % (face_source,f)
        save_dest_dir = "%s/Instancia SUN/validation_images/faces" % (salvar_em,)
        save_dest = "%s/%s" % (save_dest_dir,f)
        createDirIfNotExist(save_dest_dir)
        shutil.copyfile(save_src,save_dest)        


    #Arquivos de treinamento de cenas sao divididos em 38 grupos de igual numero,
    #pois cada estagio de treinamento usa cenas diferentes

    # print "MONTANDO CENAS DE TREINAMENTO"
    # group = 0
    # factor_group = ( len(scene_train_files)/38 )
    # per_group = [factor_group + (1 if i>=18 else 0) for i in xrange(0,38)]
    # per_group.append( len(scene_train_files) - factor_group*38 )
    # for f in scene_train_files:
    #     if per_group[0]==0:
    #         crop( "%s/Instancia SUN/training_images/non_faces/group_%s" % (salvar_em,group), crop_train_number )
    #         per_group = per_group[1:]
    #         group+=1

    #     save_src = "%s/%s" % (scene_source,f)
    #     save_dest_dir = "%s/Instancia SUN/training_images/non_faces/group_%s" % (salvar_em,group)
    #     save_dest = "%s/%s" % (save_dest_dir,f)
    #     createDirIfNotExist(save_dest_dir)
    #     shutil.copyfile(save_src,save_dest)    

    #     per_group[0]-=1

    # crop( "%s/Instancia SUN/training_images/non_faces/group_%s" % (salvar_em,group), crop_train_number )

    # #Arquivos de validacao ficam em um mesmo diretorio, porem, na mesma estrutura de grupos

    # print "MONTANDO CENAS DE VALIDACAO"
    # group = 0
    # factor_group = ( len(scene_valid_files)/38 )
    # per_group = [factor_group + (1 if i>=18 else 0) for i in xrange(0,38)]
    # per_group.append( len(scene_valid_files) - factor_group*38 )    
    # for f in scene_valid_files:
    #     if per_group[0]==0:
    #         crop( "%s/Instancia SUN/validation_images/non_faces/group_%s" % (salvar_em,group), crop_valid_number )
    #         per_group = per_group[1:]
    #         group+=1

    #     save_src = "%s/%s" % (scene_source,f)
    #     save_dest_dir = "%s/Instancia SUN/validation_images/non_faces/group_%s" % (salvar_em,group)
    #     save_dest = "%s/%s" % (save_dest_dir,f)
    #     createDirIfNotExist(save_dest_dir)
    #     shutil.copyfile(save_src,save_dest)  

    #     per_group[0]-=1

    # crop( "%s/Instancia SUN/validation_images/non_faces/group_%s" % (salvar_em,group), crop_valid_number )


if __name__=='__main__':
    if( len(sys.argv)==4 ):
        if( sys.argv[1]=="select"):
            selectImages(sys.argv[2],sys.argv[3])
        elif ( sys.argv[1]=="crop"):
            crop(sys.argv[2],sys.argv[3])
        elif ( sys.argv[1]=="match"):
            imageMatching(sys.argv[2],sys.argv[3]) 
    elif( len(sys.argv)==11):
        if( sys.argv[1]=="createTestSet"):
            createTestSet( *(sys.argv[2:]) )           


####CREATE TEST SET
# python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/lfw_faces 300 300 100 100 500 200 /home/daniel/Projects/faces-rec/dataset


####MATCH
# python src/datasetJobs.py match "/home/daniel/Projects/faces-rec/dataset/sun/imagens com face no banco SUN/faces" "/home/daniel/Projects/faces-rec/dataset/sun/SUN2012"


####SELECT
# python src/datasetJobs.py select /home/daniel/Projects/faces-rec/dataset/training_images/faces 2600

####CROP
# python src/datasetJobs.py crop /home/daniel/Projects/faces-rec/dataset/training_images/test_non_faces 100