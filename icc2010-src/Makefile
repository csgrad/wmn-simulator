all: simulator

simulator: Clique.cc Clique.h Event.cc Event.h Link.cc Link.h MeshRouter.cc MeshRouter.h MeshClient.cc MeshClient.h Simulation.cc Simulation.h TopologyHelper.cc TopologyHelper.h Subset.cc Subset.h
	g++ Clique.cc Event.cc Link.cc MeshRouter.cc MeshClient.cc Simulation.cc Subset.cc TopologyHelper.cc -o wmn-simulator -std=gnu++11

clean:
	rm -rf wmn-simulator