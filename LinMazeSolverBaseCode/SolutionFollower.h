#ifndef SOLUTION_H
#define SOLUTION_H

#include "MazeSolver.h"

class SolutionFollower {
  private:
  State state;

  void showPath();

  void followLine();

  void checkIfJunction();

  void identifyJunction();

  void turnLeft();

  void turnRight();

  public:

    Decisions path[64];
    int totalLength;


  void loop();

};

#endif