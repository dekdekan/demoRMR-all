#include "robot.h"
#include <iostream>

robot::robot(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<LaserMeasurement>("LaserMeasurement");
#ifndef DISABLE_OPENCV
    qRegisterMetaType<cv::Mat>("cv::Mat");
#endif
#ifndef DISABLE_SKELETON
    qRegisterMetaType<skeleton>("skeleton");
#endif
}

void robot::initAndStartRobot(const std::string &ipaddress)
{
    forwardspeed = 0;
    rotationspeed = 0;
    robotCom.setLaserParameters(ipaddress, 52999, 5299, [this](LaserMeasurement dat) { return this->processThisLidar(dat); });
    robotCom.setRobotParameters(ipaddress, 53000, 5300, [this](TKobukiData dat) { return this->processThisRobot(dat); });
#ifndef DISABLE_OPENCV
    robotCom.setCameraParameters("http://" + ipaddress + ":8000/stream.mjpg", [this](cv::Mat dat) { return this->processThisCamera(dat); });
#endif
#ifndef DISABLE_SKELETON
    robotCom.setSkeletonParameters("127.0.0.1", 23432, 23432, [this](skeleton dat) { return this->processThisSkeleton(dat); });
#endif
    robotCom.robotStart();
}

void robot::setSpeedVal(double forw, double rots)
{
    forwardspeed = forw;
    rotationspeed = rots;
    useDirectCommands = 0;
}

void robot::setSpeed(double forw, double rots)
{
    if (forw == 0 && rots != 0)
        robotCom.setRotationSpeed(rots);
    else if (forw != 0 && rots == 0)
        robotCom.setTranslationSpeed(forw);
    else if (forw != 0 && rots != 0)
        robotCom.setArcSpeed(forw, forw / rots);
    else
        robotCom.setTranslationSpeed(0);
    useDirectCommands = 1;
}

int robot::processThisRobot(TKobukiData robotdata)
{
    if (datacounter % 5 == 0)
    {
        emit publishPosition(x, y, fi);
    }

    if (useDirectCommands == 0)
    {
        if (forwardspeed == 0 && rotationspeed != 0)
            robotCom.setRotationSpeed(rotationspeed);
        else if (forwardspeed != 0 && rotationspeed == 0)
            robotCom.setTranslationSpeed(forwardspeed);
        else if (forwardspeed != 0 && rotationspeed != 0)
            robotCom.setArcSpeed(forwardspeed, forwardspeed / rotationspeed);
        else
            robotCom.setTranslationSpeed(0);
    }
    datacounter++;
    return 0;
}

int robot::processThisLidar(LaserMeasurement laserData)
{
    copyOfLaserData = laserData;
    emit publishLidar(copyOfLaserData);
    return 0;
}

#ifndef DISABLE_OPENCV
int robot::processThisCamera(cv::Mat cameraData)
{
    cameraData.copyTo(frame[(actIndex + 1) % 3]);
    actIndex = (actIndex + 1) % 3;
    emit publishCamera(frame[actIndex]);
    return 0;
}
#endif

#ifndef DISABLE_SKELETON
int robot::processThisSkeleton(skeleton skeledata)
{
    skeleJoints = skeledata;
    emit publishSkeleton(skeleJoints);
    return 0;
}
#endif
