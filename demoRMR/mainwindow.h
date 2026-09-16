#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#ifdef _WIN32
#define NOMINMAX
#include<windows.h>
#endif
#include<iostream>
//#include<arpa/inet.h>
//#include<unistd.h>
//#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
//#include "ckobuki.h"
//#include "rplidar.h"

#define DISABLE_AMCL
#include "robot.h"
#ifndef DISABLE_JOYSTICK
#include <QJoysticks.h>
#endif
namespace Ui {
class MainWindow;
}

///toto je trieda s oknom.. ktora sa spusti ked sa spusti aplikacia.. su tu vsetky gombiky a spustania...
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    #ifndef DISABLE_OPENCV
    bool useCamera1;
    int actIndex;
    cv::Mat frame[3];
#endif

#ifndef DISABLE_SKELETON
    int updateSkeletonPicture;
        skeleton skeleJoints;
#endif
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();


    int paintThisLidar(const LaserMeasurement &laserData);
#ifndef DISABLE_OPENCV
    int paintThisCamera(const cv::Mat &cameraData);
#endif
#ifndef DISABLE_SKELETON
    int paintThisSkeleton(const skeleton &skeledata);
#endif
    void on_pushButton_10_clicked();

private:

    robot _robot;
    //--skuste tu nic nevymazat... pridavajte co chcete, ale pri odoberani by sa mohol stat nejaky drobny problem, co bude vyhadzovat chyby
    Ui::MainWindow *ui;
     void paintEvent(QPaintEvent *event);// Q_DECL_OVERRIDE;
     int updateLaserPicture;
     LaserMeasurement copyOfLaserData;
         int datacounter;
     std::string ipaddress;


     QTimer *timer;
#ifndef DISABLE_JOYSTICK
     QJoysticks *instance;
#endif
  public slots:
     void setUiValues(double robotX,double robotY,double robotFi);
#ifndef DISABLE_AMCL
     void setUiAMCLValues(double robotX,double robotY,double robotFi);
#endif

};

#endif // MAINWINDOW_H
