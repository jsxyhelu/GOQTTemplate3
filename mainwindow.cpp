#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer   = new QTimer(this);
    imag    = new QImage();         // 初始化
    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
    bMethod = false;//是否使用算法
    //填充界面控件
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras)
    {
          ui->comboCamera->addItem(cameraInfo.description());
    }
    ui->comboCamera->setCurrentIndex(0);
    bCameraOpen = false;
    //显示的图像自动缩放
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
     // 禁止最大化按钮
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    // 禁止拖动窗口大小
    setFixedSize(this->width(),this->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 从摄像头中抓取并返回每一帧
void MainWindow::readFarme()
{   
    videocapture->read(matFrame);
    //是否图像处理
    Mat tmp;

    if(bMethod)//这里只是简单地实现一个Canny算法
    {
        cvtColor(matFrame,tmp,COLOR_BGR2GRAY);
        cv::Canny(tmp,tmp,100,255,3);
        cvtColor(tmp,matDst,COLOR_GRAY2BGR);
    }
    else
    {
        matDst = matFrame.clone();
    }
    // 格式转换
    QPixmap qpixmap = Mat2QImage(matDst);
    // 将图片显示到label上
    ui->label->setPixmap(qpixmap);
    //计算FPS
    double d_FPS = 1.0/(((double)getTickCount()  -  m_lastTime)/getTickFrequency());
    QString strShow =  QString::fromLatin1("FPS: %3 ").arg((int) qRound(d_FPS));
    ui->label_4->setText(strShow);
    //单次算法消耗时间
    d_FPS = 1000 * ((double)getTickCount()  -  m_lastTime)/getTickFrequency();
    strShow = QString::fromLatin1("%3 MS").arg((double) qRound(d_FPS));
    ui->label_5->setText(strShow);
    m_lastTime = (double)getTickCount();
}

//打开磁盘上的图片
void MainWindow::on_pushButton_clicked()
{
     //第3个参数表示查找文件时从哪个目录开始，如果为"."的话，表示从该工程目录开始查找，最后那个参数的过滤器的名字之间
     //要用空格，否则识别不出来
     QString img_name = QFileDialog::getOpenFileName( this, tr("Open Image"), ".",tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
     if(img_name == "")
         return;
     //toAscii()返回8位描述的string,为QByteArray,data()表示返回QByteArray的指针，QByteArray为字节指针
     //现在已改为toLatinl函数
     Mat src = imread(img_name.toLatin1().data());
     cvtColor( src, src, CV_BGR2RGB );
     QImage img = QImage( (const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_RGB888 );
     ui->label->setPixmap( QPixmap::fromImage(img) );
     img_name.clear();
}

//打开摄像头
void MainWindow::on_pushButton_OpenCam_clicked()
{
    int i = ui->comboCamera->currentIndex();
    //打开摄像头，从摄像头中获取视频
    if(bCameraOpen)
    {
         videocapture->release();//kill camera first
    }
    videocapture  = new VideoCapture(i);
    bCameraOpen = true;
    //这里给出了一个非常大的分辨率,如果摄像头没有这么大的分辨率，则会默认打开其支持的最大分辨率
    videocapture->set(CV_CAP_PROP_FRAME_WIDTH, 2592);
    videocapture->set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
    // 开始计时，超时则发出timeout()信号
    timer->start(0);
}

//////////////////////////helper函数//////////////////////////////////////////////////
//格式转换
QPixmap Mat2QImage(Mat src)
{
    QImage img;
    Mat tmp;
    //根据QT的显示方法进行转换
    if(src.channels() == 3)
    {
        cvtColor( src, tmp, CV_BGR2RGB );
        img = QImage( (const unsigned char*)(tmp.data), tmp.cols, tmp.rows, QImage::Format_RGB888 );
    }
    else
    {
        img = QImage( (const unsigned char*)(src.data), src.cols, src.rows, QImage::Format_Grayscale8 );
    }
    QPixmap qimg = QPixmap::fromImage(img) ;
    return qimg;
}
//采集一张图片
void MainWindow::on_pushButton_TakeAPic_clicked()
{
    // 格式转换
    QPixmap qpixmap = Mat2QImage(matDst);
    // 将图片显示到label上
    ui->label_2->setPixmap(qpixmap);
    // 也可以通過imwrite將圖片直接写到磁盘上
    imwrite("/home/helu/dst.jpg",matDst);
}
//关闭摄像头
void MainWindow::on_pushButton_CloseCam_clicked()
{
    timer->stop();         // 停止读取数据。
    if(videocapture->isOpened())
    {
         videocapture->release();//kill camera first
    }
}

void MainWindow::on_pushButton_Method_clicked()
{
    bMethod =!bMethod;
}

void MainWindow::on_pushButton_Method_2_clicked()
{
     ui->label->setWindowFlag(Qt::Window);
     ui->label->showFullScreen();
}

void MainWindow::on_pushButton_Method_3_clicked()
{
  qApp->quit();
}
