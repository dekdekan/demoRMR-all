#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <math.h>
///TOTO JE DEMO PROGRAM...AK SI HO NASIEL NA PC V LABAKU NEPREPISUJ NIC,ALE SKOPIRUJ SI MA NIEKAM DO INEHO FOLDERA
/// AK HO MAS Z GITU A ROBIS NA LABAKOVOM PC, TAK SI HO VLOZ DO FOLDERA KTORY JE JASNE ODLISITELNY OD TVOJICH KOLEGOV
/// NASLEDNE V POLOZKE Projects SKONTROLUJ CI JE VYPNUTY shadow build...
/// POTOM MIESTO TYCHTO PAR RIADKOV NAPIS SVOJE MENO ALEBO NEJAKY INY LUKRATIVNY IDENTIFIKATOR
/// KED SA NAJBLIZSIE PUSTIS DO PRACE, SKONTROLUJ CI JE MIESTO TOHTO TEXTU TVOJ IDENTIFIKATOR
/// AZ POTOM ZACNI ROBIT... AK TO NESPRAVIS, POJDU BODY DOLE... A NIE JEDEN,ALEBO DVA ALE BUDES RAD
/// AK SA DOSTANES NA SKUSKU


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //tu je napevno nastavena ip. treba zmenit na to co ste si zadali do text boxu alebo nejaku inu pevnu. co bude spravna
    ipaddress="127.0.0.1";//192.168.1.11toto je na niektory realny robot.. na lokal budete davat "127.0.0.1"

    ui->setupUi(this);
    datacounter=0;
#ifndef DISABLE_OPENCV
    actIndex=-1;
    useCamera1=false;

#endif


    datacounter=0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    ///prekreslujem obrazovku len vtedy, ked viem ze mam nove data. paintevent sa
    /// moze pochopitelne zavolat aj z inych dovodov, napriklad zmena velkosti okna
    painter.setBrush(Qt::black);//cierna farba pozadia(pouziva sa ako fill pre napriklad funkciu drawRect)
    QPen pero;
    pero.setStyle(Qt::SolidLine);//styl pera - plna ciara
    pero.setWidth(3);//hrubka pera -3pixely
    pero.setColor(Qt::green);//farba je zelena
    QRect rect;
    rect= ui->widget->geometry();//ziskate porametre stvorca,do ktoreho chcete kreslit
    rect.translate(0,15);
    painter.drawRect(rect);
#ifndef DISABLE_OPENCV
    if(useCamera1==true && actIndex>-1)/// ak zobrazujem data z kamery a aspon niektory frame vo vectore je naplneny
    {
        std::cout<<actIndex<<std::endl;
        QImage image = QImage((uchar*)frame[actIndex].data, frame[actIndex].cols, frame[actIndex].rows, frame[actIndex].step, QImage::Format_RGB888  );//kopirovanie cvmat do qimage
        painter.drawImage(rect,image.rgbSwapped());
    }
    else
