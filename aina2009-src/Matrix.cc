#ifndef MATRIX_CC
#define MATRIX_CC

#include "Matrix.h"

/* 
 * Construct a Compatibility Matrix
 */
Matrix::Matrix(std::vector <Link> links)
{
  /* Allocate Memory for Boolean Matrix */
  int num_links = (int)links.size();
  CM = new bool*[num_links];
  for(int i=0;i<num_links;i++)
    *(CM+i) = new bool[num_links];
  
  /* Set all entries incompatible by default */
  for(int x=0;x<num_links;x++)
    for(int y=0;y<num_links;y++)
      CM[x][y] = false;
      
  LS = links;
}

/*
 * Returns a vector of cliques which has scheduling performed on it
 * based on the Link data provided
 */
std::vector <Clique> Matrix::generateScheduling()
{
  std::vector <Clique> scheduling;
  
  generateCompatibilities();
  generateAllCliques();
  
  /*
  for(unsigned int x=0;x<CL.size();x++)
  {
    CL[x].display();
    std::cout << "  GAIN: " << CL[x].getGain(LS) << " WEIGHT: " << CL[x].getWeight(LS) << std::endl;
  }
  */
    
  while(!CL.empty())
  {
    int maxGain=-1; int maxLink=-1; int gain=0;
    for(unsigned int c=0;c<CL.size();c++)
    {
      gain = CL[c].getGain(LS);
      if(gain > maxGain)
      {
        maxGain = gain;
        maxLink = c;
      }
    }
    
    /* Select the Clique with the maximal gain and add it to the scheduling */
    if(maxGain >0 || CL[maxLink].getWeight(LS) > 0)
    {
      scheduling.push_back(CL[maxLink]);
    }
    
    Clique maxClique = CL[maxLink];
    
    /* Erase the Clique with the maximal gain from the group of all cliques */
    for(unsigned int c=0;c<CL.size();c++)
    {
      if(maxClique.intersect(CL[c]))
      {
        CL.erase(CL.begin()+c);
        c--; //step the counter back one since the size of the vector decreased by 1
      }
    }    
  }
  return scheduling;
}

/*
 * Displays the compatibility matrix
 * (needs to be fixed for easy reading)
 */
void Matrix::displayCM()
{
  std::cout << "   ";
  for(unsigned int x=0; x<LS.size(); x++)
  {
    std::cout << x;
    if(x>=10)
      std::cout << " ";
    else
      std::cout << "  ";
  }
  std::cout << std::endl;
  
  for(unsigned int x=0; x<LS.size(); x++)
  {
    std::cout << x;
    if(x>=10)
      std::cout << " ";
    else
      std::cout << "  ";
    for(unsigned int y=0;y<LS.size();y++)
    {
      std::cout << CM[x][y];
      if(x>=10)
        std::cout << " ";
      else
        std::cout << "  ";
    }
    std::cout << std::endl;
  }
}

/* ---- Private Functions ---- */

/*
 * Generates all of the cliques based on the compatibility matrix
 */
void Matrix::generateAllCliques()
{  
  for(unsigned int l1=0; l1<LS.size();l1++)
  {
    /* Generate the cliques of largest size */
    Clique c;
    c.addLink(LS[l1].getID());
    
    for(unsigned int l2=0;l2<LS.size();l2++)
    {
      if(LS[l1].getID() != LS[l2].getID())
      {
        if(isCompatibleWithExisting(LS[l2].getID(), c))
          c.addLink(LS[l2].getID());
      }
    }
    
    /* Generate all the subcliques from the largest clique */
    if(isUnique(c))
    {
      CL.push_back(c);
      generateSubCliques(c);
    }
    
    /* Generate the cliques of size 1 */
    Clique singleLink;
    singleLink.addLink(l1);
    if(isUnique(singleLink))
      CL.push_back(singleLink);
  }
}

/*
 * Generates a vector of all the subCliques from a larger clique
 */
void Matrix::generateSubCliques(Clique largeClique)
{
  for(unsigned int skipper=0;skipper<largeClique.size();skipper++)
  {
    Clique smallerClique;
    for(unsigned int x=0; x<largeClique.size();x++)
    {
      for(unsigned int y=0; y<largeClique.size();y++)
      {
        if(x!=y && x!=skipper && y!=skipper)
        {
          if(smallerClique.size() == 0)
            smallerClique.addLink(largeClique.getLink(x));
          smallerClique.addLink(largeClique.getLink(y));
        }
      }
    }
    
    /* 
     * Push on the Largest Clique we just made if unique and
     * generate even smaller cliques
     */
    if(smallerClique.size() > 0)
      if(isUnique(smallerClique))
      {
        CL.push_back(smallerClique);
        generateSubCliques(smallerClique);
      }
  }
}

/*
 * Checks if each link interferes with each other and sets it
 * compatible or incompatible accordingly
 */
void Matrix::generateCompatibilities()
{
  for(unsigned int x=0; x<LS.size(); x++)
  {
    for(unsigned int y=0; y<LS.size(); y++)
    {
      if(!LS[x].interfere(LS[y]))
        setCompatible(x,y);
    }
  }
}

/*
 * Makes sure that a potential link is compatible with the
 * links which already exist in the clique
 */
bool Matrix::isCompatibleWithExisting(int x, Clique existing)
{
  for(unsigned int c=0;c<existing.size();c++)
    if(!isCompatible(LS[x].getID(), existing.getLink(c)))
      return false;
  return true;
}

/*
 * Returns true if a potentialClique does not exist in CL yet
 */
bool Matrix::isUnique(Clique potentialClique)
{
  for(unsigned int c=0;c<CL.size();c++)
  {
    if(CL[c].equivalentClique(potentialClique))
      return false;
  }
  return true;
}

void Matrix::setCompatible(int x, int y)
{
  CM[x][y] = true;
}

bool Matrix::isCompatible(int x, int y)
{
  return CM[x][y];
}
#endif
