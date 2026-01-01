#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <cmath>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::MainWindow>())
{
    ipaddress = "127.0.0.1";
    ui->setupUi(this);
    datacounter = 0;
#ifndef DISABLE_OPENCV
    actIndex = -1;
    useCamera1 = false;
#endif
    datacounter = 0;
}

MainWindow::~MainWindow() = default;

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(Qt::green);
    QRect rect = ui->widget->geometry();
    rect.translate(0, 15);
    painter.drawRect(rect);
#ifndef DISABLE_OPENCV
    if (useCamera1 && actIndex > -1)
    {
        QImage image((uchar*)frame[actIndex].data, frame[actIndex].cols, frame[actIndex].rows, frame[actIndex].step, QImage::Format_RGB888);
        painter.drawImage(rect, image.rgbSwapped());
    }
    else
#endif
    {
        if (updateLaserPicture == 1)
        {
            updateLaserPicture = 0;

            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawEllipse(QPoint(rect.width() / 2 + rect.topLeft().x(), rect.height() / 2 + rect.topLeft().y()), 15, 15);
            painter.drawLine(QPoint(rect.width() / 2 + rect.topLeft().x(), rect.height() / 2 + rect.topLeft().y()), QPoint(rect.width() / 2 + rect.topLeft().x(), rect.height() / 2 + rect.topLeft().y() - 15));
            pen.setColor(Qt::green);
            painter.setPen(pen);

            for (int k = 0; k < copyOfLaserData.numberOfScans; k++)
            {
                int dist = copyOfLaserData.Data[k].scanDistance / 20;
                int xp = rect.width() - (rect.width() / 2 + dist * 2 * std::sin((360.0 - copyOfLaserData.Data[k].scanAngle) * M_PI / 180.0)) + rect.topLeft().x();
                int yp = rect.height() - (rect.height() / 2 + dist * 2 * std::cos((360.0 - copyOfLaserData.Data[k].scanAngle) * M_PI / 180.0)) + rect.topLeft().y();
                if (rect.contains(xp, yp))
                    painter.drawEllipse(QPoint(xp, yp), 2, 2);
            }
        }
    }
#ifndef DISABLE_SKELETON
    if (updateSkeletonPicture == 1)
    {
        painter.setPen(Qt::red);
        for (int i = 0; i < 75; i++)
        {
            int xp = rect.width() - rect.width() * skeleJoints.joints[i].x + rect.topLeft().x();
            int yp = (rect.height() * skeleJoints.joints[i].y) + rect.topLeft().y();
            if (rect.contains(xp, yp))
                painter.drawEllipse(QPoint(xp, yp), 2, 2);
        }
    }
#endif
}

void MainWindow::setUiValues(double robotX, double robotY, double robotFi)
{
    ui->lineEdit_2->setText(QString::number(robotX));
    ui->lineEdit_3->setText(QString::number(robotY));
    ui->lineEdit_4->setText(QString::number(robotFi));
}

void MainWindow::on_pushButton_9_clicked() //start button
{
    connect(&_robot, &robot::publishPosition, this, &MainWindow::setUiValues);
    connect(&_robot, &robot::publishLidar, this, &MainWindow::paintThisLidar);
#ifndef DISABLE_OPENCV
    connect(&_robot, &robot::publishCamera, this, &MainWindow::paintThisCamera);
#endif
#ifndef DISABLE_SKELETON
    connect(&_robot, &robot::publishSkeleton, this, &MainWindow::paintThisSkeleton);
#endif

    _robot.initAndStartRobot(ipaddress);

#ifndef DISABLE_JOYSTICK
    connect(
        QJoysticks::getInstance(), &QJoysticks::axisChanged,
        [this](const int js, const int axis, const qreal value) {
            double forw = 0, rot = 0;
            if (axis == 1) { forw = -value * 300; }
            if (axis == 0) { rot = -value * (M_PI / 2.0); }
            this->_robot.setSpeedVal(forw, rot);
        }
    );
#endif
}

void MainWindow::on_pushButton_2_clicked() //forward
{
    _robot.setSpeed(500, 0);
}

void MainWindow::on_pushButton_3_clicked() //back
{
    _robot.setSpeed(-250, 0);
}

void MainWindow::on_pushButton_6_clicked() //left
{
    _robot.setSpeed(0, M_PI_2);
}

void MainWindow::on_pushButton_5_clicked() //right
{
    _robot.setSpeed(0, -M_PI_2);
}

void MainWindow::on_pushButton_4_clicked() //stop
{
    _robot.setSpeed(0, 0);
}

void MainWindow::on_pushButton_clicked()
{
#ifndef DISABLE_OPENCV
    useCamera1 = !useCamera1;
    ui->pushButton->setText(useCamera1 ? "use laser" : "use camera");
#endif
}

void MainWindow::paintThisLidar(const LaserMeasurement &laserData)
{
    copyOfLaserData = laserData;
    updateLaserPicture = 1;
    update();
}

#ifndef DISABLE_OPENCV
void MainWindow::paintThisCamera(const cv::Mat &cameraData)
{
    cameraData.copyTo(frame[(actIndex + 1) % 3]);
    actIndex = (actIndex + 1) % 3;
    updateLaserPicture = 1;
}
#endif

#ifndef DISABLE_SKELETON
void MainWindow::paintThisSkeleton(const skeleton &skeledata)
{
    skeleJoints = skeledata;
    updateSkeletonPicture = 1;
}
#endif
