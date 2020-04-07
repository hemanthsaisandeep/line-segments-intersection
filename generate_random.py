
'''
	Generates 50 line segments as a set of 2 end coordinates 
	in the range [0,100], [0,100]

	Dumping data in json format for easy extraction for later 
	stages
'''

import numpy as np 
import pandas as pd
import matplotlib.pyplot as plt
import random



def generator(coordinate_set):
	counter=0
	while counter <= 99:
		x1_coord = random.uniform(0,100)
		y1_coord = random.uniform(0,100)
		x2_coord = random.uniform(0,100)
		y2_coord = random.uniform(0,100)
		coordinate_set.append({'x1':x1_coord,'y1':y1_coord,
														'x2':x2_coord,'y2':y2_coord,
													})
		counter+=1

	return coordinate_set



if __name__ == "__main__":
	coordinate_set = []
	coordinate_set = generator(coordinate_set)
	# print(coordinate_set)
	df = pd.DataFrame(coordinate_set)
	print(df)
	for _,item in df.iterrows():
		x_values = [item['x1'],item['x2']]
		y_values = [item['y1'],item['y2']]
		plt.plot(x_values,y_values)

	plt.show()



