# Calculate the inertia tensor of an object given its points
#  Data points input: (mass, x, y, z)

import sys
import time

from numpy import matrix
from numpy import linalg

def scalar_product(v1, v2):
	toReturn = 0
	for i in range(0, len(v1)):
		toReturn += v1[i] * v2[i]
	return toReturn

def moment_x(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * (element[2]**2 + element[3]**2)
	return total

def moment_y(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * (element[1]**2 + element[3]**2)
	return total

def moment_z(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * (element[1]**2 + element[2]**2)
	return total

def moment_xy(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * element[1] * element[2]
	return total

def moment_xz(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * element[1] * element[3]
	return total

def moment_yz(data_set):
	total = 0.0
	for element in data_set:
		total += element[0] * element[2] * element[3]
	return total

if len(sys.argv) < 2:
	print """Usage:
python inertiaTensor.py inFile

(or, more easily, in Windows, just drag a text file onto this script)

inFile: File containing point data to convert into an inertia tensor.
Output is sent to the same directory, under the InverseTensorOut.txt file

Expected input file format:
mass, xpos, ypos, zpos
mass2, xpos2, ypos2, zpos2
...
for however many points."""
else:
	fin = open(sys.argv[1], "r")
	print "File",sys.argv[1],"opened."

	data_list = [];

	for line in fin:
		elements = line.strip().split(",")
		toAdd = []
		for el in elements:
			toAdd.append(float(el))
		data_list.append(toAdd)

	print "Done!\n\n"

	print "Finding inertia tensor..."
	I = matrix([[moment_x(data_list), -moment_xy(data_list), -moment_xz(data_list)],[-moment_xy(data_list), moment_y(data_list), -moment_yz(data_list)],[-moment_xz(data_list), -moment_yz(data_list), moment_z(data_list)]])

	print "Found the tensor!\n",I

	t = sys.argv[1].split("\\")
	outFileLoc = ""
	for e in range(1, len(t)):
		outFileLoc += t[e-1] + "\\"
	outFileLoc += "InverseTensorOut.txt"

	print "Outputting tensor and inverse tensor to:",outFileLoc

	fout = open(outFileLoc, 'w')

	fout.write("Inertia Tensor:\n\n")
	fout.write(str(I[0]) + "\n" + str(I[1]) + "\n" + str(I[2]) + "\n\n")
	I = I.I
	fout.write("Inverse Inertia Tensor:\n\n")
	fout.write(str(I[0]) + "\n" + str(I[1]) + "\n" + str(I[2]))

	fout.close()

wait = raw_input("Press any key to continue...")