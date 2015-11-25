# wmn-simulator
A custom c++ simulator developed to simulate wireless mesh networks. 
Results from the simulator published in [AINA 2009](aina2009-src) paper "[Fair Scheduling 
with Multiple Gateways in Wireless Mesh Networks](http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=5076187)".

The simulator was further extended so that information from other "layers" in the network stack are usable for making decisions such as resource assignment. This was the basis of the [ICC 2010](icc2010-src) paper "[Cross-Layer Mixed Bias Scheduling for Wireless Mesh Networks](http://ieeexplore.ieee.org/xpl/freeabs_all.jsp?arnumber=5502372)"

All of the simulation parameters are set in the Simulation.h file including the number of repeats, the number of mesh routers, the interarrival rate of packets etc.
