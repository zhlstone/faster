#include "rp.hpp"


int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "reactive_planner", ros::init_options::NoSigintHandler);

	ros::NodeHandle n("~");

	REACT rp;

	// initialize listener callback for state estimate
	ros::Subscriber vic_pose = n.subscribe("vicon", 1, &REACT::stateCB, &rp);
	
	// initialize listener callback for pointcloud
	ros::Subscriber sub_pcl = n.subscribe("camera/cloud", 1, &REACT::pclCB, &rp);

	// initialize listener callback for event
	ros::Subscriber event_sub = n.subscribe("event", 1, &REACT::eventCB, &rp);

	// initialize listener callback for global goal
	ros::Subscriber global_goal_sub = n.subscribe("global_goal", 1, &REACT::global_goalCB, &rp);
	

	// SendCmd timer
	ros::Timer sendGoalTimer = n.createTimer(ros::Duration(rp.plan_eval_time_), &REACT::sendGoal, &rp);

	rp.new_goal_pub = n.advertise<geometry_msgs::PointStamped>("new_global_goal", 1);
	rp.int_goal_pub = n.advertise<sensor_msgs::PointCloud>("int_goal", 1);
	rp.last_goal_pub = n.advertise<geometry_msgs::PointStamped>("last_global_goal",1);
	rp.traj_pub = n.advertise<nav_msgs::Path>("traj",1);
	rp.latency_pub = n.advertise<acl_system::FloatStamped>("latency",1);

	rp.quad_goal_pub = n.advertise<acl_system::QuadGoal>("goal",1);

	// run the code
	// start asynchronous spinner
	ros::AsyncSpinner spinner(2); // Use 2 threads
	spinner.start();
	ros::waitForShutdown();

	return 0;
}