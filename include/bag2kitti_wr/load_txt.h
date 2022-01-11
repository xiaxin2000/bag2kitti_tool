#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <ros/ros.h>

struct Coordinates_3D {
    double x;
    double y;
    double z;
    Coordinates_3D() = default;
    Coordinates_3D( double xIn, double yIn, double zIn ) :
        x( xIn ), y( yIn ), z( zIn ) 
    {}
};

std::vector<std::string> splitString( const std::string& s, char delimiter );
std::string getLineFromFile( const char* filename );
void getDataFromFile( const char* filename, std::vector<std::string>& output ) ;
ros::Time parseDataAsRostime( std::string& line ) ;
void generate_rostime( std::vector <std::string>& lines, std::vector<ros::Time>& vectors ) ;
void generate_variable_double( std::vector <std::string>& lines, std::vector<double>& vectors,const int index );