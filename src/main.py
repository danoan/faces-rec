#coding:utf-8

from pca import experiments
from treinador import detector_experiments

def run_pca_experiments():
	experiments.Experiment1().setup().run()
	experiments.Experiment2().setup().run()
	experiments.Experiment3().setup().run()

def run_detector_experiments():
	detector_experiments.Experiment1().setup().run()
	detector_experiments.Experiment2().setup().run()


if __name__=='__main__':
	run_pca_experiments()

