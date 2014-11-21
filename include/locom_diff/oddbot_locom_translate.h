/*
 * \oddbot_locom_translate.h
 *
 * \author Chris Dunkers, CMU - cmdunkers@cmu.edu
 * \date October 13, 2014
 */

#ifndef ODDBOT_LOCOM_TRANSLATE_H_
#define ODDBOT_LOCOM_TRANSLATE_H_

#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "oddbot_lib/Serial.h"

class oddbot_locom_translate{
	public:
		oddbot_locom_translate();
		
	private:		
		ros::Publisher ve_pub;
		ros::Subscriber vel_slub;
		void get_vel(const geometry_msgs::Twist::ConstPtr& vel_msg);
		
};

int main(int argc, char** argv);

#endif
