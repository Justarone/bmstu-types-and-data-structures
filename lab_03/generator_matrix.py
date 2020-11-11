from random import randint, shuffle

size = [int(i) for i in input("Введите размеры требуемой матрицы: ").split()]

matrix = list()
for i in range(size[0]):
	for j in range(size[1]):
		matrix.append(str(i) + ' ' + str(j) + ' ' + str(randint(1, 100)) + '\n')
shuffle(matrix)

filename = "matrix" + str(size[0]) + 'x' + str(size[1]) + '.txt'
f = open(filename, "w")
for i in range(len(matrix)):
	f.write(matrix[i])

f.close()
