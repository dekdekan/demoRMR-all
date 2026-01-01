#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <string>
#include <vector>

#include "robot.h"
#ifndef DISABLE_JOYSTICK
#include <QJoysticks.h>
#endif

namespace Ui {
class MainWindow;
}

class QPaintEvent;

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_9_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();

    void paintThisLidar(const LaserMeasurement &laserData);
#ifndef DISABLE_OPENCV
    void paintThisCamera(const cv::Mat &cameraData);
#endif
#ifndef DISABLE_SKELETON
    void paintThisSkeleton(const skeleton &skeledata);
#endif

public slots:
     void setUiValues(double robotX,double robotY,double robotFi);

private:
    robot _robot;
    std::unique_ptr<Ui::MainWindow> ui;
    void paintEvent(QPaintEvent *event) override;
    int updateLaserPicture;
    LaserMeasurement copyOfLaserData;
    int datacounter;
    std::string ipaddress;
};

#endif // MAINWINDOW_H
