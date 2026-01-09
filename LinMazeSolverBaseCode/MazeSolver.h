#ifndef MAZE_H
#define MAZE_H

// enum creates a new type which can be used like any type
enum State {
  LINE_FOLLOWER,
  JUNCTION,
  TURN_LEFT,
  TURN_RIGHT,
  U_TURN,
  FINISHED,
  FAKE_END
};

// Decisions needing to be remembered
enum Decisions {
  NONE,
  RIGHT,
  LEFT,
  BACK,
  FORWARD
};

class MazeSolver {
  private:
  State state; // value of type state

  Decisions path[64]; //creates vector for remembering path
  int count = 0; 

  void simplifyPath();

  void showPath();

  char convertToCharacter(Decisions name);

  void followLine();

  void checkIfJunction();

  void checkIfDeadEnd();

  void identifyJunction();

  void turnLeft();

  void turnRight();

  void uTurn();
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    void loop();

};

#endif

