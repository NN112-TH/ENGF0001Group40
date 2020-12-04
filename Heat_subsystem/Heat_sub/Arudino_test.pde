import processing.serial.*;

Serial myPort;
String temp;
float temp_c;
int linefeed = 10;
PShape I_G40; 
PShape I_pH;
PShape I_Stirring;
PShape I_Temp;
PShape I_Dashboard;
 
//define variable
color headerl, headerr, bg;

void setup() {
  size(1440, 720);
  smooth();
  //define color
  bg = 0;
  headerl = color(71,82,243);
  headerr = color(150,155,219);
  //import project icon
  I_G40 = loadShape("G40.svg");
  I_pH = loadShape("pH.svg");
  I_Stirring = loadShape("rotate.svg");
  I_Temp = loadShape("thermometer.svg");
  I_Dashboard = loadShape("network_monitor.svg");
  //simavr virtual serial
  myPort = new Serial(this, "/tmp/simavr-uart0",9600);
  myPort.bufferUntil('\n');
}

void draw() {
  //background
  background(255);
  design();
  temp = myPort.readStringUntil('\n');
    if (temp != null) {
    println(temp);
    temp_c = float(temp-270);
    temp = ""+(nf((temp_c),0,2))+"°"+"C";
    fill(51);
    textSize(30);
    text(temp, 220, 280);
    }

}

//keep overall design of non-technical related of the UI here
void design() {
  //header
  setGradient(0, 0, 1440, 79, headerl, headerr,2);
  fill(243);
  textSize(16);
  String bioreactor = "BIOREACTOR";
  text(bioreactor, 60,45);
  shape(I_G40, 19,10,45,56);
  //body
  shape(I_Dashboard, 48,96,70,70);
  fill(51);
  textSize(36);
  text("DASHBOARD", 160,145);
  //box1
  stroke(248);
  fill(248);
  rect(48,200,319,245,8);
  shape(I_Temp, 68, 264,100,100);
  textSize(15);
  fill(41);
  text("TEMPERATURE", 68, 409);
  textSize(18);
  fill(95);
  text("Measured", 230, 231);
  text("Controlled", 230, 318);
  //box2
  stroke(248);
  fill(248);
  rect(544,200,319,245,8);
  shape(I_pH, 564, 264,100,100);
  textSize(15);
  fill(41);
  text("pH", 601, 409);
  textSize(18);
  fill(95);
  text("Measured", 726, 231);
  text("Controlled", 726, 318);
  //box3
  stroke(248);
  fill(248);
  rect(1030,200,319,245,8);
  shape(I_Stirring, 1050, 264,100,100);
  textSize(15);
  fill(41);
  text("Stirring", 1070, 409);
  textSize(18);
  fill(95);
  text("Measured", 1212, 231);
  text("Controlled", 1212, 318);
}


//set gradient function from processing.org
void setGradient(int x, int y, float w, float h, color c1, color c2, int axis ) {

  noFill();

  if (axis == 1) {  // Top to bottom gradient
    for (int i = y; i <= y+h; i++) {
      float inter = map(i, y, y+h, 0, 1);
      color c = lerpColor(c1, c2, inter);
      stroke(c);
      line(x, i, x+w, i);
    }
  }  
  else if (axis == 2) {  // Left to right gradient
    for (int i = x; i <= x+w; i++) {
      float inter = map(i, x, x+w, 0, 1);
      color c = lerpColor(c1, c2, inter);
      stroke(c);
      line(i, y, i, y+h);
    }
  }
}

class Button {
  String label;
  float x;    // top left corner x position
  float y;    // top left corner y position
  float w;    // width of button
  float h;    // height of button
  
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
  }
  
  void Draw() {
    fill(218);
    stroke(141);
    rect(x, y, w, h, 10);
    textAlign(CENTER, CENTER);
    fill(0);
    text(label, x + (w / 2), y + (h / 2));
  }
  
  boolean Click() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
}
