#ifndef MAZE_H
#define MAZE_H

// enum creates a new type which can be used like any type
enum State {
  LINE_FOLLOWER,
  JUNCTION,
  TURN_LEFT,
  U_TURN,
  FINISHED
};

class MazeSolver {
  private:
  State state; // value of type state

  void followLine();

  // !!new code!! detecting junctions
  void checkIfJunction(){ 
      //half T (left) junction
      if(lineSensorValues[0] > 950 & lineSensorValues[2]>950){
        state = JUNCTION; 
      }
      //half T (right) junction
      if(linSensorValues[2] > 950 & lineSensorValues[3]/*why sensor 3 not 4?*/ > 950{
        state = JUNCTION;
      }
      // cross junction
      if(lineSensorValues[0] > 950 & lineSensorValues[2] > 950 & lineSensorValues[4] > 950){
        state = JUNCTION;
      }
      // T junction
      if(linSensorValues[0] > 950 & lineSensorValues[4] > 950){
        state = JUNCTION;
      }
  }
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    void loop();

};

#endif
