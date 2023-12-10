
# raylib:
 ```
 mkdir build_raylib
 cd build_raylib
 cmake ..
 make
 ```
 in case of `-bash: ./build.sh: \bin\sh^M: bad interpreter: No such file or directory` error run this :
```
sed -i -e 's/\r$//' build.sh
```
--------
## Multitasking Scheduling Algorithm Simulation System (MSASS)
The multitasking scheduling algorithm simulation system is designed to implement various scheduling algorithms such as FIFO, RR, SRT, etc, offering a platform to simulation their execution and visualize the results on the interface. It is developed using C, makefile and Raylib and Raygui libraries under Linux. 

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

------
## Makefile commands 

1. To integrate Raylib and Raylib libraries into the project, you only need to run the following command:
```
make build_raylib
```
 A build_raylib directory will be generated.


2. To compile the project use the following command: 
```
make build
```
 This command will compile the source code and store the resultant output in a generated build directory. 

3. To run the project, use the following command:
```
make run
```
 This commands will open the application interface.

4. If you've added a new scheduliing algorithm or maade some changes to existing one, use the following command: 
```
make update
```
 This command updates the build to take into consideration new additions and automatically add the new algorithm to the menu.


5. To remove all generated executable files and reset the build environment, use the following command:
```
make remove
```
------
## How to use

MSASS works based on a configuration file in which we define attributes like the number of processes, arrival time bounds, runtime bounds, and priority classes. 
So, you can upadte the attributes values according to your preferences.

The system generates a processes file in the following format : one process per line in this order **processName;arrivalTime;runTime;priority**

-------
## How to contribute
Contributions to improve the multitasking scheduling algorithm simulation system are welcome and greatly appreciated! Here's how you can contribute:

1. Fork the Repository into your github account
2. Clone the forked repository to your local machine.
3. Create a new branch for your changes.
4. Implement your additions or fixes within the codebase.
5. Add and commit your changes with descriptive commit messages.
6. Push your changes to your forked repository.
7. Create and submit Pull Request.
8. Participate in discussions, reviews, and feedback regarding your pull request.






