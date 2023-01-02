/*
    Arduino and ADXL345 Accelerometer - 3D Visualization Example 
     by Dejan, https://howtomechatronics.com
*/

import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;

String data="";
float roll, pitch, yaw;

void setup() {
  size (960, 640, P3D);
  myPort = new Serial(this, "/dev/cu.usbmodem141301", 9600); // starts the serial communication
  myPort.bufferUntil('\n');
}

// void draw() {
//   translate(width/2, height/2, 0);
//   background(33);
//   textSize(22);
//   text("Roll: " + int(roll) + "     Pitch: " + int(pitch), -100, 265);

//   // Rotate the object
  // rotateX(radians(roll));
  // rotateZ(radians(-pitch));
  // rotateY(radians(yaw));

//   // 3D 0bject
//   textSize(30);  
//   fill(0, 76, 153);
//   box (386, 40, 200); // Draw box
//   textSize(25);
//   fill(255, 255, 255);
//   text("one", -183, 10, 101);

  // textSize(30);  
  // fill(0, 76, 153);
  // box (386, 40, 200); // Draw box
  // textSize(25);
  // fill(255, 255, 255);
  // text("two", -183, 10, 101);


//   //delay(10);
//   //println("ypr:\t" + angleX + "\t" + angleY); // Print the values to check whether we are getting proper values

// }

void draw() {
  background(255);

  translate(width / 2, height / 2);
  
  // seconds

  // degrees divided by number of second marks per round
  float radSec = 360 / 60 * second();
  pushMatrix();
  rotateZ(radians(radSec));
  textSize(30);  
  fill(0, 76, 153);
  box (386, 40, 200); // Draw box
  textSize(25);
  fill(255, 255, 255);
  text("two", -183, 10, 101);
  popMatrix();
  
  
  // hours
  // strokeWeight(4);
  // float radHour = 360 / 12 * hour();
  // pushMatrix();
  // rotate(radians(radHour));
  // line(0, 0, 0, -50);
  // popMatrix();
}

// Read data from the Serial Port
void serialEvent (Serial myPort) { 
  // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
  data = myPort.readStringUntil('\n');

  // if you got any bytes other than the linefeed:
  if (data != null) {
    data = trim(data);
    // split the string at "/"
    String items[] = split(data, '/');
    if (items.length > 1) {

      //--- Roll,Pitch in degrees
      roll = float(items[0]);
      pitch = float(items[1]);
      yaw = float(items[2]);
    }
  }
}
