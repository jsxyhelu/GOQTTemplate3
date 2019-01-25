#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <QImage>
#include <QTimer>     // 设置采集数据的间隔时间
#include <QCameraInfo>
using namespace cv;
//helper函数
QPixmap Mat2QImage(Mat src);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_OpenCam_clicked();
    void readFarme();       // 读取当前帧信息
    void on_pushButton_TakeAPic_clicked();
    void on_pushButton_CloseCam_clicked();
    void on_pushButton_Method_2_clicked();
    void on_pushButton_Method_clicked();
    void on_pushButton_Method_3_clicked();

private:
    Ui::MainWindow *ui;
    QTimer    *timer;
    QImage    *imag;
    VideoCapture *videocapture;   // 视频获取结构， 用来作为视频获取函数的一个参数
    Mat       matFrame;        //申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    Mat       matDst;          //处理的图片结果
    bool      bMethod;//是否使用算法
    bool      bCameraOpen;//摄像头是否打开
    double    m_lastTime;//用于计算FPS
};

#endif // MAINWINDOW_H
