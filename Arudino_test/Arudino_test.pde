import processing.serial.*;

Serial myPort;
String temp, rpm, pH;
float temp_c;
float Ctrtemp_f = 25.00;
String Ctrtemp;
float Ctrrpm_f = 800;
String Ctrrpm;
float CtrpH_f = 5;
String CtrpH;
int linefeed = 10;
PShape I_G40; 
PShape I_pH;
PShape I_Stirring;
PShape I_Temp;
PShape I_Dashboard;
Button tempInc, tempDec, pHInc, pHDec, stirInc, stirDec;
ButtonM tempMa, tempMi, pHMa, pHMi, stirMa, stirMi;
String arduino, msg;
 
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
  
  //button
  
  //simavr virtual serial
  myPort = new Serial(this, "/tmp/simavr-uart0",9600);
  myPort.bufferUntil('\n');
}

void draw() {
  //background
  background(255);
  design();
  String arduino = myPort.readStringUntil('\n');
    if (arduino != null && arduino != "0") {
    String[] msg = split(arduino, ",");  
    temp = msg[0];
    pH = msg[1];
    rpm = msg[2];
    float rpm_i = int(rpm);
    rpm = ""+ nf((rpm_i),0,0)+" RPM";
    float pH_f = int(pH);
    pH = "" + nf((pH_f), 0 ,2);
    temp_c = float(temp)-270;
    temp = ""+(nf((temp_c),0,2))+"°"+"C";
    fill(51);
    textSize(30);
    text(temp, 230, 280);
    fill(51);
    textSize(30);
    text(rpm, 1212, 280);
    fill(51);
    textSize(30);
    text(pH, 726, 280);
    }
}

void mouseClicked() {
  //tempevent
  if(tempInc.Click())
  {
    if (Ctrtemp_f < 35) {
      Ctrtemp_f = Ctrtemp_f + 0.5;
      myPort.write('q');
    }    
  }
  if(tempDec.Click())
  {
    if(Ctrtemp_f > 25) {
      Ctrtemp_f = Ctrtemp_f - 0.5;
      myPort.write('w');
    }
  }
  if(tempMa.Click())
  {
      Ctrtemp_f = 35; 
      myPort.write('e');
  }
  if(tempMi.Click())
  {
      Ctrtemp_f = 25;
      myPort.write('r');
  }
  //pH event
  if(pHInc.Click())
  {
    if (CtrpH_f < 7) {
      CtrpH_f = CtrpH_f + 0.5;
      myPort.write('a');
    }    
  }
  if(pHDec.Click())
  {
    if(CtrpH_f > 3) {
      CtrpH_f = CtrpH_f - 0.5;
      myPort.write('s');
    }
  }
  if(pHMa.Click())
  {
      CtrpH_f = 7.00; 
      myPort.write('d');
  }
  if(pHMi.Click())
  {
      CtrpH_f = 3.00; 
      myPort.write('f');
  }
  //stirring event
  if(stirInc.Click())
  {
    if (Ctrrpm_f < 1500) {
      Ctrrpm_f = Ctrrpm_f + 100;
      myPort.write('z');
    }    
  }
  if(stirDec.Click())
  {
    if(Ctrrpm_f > 500) {
      Ctrrpm_f = Ctrrpm_f - 100;
      myPort.write('x');
    }
  }
  if(stirMa.Click())
  {
      Ctrrpm_f = 1500;
      myPort.write('c');
  }
  if(stirMi.Click())
  {
      Ctrrpm_f = 500;
      myPort.write('w');
  }
}

//keep overall design of non-technical related of the UI here
void design() {
  //header
  setGradient(0, 0, 1440, 79, headerl, headerr,2);
  fill(243);
  textSize(20);
  String bioreactor = "BIOREACTOR";
  text(bioreactor, 160,35);
  shape(I_G40, 42,10,56,56);
  //body
  shape(I_Dashboard, 48,96,70,70);
  fill(51);
  textSize(36);
  text("DASHBOARD", 255,130);
  //box1
  temBox(48,200);
  //box2
  pHBox(544, 200);
  //box3
  stirBox(1030, 200); 
}

void temBox(int x, int y) {
  Box(x, y, I_Temp);
  textSize(13);
  fill(41);
  text("TEMPERATURE", x +60, y+189);
  tempInc = new Button("", x+262, y +143, 25, 25);
  tempInc.Draw();
  text("+", x+275, y +152);
  tempDec = new Button("", x+262, y +175, 25, 25);
  tempDec.Draw();
  text("-", x+275, y +184);
  fill(51);
  textSize(30);
  Ctrtemp = ""+(nf((Ctrtemp_f),0,2))+"°"+"C";
  text(Ctrtemp, x +182, y +167);
  tempMa = new ButtonM("", x +120, y +191,38,20);
  tempMa.Draw();
  tempMi = new ButtonM("", x +200, y +191,38,20);
  tempMi.Draw();
  fill(#af1607);
  textSize(16);
  text("MAX", x +140, y +200);
  text("MIN", x +220, y +200);
  //connect to arduino
  
}

void pHBox(int x, int y) {
  Box(x, y, I_pH);
  textSize(15);
  fill(41);
  text("pH", x +68, y+184);
  fill(51);
  textSize(30);
  CtrpH = ""+(nf((CtrpH_f),0,2));
  text(CtrpH, x +182, y +167);
  pHMa = new ButtonM("", x +120, y +191,38,20);
  pHMa.Draw();
  pHMi = new ButtonM("", x +200, y +191,38,20);
  pHMi.Draw();
  pHInc = new Button("", x+262, y +143, 25, 25);
  pHInc.Draw();
  textSize(13);
  fill(41);
  text("+", x+275, y +152);
  pHDec = new Button("", x+262, y +175, 25, 25);
  pHDec.Draw();
  text("-", x+275, y +184);
  fill(#af1607);
  textSize(16);
  text("MAX", x +140, y +200);
  text("MIN", x +220, y +200);
}

void stirBox(int x, int y) {
  Box(x, y, I_Stirring);
  textSize(15);
  fill(41);
  text("Stirring", x +67, y+189);
  stirInc = new Button("", x+262, y +143, 25, 25);
  stirInc.Draw();
  text("+", x+275, y +152);
  stirDec = new Button("", x+262, y +175, 25, 25);
  stirDec.Draw();
  text("-", x+275, y +184);
  fill(51);
  textSize(30);
  Ctrrpm = ""+(nf((Ctrrpm_f),0,0))+" RPM";
  text(Ctrrpm, x +182, y +167);
  stirMa = new ButtonM("", x +120, y +191,38,20);
  stirMa.Draw();
  stirMi = new ButtonM("", x +200, y +191,38,20);
  stirMi.Draw();
  fill(#af1607);
  textSize(16);
  text("MAX", x +140, y +200);
  text("MIN", x +220, y +200);
}

void Box(int x, int y, PShape a ) {
  stroke(248);
  fill(248);
  rect(x,y,319,245,8);
  shape(a, x+ 20, y+64,100,100);
  textSize(18);
  fill(95);
  text("Measured", x+182, y+31);
  text("Controlled", x+182, y+128);
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
    fill(230);
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      fill(218);
    }
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

class ButtonM {
  String label;
  float x;    // top left corner x position
  float y;    // top left corner y position
  float w;    // width of button
  float h;    // height of button
  
  ButtonM(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
  }
  
  void Draw() {
    fill(248);
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      fill(230);
    }
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
