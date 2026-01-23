# Maze Solving Robot
This is a Pololu 3PI project, where a pololu should be able to follow, solve, remember said solution and follow a maze again with no wrong turns.

This code uses the 'Hand on the Wall' searching algorithm to solve the maze. So it will keep going forward until it detects a junction and then always turn left turn left, until it finds its way out of the maze.

## Hardware
- Pololu 3PI
- USB Micro B Cable

## Software
- Arduino IDE
- Pololu A-Star Boards
- Pololu3piPlus32U4 Library

## Other Materials Needed
- White, Smooth Tabel/ Surface
- Black tape
- Bright lighting

# Getting started
## Software
### Installing the Boards and Library:  
**Boards**
1. In the Arduino IDE, open the File menu (Windows/Linux) or the Arduino menu (macOS) and select "Preferences".
2. In the Preferences dialog, find the "Additional Boards Manager URLs" text box. Copy and paste the following URL into this box:

https://files.pololu.com/arduino/package_pololu_index.json

If there are already other URLs in the box, you can either add this one separated by a comma or click the button next to the box to open an input dialog where you can add the URL on a new line.

3. Click the "OK" button to close the Preferences dialog.
4. In the Tools > Board menu, select "Boards Manager..." (at the top of the menu).
5. In the Boards Manager dialog, search for "Pololu A-Star Boards".
6. Select the "Pololu A-Star Boards" entry in the list, and click the "Install" button.

**Library**
1. In the Arduino IDE, open the sketch menu and select "Include Library" --> "Manage Libraires"
2. In the Library Manager search for 'Pololu3piPlus32U4'
3. Click the "Install" button

To run the software:
1. Clone git https://github.com/gracemartin29/MazeSolverBase.git
2. Open in Arduino IDE
3. Connect the Pololu to computer through its USB Micro B port at the back, and select on the IDE
4. Upload the code to the Pololu

## Setting up the Maze
The Pololu can turn left and right corners and deal with 4 different Junctions:
- Cross  
![cross junction](cross_junction.png)
- T  
![T junction](T_junction.png)
- Half T (Left)  
![half T left junction](half_T_L_junction.png)
- Half T (Right)   
![half T right junction](half_T_R_junction.png)

Use the tape on the table/ surface to build your maze, keeping your lines as clean and straight as possible.

To make the finish line layer multiple lines of tape to make a thick recktangle.

**Example:**  
![example of maze](example_of_maze.jpg)

*note - making sure your maze is lit well from above and there are no large shadows, as the pololu can mistake it as a line and start trying to follow it*
## Following the Maze!
1. Place the Pololu at the start of the maze, ensuring it's as straight on the line as you can get.
2. Press 'B' to calabrate the Pololu.
3. Once it has finished calabrating, presh 'B' again and it should start following the maze.

4. Once it's reached the finish line, and now knows the quickest path to get through the maze, place the pololu back at the start of your maze and press 'B' again.

It should now go through the maze without making any unnecessary detors.

## Usage
This code uses 2 main classes, MazeSolver and SolutionFollower. MazeSolver deals with exploring the whole maze and remembering the shortest path to the finish line, SolutionFollower deals with following that shortest path.

### MazeFollower Class
The Pololu has 7 different states it can be in, each with different protocols to follow:

`enum State {LINE_FOLLOWER,
  JUNCTION,
  TURN_LEFT,
  TURN_RIGHT,
  U_TURN,
  FINISHED,
  FAKE_END
};`

<ins>LINE_FOLLOWER:</ins>   
When the Pololu is in LINE_FOLLOWER state, it calls the followLine function. This function uses the Pololu sensor values to determine where the line it should be following is, how far away it is from said line and correct itself back onto the line. The function is called repeatedly until the Pololu encounters a junction or the finish line, at which point it enters a different state.  

`position = lineSensors.readLineBlack(lineSensorValues);`  
`error = position - 2000;`

Position stores the values from the Pololu sensors, which indicate where the line to be followed is. If position is less than 2000, the line is going towards the left, and if position is greater than 2000, it is going towards the right. Error changes these values to centre = 0, > 0 = right and < 0 = left, to make it easier in the rest of the code.

`int16_t speedDifference = error * (int32_t)proportioinal / 256 + (error - lastError) * (int32_t)derivative / 256;`

