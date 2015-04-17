/*#include "ros/ros.h"
#include "gazebo_msgs/ModelStates.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

void poseCallback(const gazebo_msgs::ModelStates& msg)
{
  static tf::TransformBroadcaster br ;
  
  tf::Transform transform ;
  geometry_msgs::Point pp = msg.pose[2].position ;
  geometry_msgs::Quaternion qq = msg.pose[2].orientation ;
  transform.setOrigin( tf::Vector3(pp.x, pp.y, pp.z) ) ;
  tf::Quaternion q(qq.x, qq.y, qq.z, qq.w) ;
  transform.setRotation(q) ;
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pioneer_tf_broadcaster");

  ros::NodeHandle nHandle;
  
  ros::Subscriber sub = nHandle.subscribe("/gazebo/model_states", 10, &poseCallback);

  ros::spin();
  return 0;
}*/

#include "ros/ros.h"
#include "gazebo_msgs/GetModelState.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "pioneer_tf_broadcaster") ;
    ros::NodeHandle n ;
    ros::Rate loop_rate(10);
    ros::ServiceClient client ;
    
    static tf::TransformBroadcaster br ;
  
    tf::Transform transform ;

    std::string modelName = (std::string)"pioneer" ;
    std::string relativeEntityName = (std::string)"world" ;

    gazebo_msgs::GetModelState getModelState ;
    geometry_msgs::Point pp ;
    geometry_msgs::Quaternion qq ;

    while (ros::ok())
    {
        client = n.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state") ;
        getModelState.request.model_name = modelName ;
        getModelState.request.relative_entity_name = relativeEntityName ;
        client.call(getModelState) ;

        pp = getModelState.response.pose.position ;
        qq = getModelState.response.pose.orientation ;
        
        transform.setOrigin( tf::Vector3(pp.x, pp.y, pp.z) ) ;
        tf::Quaternion q(qq.x, qq.y, qq.z, qq.w) ;
        transform.setRotation(q) ;
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link")) ;

        ros::spinOnce();
        loop_rate.sleep() ;
   }
   return 0;
}
