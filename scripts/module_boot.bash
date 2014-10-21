export ROS_MASTER_URI=http://10.0.2.1:11311
source ~/catkin_ws/devel/setup.bash

# Remember to change 'lo' to 'eth0'
address=$(ip addr show eth0 | grep -o '10.0.[[:digit:]].[[:digit:]]' | head -1 |
awk '{
	split($1, a, ".")
	subnet = a[3]
	print "module_" subnet
}')
# echo $address
roslaunch oddbot_boot module.launch node_name:=$address