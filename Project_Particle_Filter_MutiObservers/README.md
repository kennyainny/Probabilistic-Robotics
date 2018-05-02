# Install Library

## Install OpenCV

http://www.learnopencv.com/install-opencv3-on-ubuntu/

## Install Eigen

http://eigen.tuxfamily.org/index.php?title=Main_Page

```
sudo cp -R (directory)/Eigen /usr/local/include
sudo cp -R (directory)/unsupported /usr/local/include
```

## Install TurtleBot

http://emanual.robotis.com/docs/en/platform/turtlebot3/pc_setup/

## Install ROS

http://wiki.ros.org/ROS/Installation

# Git Pull only ROS project

```
cd
cd ~/catkin_ws/src/<project_folder_name>/
git init
git remote add origin -f https://github.com/numkang/Probabilistic-Robotics
git config core.sparsecheckout true
echo "Project_Particle_Filter_MutiObservers/ROS/" >> .git/info/sparse-checkout
git pull --depth=2 origin master
```

put your code folder in ~/catkin_ws/src/<project_folder_name>/Project_Particle_Filter_MutiObservers/ROS/

Ensure that your CMake file works

# Build

new terminal tab

```
cd
cd ~/catkin_ws
catkin_make
```

# Execute program with ROS

open terminal

```
cd
roscore
```

new terminal tab

```
cd
cd ~/catkin_ws/src/Probabilistic_Robotics/Project_Particle_Filter_MutiObservers/ROS/turtlebot3_simulations/turtlebot3_gazebo/launch
roslaunch turtlebot3_multi.launch
```

new terminal tab

```
cd
cd ~/catkin_ws/src/Probabilistic_Robotics/Project_Particle_Filter_MutiObservers/ROS/move_robot/src
./goToGoal.py
```

new terminal tab

```
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project_Particle_Filter_MutiObservers/ROS/filter_laser_t
./filtering_laserscan
```

new terminal tab

```
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project_Particle_Filter_MutiObservers/ROS/move_target
./move_target
```

new terminal tab

```
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project_Particle_Filter_MutiObservers/ROS/particle_filter
./particle_filter
```

