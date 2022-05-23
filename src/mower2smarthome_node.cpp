#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "mower2smarthome/mqtt.h"
#include "mower_msgs/Status.h"

mower_msgs::Status mower_status;

void mowerStatusReceived(const  mower_msgs::Status::ConstPtr & msg)
  {

       mower_status = *msg;
       ROS_INFO("I heard: charge : %4.2f V : Bat : %4.2f V : load : %4.2f A : tmm : %4.2f C : tem : %4.2f C : tel : %4.2f c : ter : %4.2f c",
           mower_status.v_charge, mower_status.v_battery, mower_status.charge_current, mower_status.mow_esc_status.temperature_motor, mower_status.mow_esc_status.temperature_pcb, mower_status.left_esc_status.temperature_pcb, 
           mower_status.right_esc_status.temperature_pcb );
       
   }


int main(int argc, char **argv)
{

  ros::Publisher  mqtt_pub;
  ros::Subscriber mower_lowlevel_state;

  mower2smarthome::mqtt mqtt_msg;
 


  ros::init(argc, argv, "serve_mqtt");

  ros::NodeHandle n;

   
   mqtt_pub = n.advertise<mower2smarthome::mqtt>("mower/2mqtt", 1);
   mower_lowlevel_state = n.subscribe("mower/status", 0, mowerStatusReceived);
   
 
  ros::Rate loop_rate(1);




  int count = 0;
  while (ros::ok())
  {

    mqtt_msg.stamp =  ros::Time::now();
    mqtt_msg.v_charge = mower_status.v_charge;
    mqtt_msg.charge_current = mower_status.charge_current;
    mqtt_msg.v_battery = mower_status.v_battery;
    mqtt_msg.emergency = mower_status.emergency;
    mqtt_msg.mower_temp = mower_status.mow_esc_status.temperature_motor;
    mqtt_msg.esc_mower_temp= mower_status.mow_esc_status.temperature_pcb;
    mqtt_msg.esc_mower_left= mower_status.left_esc_status.temperature_pcb;
    mqtt_msg.esc_mower_right= mower_status.right_esc_status.temperature_pcb;










    std_msgs::String msg;
    std::stringstream ss;
    ss << "mower2smarthome " << count;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());


   
    mqtt_pub.publish(mqtt_msg);

    
    ros::spinOnce();

    loop_rate.sleep();
    ++count;

  }


  return 0;
}

