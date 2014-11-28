# Compiler to use...
CC=g++

# Flags to use in compilation...
CFLAGS=-c -Wall -std=c++11 -pthread
LDFLAGS=

INCLUDE_DIR=../../include
SOURCE_DIR=../../src

SOURCES=$(SOURCE_DIR)/AnchoredSpringForce.cpp $(SOURCE_DIR)/BasicContact.cpp $(SOURCE_DIR)/BasicPhysicsObject.cpp $(SOURCE_DIR)/BoundingSphere.cpp $(SOURCE_DIR)/BruteForceCollisionManager.cpp $(SOURCE_DIR)/BVHNode.cpp $(SOURCE_DIR)/BVHTree.cpp $(SOURCE_DIR)/Collidable.cpp $(SOURCE_DIR)/CollisionBox.cpp $(SOURCE_DIR)/CollisionSphere.cpp $(SOURCE_DIR)/CommonCollisionBox.cpp $(SOURCE_DIR)/CommonCollisionSphere.cpp $(SOURCE_DIR)/CommonContact.cpp $(SOURCE_DIR)/DebugLogger.cpp $(SOURCE_DIR)/ForceRegistry.cpp $(SOURCE_DIR)/GravityForce.cpp $(SOURCE_DIR)/ICollisionGeometry.cpp $(SOURCE_DIR)/IContact.cpp $(SOURCE_DIR)/IPhysicsNode.cpp $(SOURCE_DIR)/IPhysicsObject.cpp $(SOURCE_DIR)/Matrix.cpp $(SOURCE_DIR)/Movable.cpp $(SOURCE_DIR)/PlayerMotionForce.cpp $(SOURCE_DIR)/Quaternion.cpp $(SOURCE_DIR)/SpringForce.cpp $(SOURCE_DIR)/TestForce.cpp $(SOURCE_DIR)/Vect3.cpp $(SOURCE_DIR)/Vect3Normal.cpp $(SOURCE_DIR)/WorldManager.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libindigofrost.a

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^
	if [ ! -d "../../lib" ]; then mkdir ../../lib; fi
	if [ ! -d "../../lib/Linux" ]; then mkdir ../../lib/Linux; fi
	mv $(LIBRARY) ../../lib/Linux

.cpp.o:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@

clean:
	rm $(SOURCE_DIR)/*.o
