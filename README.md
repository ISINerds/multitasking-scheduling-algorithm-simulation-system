--------
## Multitasking Scheduling Algorithm Simulation System (MSASS)


The multitasking scheduling algorithm simulation system is designed to implement various scheduling algorithms such as FIFO, RR, SRT, etc, offering a platform to simulate their execution and visualize the results on the interface. It is developed using C, makefile and Raylib and Raygui libraries under Linux. 

## Key features

- Simulation of various  scheduling algorithms.
- Visualization and analysis of processes scheduling behavior.
- Customizable processes parameters for experimentation.
  
----
## Prerequisities

Before utilising this simulation system ensure the following: 

 - Access to Linux or WSL 
 - The gcc compiler is installed 
 - The Cmake is installed

-----
## Getting started
  1. Clone the repo
     
     ```
     git clone https://github.com/ISINerds/multitasking-scheduling-algorithm-simulation-system.git
     ```
     
  2. Integrate Raylib and Raygui
     
     ```
       make build_raylib
     ```
     
     In case of `-bash: ./build.sh: \bin\sh^M: bad interpreter: No such file or directory` error run this :
     
      ```
      sed -i -e 's/\r$//' build.sh
      ```
      
  3. Compile and build the project
     
     ```
     make build
     ```
     
  4. Run the project
     
     ```
     make run
     ```
     
------
## How to use

MSASS works based on a configuration file **config.conf** in which we define attributes like the number of processes, arrival time bounds, runtime bounds, and priority classes. 
So, you can upadte the attributes values according to your preferences.

The system generates a processes file  in the following format : one process per line in this order **processName;arrivalTime;runTime;priority** 

Upon running the project, the interface will be displayed. Use the interface controls to choose a scheduling algorithm from the menu, enter inputs and start the simulation. 

------
## Additional information 
### Makefile commands 
  1. The first one serves to integrate Raylib and Raylib libraries into the project, by generating a build_raylib directory.
     
   ```
   make build_raylib
   ```

2. This command will compile the source code and store the resultant output in a generated build directory.
       
    ```
    make build
    ```

3. This commands will  run the project opening the application's interface.
       
    ```
    make run
    ```
 
4. This command can be used if you've added a new scheduliing algorithm or maade some changes to existing one. It updates the build to take into consideration new additions and automatically add the new algorithm to the menu.
   
    ```
    make update
    ```

5. This command removes all generated executable files and resets the build environment.
   
    ```
    make remove
    ```
    
----------
## How to contribute
Contributions to improve the multitasking scheduling algorithm simulation system are welcome and greatly appreciated! Here's how you can contribute:

1. Fork the Repository.
2. Clone the forked repository.
3. Create a new branch for your changes (```git checkout -b feature/AmazingFeature```).
4. Commit your changes (```git commit -m 'Add some AmazingFeature'```) .
5. Push your changes to the branch (```git push origin feature/AmazingFeature```).
6. Create and submit Pull Request.
7. Participate in discussions, reviews, and feedback regarding your pull request.






