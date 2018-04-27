# Install Library

## Install OpenCV

http://www.learnopencv.com/install-opencv3-on-ubuntu/

## Install Eigen

http://eigen.tuxfamily.org/index.php?title=Main_Page

```console
sudo cp -R (directory)/Eigen /usr/local/include
sudo cp -R (directory)/unsupported /usr/local/include
```

## Install TurtleBot

http://emanual.robotis.com/docs/en/platform/turtlebot3/pc_setup/

## Install ROS

http://wiki.ros.org/ROS/Installation

# Git Pull only ROS project

```console
cd
cd ~/catkin_ws/src/<project_folder_name>/
git init
git remote add origin -f https://github.com/numkang/Probabilistic-Robotics
git config core.sparsecheckout true
echo "Project/ROS/" >> .git/info/sparse-checkout
git pull --depth=2 origin master
```console

put your code folder in ~/catkin_ws/src/<project_folder_name>/Project/ROS/

Ensure that your CMake file works

# Build

new terminal tab

```console
cd
cd ~/catkin_ws
catkin_make
```

# Execute program with ROS

new terminal tab

```console
cd
cd ~/catkin_ws/src/Probabilistic_Robotics/Project/ROS/turtlebot3_simulations/turtlebot3_gazebo/launch
roslaunch turtlebot3_multi.launch
```

new terminal tab

```console
cd
cd ~/catkin_ws/src/Probabilistic_Robotics/Project/ROS/move_robot_src
./goToGoal.py
```

new terminal tab

```console
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project/ROS/filter_laser_t
./filtering_laserscan
```

new terminal tab

```console
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project/ROS/move_target
./move_target
```

new terminal tab

```console
cd
cd ~/catkin_ws/build/Probabilistic_Robotics/Project/ROS/particle_filter
./particle_filter
```

