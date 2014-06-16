faces-rec
=========

datasetJobs.py
=============
    Seleção de scripts para automação de tarefas comuns na criação de conjuntos de dados
    para testes e treinamento.

    -crop [folder_scenes] [n]
        Corta n subjanelas (sem repetí-las) utilizando como base todas as imagens presentes em 
        folder_scenes
    
        folder_scenes: Caminho que contém as imagens a serem recortadas
        n: Número de recortes a ser feito

    -select [d] [n]
        Seleciona uma amostra aleatoria de arquivos um diretorio
    
        d: Diretório
        n: Número de imagens a selecionar

    -match [normal] [match]
        Retorna True se as imagens casam (no contexto dessa função, isso implica as imagens
        são semelhantes ou iguais)

        normal: Caminho da imagem que se quer encontrar um match
        match: Caminho da imagem candidata a matching

    -createTestSet [scene_source] [face_source] [face_train_number] [scene_train_number] [face_valid_number] 
                   [scene_valid_number] [crop_train_number] [crop_valid_number] [max_stages] [salvar_em] {face_test} {scene_test}

        Cria um conjunto de dados para teste e validação usando uma mesma fonte de dados. A estrutura resultante
        possui a seguinte estrutura. Caso a base de cenários seja a da SUN, como pré-processamento são retiradas
        todas as imagens que contém faces, baseando-se no algoritmo de match e usando as imagens identificadas 
        anteriormente em uma outra situação e que estão localizadas em: dataset/sun/com_faces
        
            Instancia/

                training_images/
                    faces/
                        {images}
                    non_faces/
                        group_0/
                            crop/
                                [random_number]/
                                    {cropped_images}
                            {full_images}
                        .
                        .
                        .
                        group_37

                validation_images/
                    faces/
                        {images}
                    non_faces/
                        group_0/
                            crop/
                                [random_number]/
                                    {cropped_images}
                            {full_images}

        scene_source: Caminho da fonte de dados de cenários (SUN DATABASE)
        face_source: Caminho da fonte de dados de faces (Labeled Faces in the Wild LFW)
        face_train_number: Número de elementos do conjunto de treino para faces
        scene_train_number: Número de elementos do conjunto de treino para cenários
        face_valid_number: Número de elementos do conjunto de validação para faces
        scene_valid_number: Número de elementos do conjunto de validação para cenários
        crop_train_number: Número de recortes utilizando as imagens de treinamento de cenários
        crop_valid_number: Número de recortes utilizando as imagens de validação de cenários
        salvar_em: Local em que o conjunto de dados gerado será salvo


