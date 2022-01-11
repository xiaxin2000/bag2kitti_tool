#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <rosbag/bag.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <rosgraph_msgs/Clock.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>
#include <bag2kitti_wr/load_txt.h>
#include <gps_common/GPSFix.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <pcl/io/pcd_io.h>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <pcl_ros/point_cloud.h>

#include <stdio.h>
#include <time.h>
#include <ros/ros.h>

#include <iostream>           
#include <pcl/io/pcd_io.h>      
#include <pcl/point_types.h>     
using namespace std;


#define foreach BOOST_FOREACH

std::ofstream time_stamp_file;

 std::string stampToString(const ros::Time& stamp, const std::string format="%Y-%m-%d %H:%M:%S")
 {
    const int output_size = 100;
    char output[output_size];
    std::time_t raw_time = static_cast<time_t>(stamp.sec);
    struct tm* timeinfo = localtime(&raw_time);
    std::strftime(output, output_size, format.c_str(), timeinfo);
    std::stringstream ss; 
    ss << std::setw(9) << std::setfill('0') << stamp.nsec;  
    const size_t fractional_second_digits = 4;
    return std::string(output) + "." + ss.str().substr(0, fractional_second_digits);
}

//Transform PCD 2 BIN
void pcd2bin (pcl::PointCloud<pcl::PointXYZI>::Ptr cloud, string& out_file)
{ 

  ofstream bin_file(out_file.c_str(),ios::out|ios::binary|ios::app);
  if(!bin_file.good()) cout<<"Couldn't open "<<out_file<<endl;  

  for (size_t i = 0; i < cloud->points.size (); ++i)
  {
    bin_file.write((char*)&cloud->points[i].x,3*sizeof(float)); 
    bin_file.write((char*)&cloud->points[i].intensity,sizeof(float));
    //cout<<    cloud->points[i]<<endl;
  }
    
  bin_file.close();
}
static std::vector<std::string> file_lists;

//Read the file lists
void read_filelists(const std::string& dir_path,std::vector<std::string>& out_filelsits,std::string type)
{
    struct dirent *ptr;
    DIR *dir;
    dir = opendir(dir_path.c_str());
    out_filelsits.clear();
    while ((ptr = readdir(dir)) != NULL){
        std::string tmp_file = ptr->d_name;
        if (tmp_file[0] == '.')continue;
        if (type.size() <= 0){
            out_filelsits.push_back(ptr->d_name);
        }else{
            if (tmp_file.size() < type.size())continue;
            std::string tmp_cut_type = tmp_file.substr(tmp_file.size() - type.size(),type.size());
            if (tmp_cut_type == type){
                out_filelsits.push_back(ptr->d_name);
            }
        }
    }
}

bool computePairNum(std::string pair1,std::string pair2)
{
    return pair1 < pair2;
}

//Sort the file list
void sort_filelists(std::vector<std::string>& filists,std::string type)
{
    if (filists.empty())return;

    std::sort(filists.begin(),filists.end(),computePairNum);
}

int main(int argc, char** argv)
{
    ros::init(argc,argv,"bag2kitti");
    ros::NodeHandle nh;
    rosbag::Bag bag_input_lidar;

    bag_input_lidar.open("/home/carma/autoware.ai/2021-12-13-13-14-18.bag", rosbag::bagmode::Read);    
    std::string bin_path = "/home/carma/catkin_bag2kitti/bin/";
    std::string PathName_of_txt="/home/carma/catkin_bag2kitti/timestamp.txt";//needing to be changed to customized path
    std::string lidar_topic="/velodyne_points";
    time_stamp_file.open(PathName_of_txt,std::ios::out);
    

    std::vector<std::string> topics;
    topics.push_back(lidar_topic);

    rosbag::View view_lidar(bag_input_lidar, rosbag::TopicQuery(topics));
    rosbag::View::iterator it=view_lidar.begin();

    int j=0;

    for (it; it !=view_lidar.end(); ++it)
    {
        auto m=*it;

        std::string topic=m.getTopic();
        if (topic == lidar_topic)
        {
            sensor_msgs::PointCloud2::ConstPtr msgPtr=m.instantiate<sensor_msgs::PointCloud2>();
            if (msgPtr !=NULL  && msgPtr->header.stamp.sec>1)
            {

                std::stringstream ss;
                std::cout << "j = " << j << std::endl;
                j++;
                ss << j;
                std::string tmp_str = ss.str()+ ".bin";
                std::string bin_file = bin_path + tmp_str;

                pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>);

                sensor_msgs::PointCloud2 pcd_frame=*msgPtr;
                ros::Time lidar_time=pcd_frame.header.stamp;
                std::string time_stamp_str=stampToString(lidar_time);

                std::cout<<"lidar_time.sec = " <<lidar_time.sec <<" lidar_time.nsec = "<< lidar_time.nsec <<std::endl;
                std::cout<<"converted time = "<<time_stamp_str<<std::endl;

                pcl::fromROSMsg(pcd_frame, *cloud);
                pcd2bin( cloud, bin_file);
                time_stamp_file<<time_stamp_str <<endl;
            }
        }
    }

    
    bag_input_lidar.close();

    
    ros::spin();
    return 0;
}

