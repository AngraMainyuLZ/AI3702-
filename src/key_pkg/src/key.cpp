#include "ros/ros.h"
#include "std_msgs/String.h"
#include "key_pkg/key.h"

#define KEYBOARD_INPUT_TOPIC  "/robot/keyboard_input"

int main(int argc, char **argv) {
  ros::init(argc, argv, "keyboard_listener");
  ros::NodeHandle nh;
  ros::Publisher key_pub = nh.advertise<std_msgs::String>(KEYBOARD_INPUT_TOPIC, 10);

  Keyboard_ctrl keyboard;
  ros::Rate loop_rate(10);  // 10Hz

  while (ros::ok()) {
    int key = keyboard.get_keyboard_press_key();

    // valid key input
    if(key > 0){
      std_msgs::String msg;
      // key
      if(key == KEY_Q){                           // q for exit
        msg.data = std::string(1, (char)key);
        key_pub.publish(msg);
        usleep(500 * 1000);
        return 0;
      }else if(key == KEY_UP){
        msg.data = std::string("UP");
      }else if(key == KEY_DOWN){
        msg.data = std::string("DOWN");
      }else if(key == KEY_RIGHT){
        msg.data = std::string("RIGHT");
      }else if(key == KEY_LEFT){
        msg.data = std::string("LEFT");
      }else{
        msg.data = std::string(1, (char)key);
        // std::cout << key << std::endl;
      }


      key_pub.publish(msg);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
