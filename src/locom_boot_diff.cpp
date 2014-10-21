#include "locom_diff/locom_boot_diff.h"


locom_boot_diff::locom_boot_diff(){
	//subscribe to oddbot_boot
	//publish to oddbot_boot_stop
  ros::NodeHandle nodeHandle;
  boot_pub = nodeHandle.advertise<oddbot_msgs::OddbotBoot>("boot", 1000);
  stop_sub = nodeHandle.subscribe("boot_stop", 1000, &locom_boot_diff::get_info, this);
  subnet = get_subnet();
}
// Callback message
void locom_boot_diff::get_info(const oddbot_msgs::OddbotBootStop::ConstPtr& stop_msg){
	
	//check my IP address	
	if (stop_msg->subnet == subnet) {
    //set stop running flag
    subnet_msg_stop = true;
  }
}

void locom_boot_diff::send_info(){
  // Set the subnet
  module_info_msg.subnet = subnet;
	// Publish the subnet
  boot_pub.publish(module_info_msg);
}

bool locom_boot_diff::get_subnet_msg_stop() {
  return subnet_msg_stop;
}


int main(int argc, char** argv){
	
	//stop when stop running flag is true
	ros::init(argc, argv, "locom_boot_diff");

  locom_boot_diff module = locom_boot_diff();
  
  ROS_INFO("locomotion boot diff node started!");  

  ros::Rate loop_rate(10);

  while (ros::ok() && !module.get_subnet_msg_stop())
  {
    module.send_info();
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  return 0;
}
