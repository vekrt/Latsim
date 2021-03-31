CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pedantic -fopenmp -fPIC -O3 #-g -v -da -Q
LIBS = -lX11 -lpthread -lXrandr -lXi -ldl `pkg-config --libs glfw3 gl`
OBJECTS = square_lattice.o triangular_lattice.o honeycomb_lattice.o integrator_mh.o ising.o integrator_wolff.o config_file.h potts.o
OBJECTS_PT = main_pt.cpp square_lattice.o triangular_lattice.o honeycomb_lattice.o integrator_mh.o ising.o integrator_wolff.o


.PHONY: all clean

all: main square_lattice.o integrator_mh.o integrator_wolff.o ising.o config_file.h config_file.tpp

main_opengl: main_opengl.cpp glad.c renderer.o camera.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main: main.cpp glad.c renderer.o camera.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

main_pt: $(OBJECTS_PT)
	$(CXX) $(CXXFLAGS) `python3.7-config --cflags` -o $@ $^ `python3.7-config --libs`

square_lattice.o: square_lattice.cpp square_lattice.h
#	$(CXX) $(CXXFLAGS) -c -o $@ $<

triangular_lattice.o: triangular_lattice.cpp triangular_lattice.h

honeycomb_lattice.o: honeycomb_lattice.cpp honeycomb_lattice.h

integrator_mh.o: integrator_mh.cpp integrator_mh.h

integrator_wolff.o: integrator_wolff.cpp integrator_wolff.h

glad.o: glad.c glad.h khrplatform.h
	gcc -c -o $@ $< -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor

renderer.o: renderer.cpp renderer.h

camera.o: camera.cpp renderer.h

ising.o: ising.cpp ising.h

potts.o: potts.cpp potts.h

config_file.o: config_file.tpp config_file.h

clean:
	rm -f main main_pt main_opengl *.o
