#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "librobot.h"
#include <string>

#ifndef DISABLE_OPENCV
#include <opencv2/core/mat.hpp>
Q_DECLARE_METATYPE(cv::Mat)
#endif

#ifndef DISABLE_SKELETON
Q_DECLARE_METATYPE(skeleton)
#endif

Q_DECLARE_METATYPE(LaserMeasurement)

class robot : public QObject
{
    Q_OBJECT

public:
    explicit robot(QObject *parent = nullptr);

    void initAndStartRobot(const std::string &ipaddress);
    void setSpeedVal(double forw, double rots);
    void setSpeed(double forw, double rots);

signals:
    void publishPosition(double x, double y, double z);
    void publishLidar(const LaserMeasurement &lidata);
#ifndef DISABLE_OPENCV
    void publishCamera(const cv::Mat &camframe);
#endif
#ifndef DISABLE_SKELETON
    void publishSkeleton(const skeleton &skeledata);
#endif

private:
    double x = 0.0;
    double y = 0.0;
    double fi = 0.0;
    double forwardspeed = 0.0;
    double rotationspeed = 0.0;

    int processThisLidar(LaserMeasurement laserData);
    int processThisRobot(TKobukiData robotdata);
#ifndef DISABLE_OPENCV
    int processThisCamera(cv::Mat cameraData);
#endif

    LaserMeasurement copyOfLaserData;
#ifndef DISABLE_OPENCV
    cv::Mat frame[3];
#endif
    libRobot robotCom;
    int datacounter = 0;
#ifndef DISABLE_OPENCV
    bool useCamera1 = false;
    int actIndex = 0;
#endif

#ifndef DISABLE_SKELETON
    int processThisSkeleton(skeleton skeledata);
    int updateSkeletonPicture = 0;
    skeleton skeleJoints;
#endif
    int useDirectCommands = 0;
};

#endif // ROBOT_H
