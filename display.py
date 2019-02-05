import matplotlib.pyplot as plt
import numpy as np


with open("color"+str(i)+".txt", 'r') as file: 
	f = lambda x: int(x)**0.7 #more contrast for lower values
	matrix = np.array([list( map(f, u.split(" ")[:-1]) ) for u in file])
	
plt.imsave("figure"+str(i)+".png", matrix, cmap = plt.cm.magma)