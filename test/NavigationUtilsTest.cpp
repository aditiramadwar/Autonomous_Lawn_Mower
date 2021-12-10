/**
 * Copyright (c) 2021 Aditi Ramadwar, Arunava Basu
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <ros/ros.h>
#include <gtest/gtest.h>
#include "NavigationUtils.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <actionlib/client/simple_action_client.h>

TEST(NavigationUtilsTests, convert_To_Quaternion) {
  NavigationUtils nav_test;
  geometry_msgs::Quaternion q_test;
  q_test = nav_test.convertToQuaternion(0.0);
  EXPECT_EQ(1, q_test.w);
}

TEST(NavigationUtilsTests, set_goal) {
  NavigationUtils nav_test;
  move_base_msgs::MoveBaseGoal goal;
  geometry_msgs::Quaternion quart;
  quart.x = 0;
  quart.y = 0;
  quart.z = 0;
  quart.w = 0;
  std::vector<double> position = {0.5, 0};
  nav_test.setDesiredGoal(goal, position, quart);
  EXPECT_EQ(position[0], goal.target_pose.pose.position.x);
}

TEST(NavigationUtilsTests, check_if_Goal_Reached) {
  typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
  MoveBaseClient;
  NavigationUtils nav_test;
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = 0.1;
  goal.target_pose.pose.position.y = 0;
  goal.target_pose.pose.orientation.x = 0;
  goal.target_pose.pose.orientation.y = 0;
  goal.target_pose.pose.orientation.z = 0;
  goal.target_pose.pose.orientation.w = 1;
  MoveBaseClient actionClient("move_base", true);
  while (!actionClient.waitForServer(ros::Duration(1.0))) {
    // Wait for move base server
    ros::spinOnce();
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  actionClient.sendGoal(goal);
  bool success_flag = nav_test.checkGoalReach(actionClient);
  EXPECT_TRUE(success_flag);
}
