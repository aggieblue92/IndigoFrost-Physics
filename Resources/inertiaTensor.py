# VERSION 2 - Takes in an Ogre.mesh.xml file.
#  Version 2 assumes all masses to be at 1.0 units - uniform point
#  mass. You should change this... Yeah you should.

# Calculate the inertia tensor of an object given its points
#  Data points input: (mass, x, y, z)

import sys
import time
import xml.etree.ElementTree as ET

from numpy import matrix
from numpy import linalg

# Parse the OGRE file model...
def Parse(inputFileName):
	tree = ET.parse(inputFileName)
	root = tree.getroot()

	# Okay, we have vertex and face information now.
	vertexBuffer = StripVertexData(root)

	return vertexBuffer

def StripVertexData(tree):
	# Get to vertex buffer element.
	vertexData = []
	for vertex in tree.iter('vertex'):
		for pos in vertex.iter('position'):
			vertexData.append([1., float(pos.get("x")), float(pos.get("y")), float(pos.get("z"))])
	return vertexData

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

if len(sys.argv) < 3:
	print """Usage:
python inertiaTensor.py inFile objTotalMass

inFile: File containing point data to convert into an inertia tensor.
Output is sent to the same directory, under the InverseTensorOut.txt file

Expected input file format:
mass, xpos, ypos, zpos
mass2, xpos2, ypos2, zpos2
...
for however many points.

objTotalMass: The total mass of the object. This mass is in kilograms
(or other relative measurement), and is distributed evenly among vertices."""
else:
	
	data_list = Parse(sys.argv[1])
	for ele in data_list:
		ele[0] *= float(sys.argv[2])
		ele[0] /= len(data_list)

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
	fout.write(str(I.item(0)) + ", " + str(I.item(1)) + ", " + str(I.item(2)) + "\n")
	fout.write(str(I.item(3)) + ", " + str(I.item(4)) + ", " + str(I.item(5)) + "\n")
	fout.write(str(I.item(6)) + ", " + str(I.item(7)) + ", " + str(I.item(8)) + "\n\n\n")
	I = I.I
	fout.write("Inverse Inertia Tensor:\n\n")
	fout.write(str(I.item(0)) + ", " + str(I.item(1)) + ", " + str(I.item(2)) + "\n")
	fout.write(str(I.item(3)) + ", " + str(I.item(4)) + ", " + str(I.item(5)) + "\n")
	fout.write(str(I.item(6)) + ", " + str(I.item(7)) + ", " + str(I.item(8)) + "\n")

	fout.close()

wait = raw_input("Press any key to continue...")