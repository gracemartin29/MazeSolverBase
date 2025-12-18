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
  checkIfJunction(); //new code!!
  
  public:

    // constructor
    MazeSolver();

    // function to be called at every main loop
    //void loop(); //original!!
    // new replaced code!! (unsure if correct)
    void loop(){
      if(state == JUNCTION){
        motors.setSpeeds(0, 0);
      }
      identifyJunction();
    }

};

//////////////////
// !!new code!! //
//////////////////
  //detecting junctions
  void MazeSolver::checkIfJunction(){ 
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
  
  //identifying junctions
  void MazeSolver::identifyJunction(){
    motors.setSpeeds(baseSpeed, baseSpeed);
    delay(50);
    lineSensors.readLineBlack(lineSensorValues);
    motors.setSpeeds(0, 0);
    
    // Finish line
    if(lineSensorValues[0] > 950 & lineSensorValues[1] > 950 & lineSensorValues[2] > 950 & lineSensorValues[3] > 950 & lineSensorValues[4] > 950){
      state = FINISHED;
    }
    //half T (left) junction
    else if(lineSensorValues[0] > 950 & lineSensorValues[2]>950){
      state = TURN_LEFT; 
    }
    //half T (right) junction
    else if(linSensorValues[2] > 950 & lineSensorValues[3]/*why sensor 3 not 4?*/ > 950{
      state = FOLLOW_LINE;
    }
    // cross junction
    else if(lineSensorValues[0] > 950 & lineSensorValues[2] > 950 & lineSensorValues[4] > 950){
      state = TURN_LEFT;
    }
    // T junction
    else if(linSensorValues[0] > 950 & lineSensorValues[4] > 950){
      state = TURN_LEFT;
    }
  }  

  // turn left
  void MazeSolver::turnLeft(){
    //step forward
    motors.setSpeeds(baseSpeed, baseSpeed);
    delay(50);
    //turn left 90 degrees (which value is which side of the motor?
    motors.setSpeeds(-baseSpeed, baseSpeed);
    // how long does it take to turn 90 degrees?
    delay(50);
    // change state back
    state = LINE_FOLLOWER;

  }

  // checking for dead ends
  void MazeSolver::checkIfDeadEnd(){
    // dead end
    if(lineSensorValues[0] = 0 & lineSensorValues[1] = 0 & lineSensorValues[2] = 0 & lineSensorValues[3] = 0 & lineSensorValues[4] = 0){
      return true;

      // stop and prepare to U turn
      motors.setSpeeds(0, 0);
      state = U_TURN;
      
    } else {
      return false;
    }
  }

  void MazeSolver::uTurn(){
    // turn 180 degrees
    motor.setSpeeds(-baseSpeed, baseSpeed);
    delay(100); // how long does it take to turn 180 degrees?
    state = LINE_FOLLOWER;
  }
#endif
