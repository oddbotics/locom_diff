export ROS_MASTER_URI=http://10.0.2.1:11311
export ROS_HOSTNAME=udoo14
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

while[1]; do
address=$(ip addr show eth0 | grep -o '10.0.[[:digit:]].[[:digit:]]' | head -1 |
awk '{
	split($1, a, ".")
	oldsubnet = subnet
	subnet = a[3]
	
}')
if [oldsubnet != subnet]; then
#kill old nodes
if [ "$(ps aux | grep ros | grep -v grep | awk '{print $2}')" ]; then # or $(rosnode list)
    for i in $(ps aux | grep ros | grep -v grep | awk '{print $2}')
    do
        kill -2 $i;
    done
fi

#launch new ones
roslaunch oddbot_boot module.launch node_name:=$address;
fi

sleep 10;
done
