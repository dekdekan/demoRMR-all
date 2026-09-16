#ifndef LIBROBOT_H
#define LIBROBOT_H
#define DISABLE_AMCL
#ifndef DISABLE_OPENCV
#define useCamera
#endif
#ifdef useCamera
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"
#endif

#include "robot_global.h"
#include "rplidar.h"
#include "CKobuki.h"
#include <thread>
#include <functional>
#include <list>
#include <algorithm>
#include <utility>
#include <atomic>
#include <mutex>
#include <future>
#include <random>
#include <iostream>
#include <memory>
#include "udp_communication.h"
#include "amcl.h"
#include "skeleton.h"
class ROBOT_EXPORT libRobot
{
public:
    ~libRobot();
    libRobot(std::string ipaddressLaser="127.0.0.1",int laserportRobot=52999, int laserportMe=5299,std::function<int(LaserMeasurement)> &lascallback=do_nothing_laser,std::string ipaddressRobot="127.0.0.1",int robotportRobot=53000, int robotportMe=5300,std::function<int(TKobukiData)> &robcallback=do_nothing_robot);


    //default functions.. please do not rewrite.. make your own callback
    static  std::function<int(TKobukiData)> do_nothing_robot;
    static std::function<int(LaserMeasurement)> do_nothing_laser;


    void robotStart();
    void setLaserParameters(std::string ipaddress,int laserportRobot, int laserportMe,std::function<int(LaserMeasurement)> callback )
    {
        laser_ip_portOut=laserportRobot;
        laser_ip_portIn=laserportMe;
        laser_ipaddress=ipaddress;
        laser_callback=callback;
        wasLaserSet=1;
    }
    void setRobotParameters(std::string ipaddress,int robotportRobot, int robotportMe,std::function<int(TKobukiData)> callback )
    {
        robot_ip_portOut=robotportRobot;
        robot_ip_portIn=robotportMe;
        robot_ipaddress=ipaddress;
        robot_callback=callback;
        wasRobotSet=1;
    }
#ifndef DISABLE_AMCL
    void setAMCLParameters(std::string mappath,int particleCount,double rotSTD,double transSTD,std::function<int(float x, float y,float fi)> callback )
    {
       
        wasAMCLSet=1;
        amcl_callback=callback;
        amcld.loadGridMap(/*"mapa.txt"*/mappath,amclmap);
        amcld.setSTDs(transSTD,rotSTD);
        particles=amcld.initializeParticles(particleCount,amclmap);
    }
    GridMap &getAmclMap(){return amclmap;}
    Particle &getBestParticle(){return bestparticle;}
    void getGridCoordinates(double realX,double realY, int &gX,int &gY){amcld.worldToGrid(realX, realY, gX, gY, amclmap);}

#endif
    void setTranslationSpeed(int mmpersec);

    void setRotationSpeed(double radpersec);
    void setArcSpeed(int mmpersec,int radius);
#ifndef DISABLE_OPENCV
    void setCameraParameters(std::string link,std::function<int(cv::Mat)> callback )
    {

        camera_link=link;
        camera_callback=callback;
        wasCameraSet=1;
    }
#endif

#ifndef DISABLE_SKELETON
    void setSkeletonParameters(std::string ipaddress,int skeletonportRobot, int skeletonportMe,std::function<int(skeleton)> callback )
       {
           skeleton_ip_portOut=skeletonportRobot;
           skeleton_ip_portIn=skeletonportMe;
           skeleton_ipaddress=ipaddress;
           skeleton_callback=callback;
           wasSkeletonSet=1;
       }

#endif
    long double tickToMeter = 0.000085292090497737556558; // [m/tick]
    long double b = 0.23; // wheelbase distance in meters, from kobuki manual https://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html

private:
     std::promise<void> ready_promise;
    std::shared_future<void> readyFuture;
    int wasLaserSet;
    int wasRobotSet;
    int wasCameraSet;
    int wasSkeletonSet;
    int wasAMCLSet;
    //veci na laser
    LaserMeasurement copyOfLaserData;
    void laserprocess();
    std::string laser_ipaddress;
    int laser_ip_portOut;
    int laser_ip_portIn;
    std::thread laserthreadHandle;
    std::function<int(LaserMeasurement)> laser_callback=nullptr;

    //veci pre podvozok
    CKobuki robot;
    TKobukiData sens;
    std::string robot_ipaddress;
    int robot_ip_portOut;
    int robot_ip_portIn;
    std::thread robotthreadHandle;
    void robotprocess();
    std::function<int(TKobukiData)> robot_callback=nullptr;

    udp_communication laserCom;
    udp_communication robotCom;

    //veci pre kameru -- pozor na kameru, neotvarat ak nahodou chcete kameru pripojit na detekciu kostry...
#ifndef DISABLE_OPENCV
    std::string camera_link;
    std::thread camerathreadhandle;
    std::function<int(cv::Mat)> camera_callback=nullptr;
    void imageViewer();
#endif

#ifndef DISABLE_SKELETON
    //--veci pre kostru
    udp_communication skeletonCom;
        std::thread skeletonthreadHandle;
        std::string skeleton_ipaddress;
        int skeleton_ip_portOut;
        int skeleton_ip_portIn;
        void skeletonprocess();
        std::function<int(skeleton)> skeleton_callback=nullptr;
#endif
#ifndef DISABLE_AMCL

        GridMap amclmap;
        int gx,gy;
          amcl amcld;
        Particle bestparticle;
        float dist;
        float theta;
        std::vector<Particle> particles;
        void calculateAMCL(TKobukiData sens,float &ddist, float &dtheta);
        std::function<int(float x, float y,float fi)> amcl_callback=nullptr;
#endif
};

#endif // LIBROBOT_H
