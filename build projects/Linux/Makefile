libindigofrost.a:
	g++ -c -std=c++11 -I../../include ../../src/*.cpp
	ar rvs libindigofrost.a *.o
	if [ ! -d "./obj" ]; then mkdir obj; fi
	mv *.o obj
	if [ ! -d "../../lib" ]; then mkdir ../../lib; fi
	if [ ! -d "../../lib/Linux" ]; then mkdir ../../lib/Linux; fi
	mv libindigofrost.a ../../lib/Linux