all: simulator

simulator: Clique.cc Clique.h Event.cc Event.h Link.cc Link.h Matrix.cc Matrix.h MeshRouter.cc MeshRouter.h Simulation.cc Simulation.h
	g++ Clique.cc Event.cc Link.cc Matrix.cc MeshRouter.cc Simulation.cc -o wmn-simulator -std=gnu++11

clean:
	rm -rf wmn-simulator