#endif
    {
        if(updateLaserPicture==1) ///ak mam nove data z lidaru
        {
            updateLaserPicture=0;


            pero.setColor(Qt::red);//farba je zelena
            painter.setPen(pero);
            painter.drawEllipse(QPoint(rect.width()/2+rect.topLeft().x(), rect.height()/2+rect.topLeft().y()),15,15);
            painter.drawLine(QPoint(rect.width()/2+rect.topLeft().x(), rect.height()/2+rect.topLeft().y()),QPoint(rect.width()/2+rect.topLeft().x(), rect.height()/2+rect.topLeft().y()-15));
            pero.setColor(Qt::green);//farba je zelena
            painter.setPen(pero);
            //teraz tu kreslime random udaje... vykreslite to co treba... t.j. data z lidaru
            //   std::cout<<copyOfLaserData.numberOfScans<<std::endl;
#if DEMO_HAS_AMCL
            if (_robot.isAmclReady()) {
                const GridMap &map = _robot.getAmclMap();
                if (map.width > 0 && map.height > 0) {
                    const double cellWidth =
                        static_cast<double>(rect.width()) / map.width;
                    const double cellHeight =
                        static_cast<double>(rect.height()) / map.height;

                    painter.setPen(QColor(100, 100, 100));
                    for (int row = 0; row < map.height; ++row) {
                        for (int column = 0; column < map.width; ++column) {
                            painter.setBrush(
                                map.distanceField[map.index(column, row)] == 1
                                    ? QColor(0, 200, 0)
                                    : Qt::black);
                            painter.drawRect(QRectF(
                                column * cellWidth + rect.left(),
                                row * cellHeight + rect.top(),
                                cellWidth, cellHeight));
                        }
                    }

                    const Particle robotPose = _robot.getBestParticle();
                    painter.setPen(QColor(200, 0, 0));
                    painter.setBrush(QColor(200, 0, 0));
                    int robotGridX = -1;
                    int robotGridY = -1;
                    _robot.getGridCoordinates(robotPose.x, robotPose.y,
                                              robotGridX, robotGridY);
                    if (robotGridX >= 0 && robotGridX < map.width &&
                        robotGridY >= 0 && robotGridY < map.height) {
                        painter.drawEllipse(
                            QPointF((robotGridX + 0.5) * cellWidth + rect.left(),
                                    (robotGridY + 0.5) * cellHeight + rect.top()),
                            4, 4);
                    }

                    for (const LaserData &sample : copyOfLaserData) {
                        const double angle =
                            robotPose.theta -
                            sample.scanAngle * 3.14159265358979323846 / 180.0;
                        const double laserX =
                            robotPose.x + sample.scanDistance * std::cos(angle);
                        const double laserY =
                            robotPose.y + sample.scanDistance * std::sin(angle);
                        int gridX = -1;
                        int gridY = -1;
                        _robot.getGridCoordinates(laserX, laserY, gridX, gridY);
                        if (gridX >= 0 && gridX < map.width &&
                            gridY >= 0 && gridY < map.height) {
                            painter.drawPoint(QPointF(
                                (gridX + 0.5) * cellWidth + rect.left(),
                                (gridY + 0.5) * cellHeight + rect.top()));
                        }
                    }
                }
            } else
#endif
            {
            for(const auto &k :copyOfLaserData)
            {
                int dist=k.scanDistance/20; ///vzdialenost nahodne predelena 20 aby to nejako vyzeralo v okne.. zmen podla uvazenia
                int xp=rect.width()-(rect.width()/2+dist*2*sin((360.0-k.scanAngle)*3.14159/180.0))+rect.topLeft().x(); //prepocet do obrazovky
                int yp=rect.height()-(rect.height()/2+dist*2*cos((360.0-k.scanAngle)*3.14159/180.0))+rect.topLeft().y();//prepocet do obrazovky
                if(rect.contains(xp,yp))//ak je bod vo vnutri nasho obdlznika tak iba vtedy budem chciet kreslit
                    painter.drawEllipse(QPoint(xp, yp),2,2);
            }
            }
        }
    }
#ifndef DISABLE_SKELETON
    if(updateSkeletonPicture==1 )
    {
        painter.setPen(Qt::red);
        for(int i=0;i<75;i++)
        {
            int xp=rect.width()-rect.width() * skeleJoints.joints[i].x+rect.topLeft().x();
            int yp= (rect.height() *skeleJoints.joints[i].y)+rect.topLeft().y();
            if(rect.contains(xp,yp))
                painter.drawEllipse(QPoint(xp, yp),2,2);
        }
    }
#endif
}


/// toto je slot. niekde v kode existuje signal, ktory je prepojeny. pouziva sa napriklad (v tomto pripade) ak chcete dostat data z jedneho vlakna (robot) do ineho (ui)
/// prepojenie signal slot je vo funkcii  on_pushButton_9_clicked
void  MainWindow::setUiValues(double robotX,double robotY,double robotFi)
{
    ui->lineEdit_2->setText(QString::number(robotX));
    ui->lineEdit_3->setText(QString::number(robotY));
    ui->lineEdit_4->setText(QString::number(robotFi));
}

