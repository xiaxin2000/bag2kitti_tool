#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <bag2kitti_wr/load_txt.h>

// const int camera_time_sec_index=298; //front camera
// const int camera_time_nsec_index=299; //front camera

const int camera_time_sec_index=300; //rear camera
const int camera_time_nsec_index=301; //rear camera

std::vector<std::string> splitString( const std::string& s, char delimiter ) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream( s );
    // std::cout<<"s = "<<s<<std::endl;
    // std::cout<<"tokenStream = "<<tokenStream<<std::endl;
    while( std::getline( tokenStream, token, delimiter ) ) {
        tokens.push_back( token );
        // std::cout<<"token = "<<token<<std::endl;
    }
    // for (int i = 0; i < tokens.size(); ++i)
    // {
    //     std::cout<<"tokens = "<<tokens[i]<<std::endl;
    // }
    return tokens;
}

// NOTE: This function is not being used however
// it shows how to read a single line from the beginning of the file.
std::string getLineFromFile( const char* filename ) {
    std::ifstream file( filename );
    if( !file ) {
        std::stringstream stream;
        stream << "failed to open file " << filename << '\n';
        throw std::runtime_error( stream.str() );
    }

    std::string line;
    std::getline( file, line );

    file.close();

    return line;
}

void getDataFromFile( const char* filename, std::vector<std::string>& output ) {
    std::ifstream file( filename );
    if( !file ) {
        std::stringstream stream;
        stream << "failed to open file " << filename << '\n';
        throw std::runtime_error( stream.str() );
    }

    std::string line;

    while( std::getline( file, line ) ) {

        if ( line.size() > 0 )
        {
            // std::cout<<"line = "<<line<<std::endl; 
            output.push_back( line );
        }
    }
    file.close();    
}

ros::Time parseDataAsRostime( std::string& line ) {
    // std::cout<<"line = "<<line<<std::endl;
    std::vector<std::string> tokens = splitString( line, ',' ); // Parse Line   
    // for (int i = 0; i < tokens.size(); ++i)//加入这些观察处理中的变量
    // {
    //     std::cout<<"tokens2 = "<<tokens[i]<<std::endl;
    // } 
    // std::cout<<"std::stod( tokens[0] ) = "<<std::stod( tokens[0] )<<std::endl;//加入这些观察处理中的变量
    double sec,nsec;
    sec = std::stod( tokens[camera_time_sec_index-1] );
    nsec = std::stod( tokens[camera_time_nsec_index-1] );
    ros::Time cam_rostime;
    cam_rostime.sec=sec;
    cam_rostime.nsec=nsec;
    return cam_rostime;//std::stod 这个函数将字符串转化成double 类型
}

double parseDataAsDouble( std::string& line, const int index ) {
    // std::cout<<"line = "<<line<<std::endl;
    std::vector<std::string> tokens = splitString( line, ',' ); // Parse Line   
    // for (int i = 0; i < tokens.size(); ++i)//加入这些观察处理中的变量
    // {
    //     std::cout<<"tokens2 = "<<tokens[i]<<std::endl;
    // } 
    // std::cout<<"std::stod( tokens[0] ) = "<<std::stod( tokens[0] )<<std::endl;//加入这些观察处理中的变量
    double varibale_;
    return varibale_ = std::stod( tokens[index-1] );
}


void generate_rostime( std::vector <std::string>& lines, std::vector<ros::Time>& vectors ) {
    for( auto& l : lines ) {
        vectors.push_back( parseDataAsRostime( l ) );
    }
    // return vectors;
}

void generate_variable_double( std::vector <std::string>& lines, std::vector<double>& vectors,const int index ) {
    for( auto& l : lines ) {
        vectors.push_back( parseDataAsDouble( l, index) );
    }
    // return vectors;
}