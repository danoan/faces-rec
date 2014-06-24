#[scene_source] [face_source] 
#[face_train_number] [crop_train_number] [crop_valid_number] 
#[max_stages] [salvar_em] 

#64x64

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces 300 400 20 "/home/daniel/Projects/faces-rec/dataset/64x64/Instancia Sun_300_400" 64

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces 600 800 20 "/home/daniel/Projects/faces-rec/dataset/64x64/Instancia Sun_600_800" 64

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces 1200 1600 20 "/home/daniel/Projects/faces-rec/dataset/64x64/Instancia Sun_1200_1600" 64

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces 2400 3200 20 "/home/daniel/Projects/faces-rec/dataset/64x64/Instancia Sun_2400_3200" 64

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/64x64/lfw_faces 8000 6000 20 "/home/daniel/Projects/faces-rec/dataset/64x64/Instancia Sun_8000_6000" 64



#24x24

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces 300 400 20 "/home/daniel/Projects/faces-rec/dataset/24x24/Instancia Sun_300_400" 24

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces 600 800 20 "/home/daniel/Projects/faces-rec/dataset/24x24/Instancia Sun_600_800" 24

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces 1200 1600 20 "/home/daniel/Projects/faces-rec/dataset/24x24/Instancia Sun_1200_1600" 24

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces 2400 3200 20 "/home/daniel/Projects/faces-rec/dataset/24x24/Instancia Sun_2400_3200" 24

python src_python/datasetJobs.py createTestSet /home/daniel/Projects/faces-rec/dataset/sun/SUN2012 /home/daniel/Projects/faces-rec/dataset/24x24/lfw_faces 8000 6000 20 "/home/daniel/Projects/faces-rec/dataset/24x24/Instancia Sun_8000_6000" 24