#if DEMO_HAS_AMCL
void MainWindow::setUiAMCLValues(double robotX, double robotY,
                                 double robotTheta)
{
    setUiValues(robotX, robotY, robotTheta);
}
#endif


void MainWindow::on_pushButton_9_clicked() //start button
{
    //ziskanie joystickov


    //tu sa nastartuju vlakna ktore citaju data z lidaru a robota



    connect(&_robot,SIGNAL(publishPosition(double,double,double)),this,SLOT(setUiValues(double,double,double)));
    connect(&_robot,SIGNAL(publishLidar(const std::vector<LaserData> &)),this,SLOT(paintThisLidar(const std::vector<LaserData> &)));
#ifndef DISABLE_OPENCV
    connect(&_robot,SIGNAL(publishCamera(const cv::Mat &)),this,SLOT(paintThisCamera(const cv::Mat &)));
#endif
#ifndef DISABLE_SKELETON
    connect(&_robot,SIGNAL(publishSkeleton(const skeleton &)),this,SLOT(paintThisSkeleton(const skeleton &)));
#endif
#if DEMO_HAS_AMCL
    connect(&_robot, &robot::publishAMCLPosition,
            this, &MainWindow::setUiAMCLValues);
#endif

    _robot.initAndStartRobot(ipaddress);

    #ifndef DISABLE_JOYSTICK
        instance = QJoysticks::getInstance();
    /// prepojenie joysticku s jeho callbackom... zas cez lambdu. neviem ci som to niekde spominal,ale lambdy su super. okrem toho mam este rad ternarne operatory a spolocneske hry ale to tiez nikoho nezaujima
    /// co vas vlastne zaujima? citanie komentov asi nie, inak by ste citali toto a ze tu je blbosti
    connect(
                instance, &QJoysticks::axisChanged,
                [this]( const int js, const int axis, const qreal value) {
        double forw=0, rot=0;
        if(/*js==0 &&*/ axis==1){forw=-value*300;}
        if(/*js==0 &&*/ axis==0){rot=-value*(3.14159/2.0);}
        this->_robot.setSpeedVal(forw,rot);
    }
    );
#endif
}

void MainWindow::on_pushButton_2_clicked() //forward
{
    //pohyb dopredu
    _robot.setSpeed(500,0);

}

void MainWindow::on_pushButton_3_clicked() //back
{
    _robot.setSpeed(-250,0);

}

void MainWindow::on_pushButton_6_clicked() //left
{
    _robot.setSpeed(0,3.14159/2);

}

void MainWindow::on_pushButton_5_clicked()//right
{
    _robot.setSpeed(0,-3.14159/2);

}

void MainWindow::on_pushButton_4_clicked() //stop
{
    _robot.setSpeed(0,0);

}




void MainWindow::on_pushButton_clicked()
{
#ifndef DISABLE_OPENCV
    if(useCamera1==true)
    {
        useCamera1=false;

        ui->pushButton->setText("use camera");
    }
    else
    {
        useCamera1=true;

        ui->pushButton->setText("use laser");
    }
#endif
}





int MainWindow::paintThisLidar(const std::vector<LaserData> &laserData)
{
    copyOfLaserData=laserData;
    //memcpy( &copyOfLaserData,&laserData,sizeof(LaserMeasurement));
    updateLaserPicture=1;

    update();
    return 0;
}

#ifndef DISABLE_OPENCV

///toto je calback na data z kamery, ktory ste podhodili robotu vo funkcii initAndStartRobot
/// vola sa ked dojdu nove data z kamery
int MainWindow::paintThisCamera(const cv::Mat &cameraData)
{

    cameraData.copyTo(frame[(actIndex+1)%3]);//kopirujem do nasej strukury
    actIndex=(actIndex+1)%3;//aktualizujem kde je nova fotka


    updateLaserPicture=1;

    return 0;
}
#endif

#ifndef DISABLE_SKELETON
int MainWindow::paintThisSkeleton(const skeleton &skeledata)
{
    memcpy(&skeleJoints,&skeledata,sizeof(skeleton));

    updateSkeletonPicture=1;
    return 0;
}
#endif
