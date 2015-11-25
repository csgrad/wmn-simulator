#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>       //std::cout/cin, srand48, drand48
#include <vector>         //std::vector

#include "Link.h"
#include "Clique.h"

class Matrix
{
  public:
  Matrix(std::vector <Link> links);
  std::vector <Clique> generateScheduling();
  
  /* Display Functions */
  void displayCM();
  
  private:
  void setCompatible(int x, int y);
  void generateAllCliques();
  void generateCompatibilities();
  void generateSubCliques(Clique largeClique);
  bool isUnique(Clique potentialClique);
  bool isCompatible(int x, int y);
  bool isCompatibleWithExisting(int x, Clique existing);
  bool** CM;                      //compat matrix
  std::vector <Clique> CL;        //all of the cliques
  std::vector <Link> LS;          //all the links
};

#endif
