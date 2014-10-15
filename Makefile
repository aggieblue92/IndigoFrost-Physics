libindigofrost.a:
	g++ -c -std=c++11 -I../../include ../../src/*.cpp
	ar rvs libindigofrost.a *.o
	rm *.o
	mv libindigofrost.a ../../lib/$(SYSTEM)