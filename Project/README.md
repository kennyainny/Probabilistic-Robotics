Install OpenCV
http://www.learnopencv.com/install-opencv3-on-ubuntu/

Install Eigen
http://eigen.tuxfamily.org/index.php?title=Main_Page

sudo cp -R (directory)/Eigen /usr/local/include

sudo cp -R (directory)/unsupported /usr/local/include

Install TurtleBot
http://emanual.robotis.com/docs/en/platform/turtlebot3/pc_setup/

Execute program
../bin/project -j4 

cd ~/catkin_ws/src/<project_folder_name>/

git init

git remote add origin -f https://github.com/numkang/Probabilistic-Robotics

git config core.sparsecheckout true

echo "Project/ROS/" >> .git/info/sparse-checkout

git pull --depth=2 origin master

put your code your in ~/catkin_ws/src/<project_folder_name>/Project/ROS/

cd ~/catkin_ws/src/Probabilistic_Robotics/Project/ROS/turtlebot3_simulations/turtlebot3_gazebo/launch

roslaunch turtlebot3_multi.launch

./goToGoal.py

./filtering_laserscan

./move_target

./particle_filter

