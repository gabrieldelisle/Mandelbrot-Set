import matplotlib.pyplot as plt

with open("color.txt", 'r') as file: 
	f = lambda x: (int(x)+2)**0.1
	matrix = [list( map(f, u.split(" ")[:-1]) ) for u in file]
	
plt.matshow(matrix, cmap = plt.cm.magma)
plt.savefig("figure.pdf")
plt.show()