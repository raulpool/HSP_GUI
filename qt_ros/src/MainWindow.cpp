#include "MainWindow.h"
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <QTextEdit>
#include <QKeyEvent>
#include "UGVControl.h"




MainWindow::MainWindow(UGVControl *husky):
    linear_scale(10),// was two // makes it so it goes further
    angular_scale(10),// was two and a comma 
    linear(0),
    angular(0)

{
    
    widget.setupUi(this);
    this->husky = husky;

    pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel", 100);
    //pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("/joy_teleop/cmd_vel", 1);
    
    image_transport::ImageTransport it{nh};
    
    sub_img = it.subscribe("/axis/image_raw_out", 1000, &MainWindow::ImageCallback, this);    
    //sub_img = it.subscribe("/usb_cam/image_raw", 100, &MainWindow::ImageCallback, this);

  //  connect(widget.btn_up, &QWidget::keyPressEvent()
    //        this, &MainWindow::OnUpClicked);
    connect(widget.up_Button, &QPushButton::pressed,
            this, &MainWindow::OnUpClicked);
    connect(widget.up_Button, &QPushButton::released,
            this, &MainWindow::OnReleased);
    
    //connect(widget.btn_up, &QPushButton::grabKeyboard,
      //      this,&MainWindow::OnRightClicked);
    
    
    connect(widget.left_Button, &QPushButton::pressed,
            this, &MainWindow::OnLeftClicked);
    connect(widget.left_Button, &QPushButton::released,
            this, &MainWindow::OnReleased);
    
    connect(widget.right_Button, &QPushButton::pressed,
            this, &MainWindow::OnRightClicked);
    connect(widget.right_Button, &QPushButton::released,
            this, &MainWindow::OnReleased);
    
    connect(widget.down_Button, &QPushButton::pressed,
            this, &MainWindow::OnDownClicked);
    connect(widget.down_Button, &QPushButton::released,
            this, &MainWindow::OnReleased);
    
    connect(widget.close_button, &QPushButton::clicked,
            this, &MainWindow::OnCloseClicked);
    
    connect(widget.go_Button, &QPushButton::clicked,
            this, &MainWindow::OnGoClicked);
    
    //connect(widget.Go_Button, &QPushButton::released,
       //     this, &MainWindow::OnReleased);
    
    connect(widget.stop_Button, &QPushButton::clicked,
            this, &MainWindow::OnStopClicked);
    
    connect(widget.update_Button, &QPushButton::pressed,
            this,&MainWindow::UpdateButtonClicked);
    connect(widget.update_Button, &QPushButton::released,
            this,&MainWindow::OnReleased);
    
    connect(widget.auto_Button, &QPushButton::clicked,
            this,&MainWindow::AutoButtonClicked);
    
    
    // connect(widget.horizontal_Slider, &QSlider::actionTriggered,
      //      this,&MainWindow::OnRightClicked);
    
    
  
    /* connect(widget.Go_Button, &QPushButton::clicked,
            this, &QProgressBar::value(100)); */
    widget.text_Edit_Box->setText("Hello I am a Log Box\n");
   
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::ImageCallback(const sensor_msgs::ImageConstPtr& in)
{
    ROS_WARN_STREAM("in image callback");
    
    QImage::Format f = QImage::Format_RGB888;
    QImage img(in->data.data(), in->width, in->height, in->step, f);
    img = img.rgbSwapped();
    
    QPixmap pic = QPixmap::fromImage(img);
    
    int w = widget.image_frame->width();
    int h = widget.image_frame->height();
    
    pic = pic.scaled(w, h, Qt::KeepAspectRatio);
    widget.image_frame->setPixmap(pic);
}

void MainWindow::TranslateAndPublish()
{
    geometry_msgs::Twist twist;
    twist.angular.z = angular_scale * angular;
    twist.linear.x = linear_scale * linear;
    //twist.linear.y = linear_scale * linear; 
    pub_cmd_vel.publish(twist);
    
    linear = angular = 0;
}


void MainWindow::OnLeftClicked()
{   
    widget.text_Edit_Box->append("Moving husky to the left. \n");
    //system("rostopic pub joy_teleop/cmd_vel geometry_msgs/Twist '[0,0,0]' '[0,0,-1.0]'");
    husky -> turn(5);
}

void MainWindow::OnReleased()
{
    widget.text_Edit_Box->insertPlainText("Releasing the button\n");
    husky -> stop();
}

void MainWindow::OnRightClicked()
{
    widget.text_Edit_Box->insertPlainText("Moving husky to the right. \n");
    husky -> turn(-5);
}

void MainWindow::OnUpClicked()
{
    widget.text_Edit_Box->insertPlainText("Moving husky up. \n");
    husky -> crawl(10);
    
}

void MainWindow::OnDownClicked()
{
    widget.text_Edit_Box->insertPlainText("Moving husky down. \n");
    husky -> crawl(-10);
}

void MainWindow::OnCloseClicked()
{
  
   this ->close();
   // system("rosnode kill usb_cam");
    //system("rosnode kill image_view");
    
}

void MainWindow::OnGoClicked() // this will just make the turtle sim go out 10 spaces
{
   // ROS_WARN_STREAM("in On Go Clicked will go forth 10 spaces");
    widget.text_Edit_Box->insertPlainText("Will go forward 10 spaces\n");
    husky -> crawl(10);
    //widget.textEdit->setText("Will go backwards 5 spaces\n");
    //husky -> crawl(-5);
    //linear = 10;
    //angular = 10;
    //this -> TranslateAndPublish();
}

void MainWindow::OnStopClicked()
{
    // if you use append itll just add instead of replace the string
    widget.text_Edit_Box->insertPlainText("Stopping movement\n");
    husky -> stop();
    //linear = 0;
    //angular = 0;
    //this -> TranslateAndPublish();
}

// testing the input system
void MainWindow::UpdateButtonClicked()
{ 
    //tring text;
    //xt -> 
    widget.text_Edit_Box->insertPlainText("Moving to location. \n");
    double lat = 0;
    double longitude = 0;
   // lat = widget.lattitude_Box -> text().toDouble();
    longitude = widget.longitude_Box -> text().toDouble();
    
    husky -> crawl(lat);
    husky -> turn(longitude);
    //linear = lat;
   // this -> TranslateAndPublish();
   // angular = longitude;
  //  this -> TranslateAndPublish();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
 {
        //if(e)
    if (e->key() == Qt::Key_Left)
   {
        this->OnLeftClicked();

    }
    else if (e->key() == Qt::Key_Up)
    {
        this->OnUpClicked();

    }
    else if (e->key() == Qt::Key_Right)
    {
        this->OnRightClicked();
    }
    else if (e->key() == Qt::Key_Down)
    {
         this->OnDownClicked();

    }
}
void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    widget.text_Edit_Box->append("Releasing the button\n");
    husky -> stop();
}

void MainWindow::AutoButtonClicked()
{
    system("google-chrome https://www.youtube.com/watch?v=dQw4w9WgXcQ");
}

// check if there is a way to send commands to a terminal in qt