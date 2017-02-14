#include <QApplication>
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "MainWindow.h"
#include <thread>
#include "UGVControl.h"

/* The following two functions are what the thread calls, we need this because
 if we don't use threads and just run them in the main function they get held
 there and we have to press run twice, which creates issues. This way each
 thread is executed individually just have to call them at the right time.*/

void rosCoreRun()
{
    system("roscore");
}

void runCam()
{
   // system("source ~/catkin_ws/devel/setup.bash");
    system("roslaunch usb_cam usb_cam-test.launch");
}

void runGazebo()
{
    system("roslaunch husky_gazebo husky_empty_world.launch");
   // system("roslaunch husky_gazebo husky_playpen.launch");
}
int main(int argc, char *argv[])
{
    // Q_INIT_RESOURCE(resfile);
   /* 
    * first thread, used to start roscore, so we don't have to
    * run it from the terminal everytime. Makes it a simple push
    * to start everything. 
    */
    
    //std::thread core(runGazebo);
    //core.detach();
   
    QApplication app(argc, argv);

    ros::init(argc, argv, "husky");
    ros::AsyncSpinner spinner(3);
    UGVControl husky;
   // MockupPart2 m(&husky);
    MainWindow m(&husky);
   // UGVControl moveTester;
   // moveTester.crawl(10);
   // moveTester.turn(10);
   // ros::spin(); 
    m.show();
    spinner.start();
    
    
    
    /*
     * second thread to run camera from terminal. Only runs after
     * the main window has been opened. Called it here because if I called it
     * right after the roscore thread, it won't open due to the fact that
     * roscore isn't finished opening by the time we call usbcam.
     */
    
    //std::thread cam(runCam);
    //cam.detach();
    // create and show your widgets here

    return app.exec();
}

