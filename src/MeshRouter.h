#ifndef MESHROUTER_H
#define MESHROUTER_H

#include <iostream>       //std::cout/cin, srand48, drand48
#include <cmath>          //sqrt
#include <vector>         //std::vector

class MeshRouter
{
  public:
  /* Constructors */
  MeshRouter(){id=-1;x=-1;y=-1;gateway=false; permission=false; busy=false; buffer=-1; retries=0;};
  MeshRouter(int newID, double newX, double newY){id=newID;x=newX;y=newY;gateway=false; permission=false; busy=false; buffer=-1; retries=0;};
  
  /* Accessors */
  double getX(){return x;};
  double getY(){return y;};
  int getID(){return id;};
  int getBuffer(){return buffer;};
  bool isGateway(){return gateway;};
  bool isBusy(){return busy;};
  bool hasPermission(){return permission;};
  std::vector<int> getNeighbours(){return neighbours;};
  int getRetries(){return retries;};
  
  /* Mutators */
  void setGateway(){gateway=true;};
  void addNeighbour(int neighbour_id);
  void setPermission(bool p){permission=p;};
  void setBusy(bool b){busy=b;};
  void setBuffer(int b){buffer=b;};  
  void setRetries(int r){retries = r;};
  
  /* Functions */
  double distance(MeshRouter MR);
  
  /* Display */
  void display();
  
  private:
  
  void displayNeighbours();
  
  std::vector<int> neighbours;
  int id, buffer, retries;
  double x,y;
  bool gateway, permission, busy;
};

#include "MeshRouter.cc"

#endif
