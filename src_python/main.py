#coding:utf-8

from pca import experiments

def run_pca_experiments():
	experiments.Experiment1().setup().run()
	experiments.Experiment2().setup().run()
	experiments.Experiment3().setup().run()


if __name__=='__main__':
	run_pca_experiments()

