#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/utils/system.h>


/*
 * You may want to add macros here.
 */
#define TIME_STEP 64
#define MAX_SPEED 6.28

//enum for blob identification
enum BLOBS { RED,notRED,NONE };
/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();
  int red, blue, green;
  int i, j;
  
  enum BLOBS blob;
 
  

  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */
    WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
    WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
    
    //camera enable and variables for get color
    WbDeviceTag camera = wb_robot_get_device("camera");
    wb_camera_enable(camera, TIME_STEP);
    int width = wb_camera_get_width(camera);
    int height = wb_camera_get_height(camera);

    
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);

    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
    
    /*WbDeviceTag prox_sensors[8];
    char prox_sensor_name[50];
    for (int ind =0; ind < 8; ++ind)
    {
      sprintf(prox_sensor_name, "ps%d", ind);
      prox_sensors[ind] = wb_robot_get_device(prox_sensor_name);
      wb_distance_sensor_enable(prox_sensors[ind], TIME_STEP);
*/
    
  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
   
   double left_speed = MAX_SPEED;
   double right_speed = MAX_SPEED;


  while (wb_robot_step(TIME_STEP) != -1) {
    /*
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */
     //get color
     const unsigned char *image = wb_camera_get_image(camera);
     /*bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 80;
     bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 80;
     bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 80;
     */
    
      //counter for red blue and green pixel
      red = 0;
      green = 0;
      blue = 0;

      //loop to increment red blue and green count
    for (i = width / 3; i < 2 * width / 3; i++) {
        for (j = height / 2; j < 3 * height / 4; j++) {
          red += wb_camera_image_get_red(image, width, i, j);
          blue += wb_camera_image_get_blue(image, width, i, j);
          green += wb_camera_image_get_green(image, width, i, j);
        }
      }
      // if more color than other, blob is a color
       if ((red > 3 * green) && (red > 3 * blue))
        blob = RED;
      else if ((green > 3 * red) && (green > 3 * blue))
        blob = notRED;
      else if ((blue > 3 * red) && (blue > 3 * green))
        blob = notRED;
      else
        blob = NONE;

      
      if (blob == NONE) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
        
      }
      if (blob == notRED) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
        }
      
       if(blob == RED){
        left_speed = 0;
        right_speed = 0;
    /* Process sensor data here */
    }
    
    /*if (front_wall == true)
    {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
     }
     else
     {
       if(left_wall == true)
       {
         left_speed = MAX_SPEED;
         right_speed = MAX_SPEED;
       }
       
       else
       {
         left_speed = MAX_SPEED;
         right_speed = MAX_SPEED;
       }
       
       if(left_corner == true)
       {
         left_speed = MAX_SPEED;
         right_speed = MAX_SPEED/8;
       }
       
      }*/

    /*
     * Enter here functions to send actuator commands, like:
     * wb_motor_set_position(my_actuator, 10.0);
     */
     
     wb_motor_set_velocity(left_motor, left_speed);
     wb_motor_set_velocity(right_motor, right_speed);
  };

  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