Speed difference calculates how much the Pololu needs to correct its course. If it is negative, the left motor (leftSpeed) is decreased, and the right motor (rightSpeed) is increased. The opposite happens if it is positive. After these speeds are calculated, they are sent to the Pololu.

`int16_t leftSpeed = (int16_t)baseSpeed + speedDifference;`
`int16_t rightSpeed = (int16_t)baseSpeed - speedDifference;`
`motors.setSpeed(leftSpeed, rightSpeed);`

<ins>JUNCTION:</ins>  
Junctions are split into 2 functions: checkIfJunction (identify that there is a junction) and identifyJunction (how does the Pololu go about the junction). CheckIfJunction reads sensors 0, 1, 3, and 4, skipping sensor 2 (the centre sensor) on the Pololu. If any of them is greater than 950, it means there is a line to the left and/or right, and therefore it has encountered a junction.  

`if (lineSensorValues[0] > 950) junction = true;  // detect a line to the left`  
  `if (lineSensorValues[1] > 950) junction = true;  // detect a line to the left`  
  `if (lineSensorValues[3] > 950) junction = true;  // detect a line to the right`  
  `if (lineSensorValues[4] > 950) junction = true;  // detect a line to the right`  

It uses junction as a boolean value, and when it is set to true, the Pololu's state is switched to JUNCTION, and the motor speeds are set to 0, pausing the Pololu’s movement.  

`if (junction) {`  
`state = JUNCTION;`   
`motors.setSpeeds(0, 0);}`

The next function, identifyJunction, works out what kind of junction the Polou is encountering (T, cross, half T, or the finish line), which determines how it will proceed. 

Since this code using the hand on the wall algorithm, if there is a left in the junction (cross, T, half T left), it will always take it by changing its state to TURN_LEFT.  

`if (lineSensorValues[0] > 750) {`
`state = TURN_LEFT;`   
`return;}`  

However, if there is no left in the junction (half T right), it will continue forward, keeping its state on LINE_FOLLOWER.  

`if (lineSensorValues[2] > 750) {`  
`motors.setSpeeds(baseSpeed, baseSpeed);`   
`delay(100);`    
`state = LINE_FOLLOWER;`   
`return;}`

<ins>FINISHED:</ins>
Still inside the identifyJunction function, if all sensors are high, the Pololu has found the finish line, and its state changes to FINISHED and the Pololu's motors stop.  

`if (lineSensorValues[0] > 950 && lbineSensorValues[1] > 950 && lineSensorValues[2] > 950 && lineSensorValues[3] > 950 && lineSensorValues[4] > 950) {`  
`state = FINISHED;`  
`return;}`

<ins>TURN_LEFT & TURN_RIGHT</ins>  
When the Pololu is in the TURN_LEFT state, it enters the turnLeft function, where it takes a step forward, stops, its left motor turns backwards while its right turns forward, stops again and then switches its state back to LINE_FOLLOWER:  

`motors.setSpeeds(baseSpeed, baseSpeed);`  
`delay(250);`  
`motors.setSpeeds(0, 0);`   
`motors.setSpeeds(-baseSpeed, baseSpeed);`  
`delay(760);`  
`motors.setSpeeds(0, 0);`  
`state = LINE_FOLLOWER;`

The same happens for TURN_RIGHT, but instead the left motor turns forward, and the right motor turns backwards. This is where FAKE_END can be really useful.

<ins>FAKE_END:</ins>  
FAKE_END stops the Pololu and can be used as a placeholder to debug/ calibrate the Pololu while testing. To ensure the Pololu is properly lined up after a turn, you can use FAKE_END in place of LINE_FOLLOWER to stop the Pololu and adjust how much it turns by changing the delays after turning (i.e., how long it turns for).

<ins>U_TURN:</ins>  
U_TURN is needed when the Pololu reaches a dead end. The checkIfDeadEnd function sets Pololu’s state to U_TURN when sensor 2 is less than 500, i.e there is nothing in front of it.

`if(lineSensorValues[2] < 500>){`  
`state = U_TURN;}`

Then the uTurn function is executed:

`motors.setSpeeds(-baseSpeed, baseSpeed);`
`delay(1620);`  
`motors.setSpeeds(0, 0);`  
`state = LINE_FOLLOWER;`

<ins>**Remebering the shortest path:**</ins>
