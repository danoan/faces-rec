#coding:utf-8

import time

def helloworld():
	start = time.time()
	for i in range(0,500000):
		c = (i*3.0)/2.0
	stop = time.time()

	print stop-start
