import processing.serial.*;

Serial port;

PrintWriter file;

boolean flag_receive_data = true;

final int BUF_SIZE = 500;
int buf_index = 0;

float[] desired_dphi_deg_s_1 = new float[BUF_SIZE];
int[] desired_enc_count_1 = new int[BUF_SIZE];
int[] enc_count_1 = new int[BUF_SIZE];

float print_desired_dphi_deg_s_1;
int print_desired_enc_count_1;
int print_enc_count_1;

void setup() {
  size(500, 500);
  println(Serial.list());
  port = new Serial(this, "/dev/cu.usbserial-A403616Z", 9600);
  
  for (int i = 0; i < BUF_SIZE; i++) {
    desired_dphi_deg_s_1[i] = 0.0;
    desired_enc_count_1[i] = 0;
    enc_count_1[i] = 0;
  }
  
  file = createWriter("result.csv");
}

void draw() {
  background(255);
  drawAxis();
  drawLegend();

  for (int i = 0; i < BUF_SIZE-1; i++) {
    stroke(0, 0, 255);
    line(i, height/2 - map(desired_enc_count_1[i], 0, 30, 0, height/2), i+1, height/2 - map(desired_enc_count_1[i+1], 0, 30, 0, height/2));
    stroke(255, 0, 0);
    line(i, height/2 - map(enc_count_1[i], 0, 30, 0, height/2), i+1, height/2 - map(enc_count_1[i+1], 0, 30, 0, height/2));
  }
}

void drawAxis() {
  stroke(0);
  line(0, height/2, width, height/2);
}

void drawLegend() {
  int start_x = 50;
  int end_x = start_x + 50;
  stroke(0, 0, 255);
  line(start_x, 40, end_x, 40);
  stroke(255, 0, 0);
  line(start_x, 60, end_x, 60);
  
  textSize(14);
  textAlign(LEFT);
  fill(#000000);
  text("desired_dphi_deg_s_1: ", end_x+10, 23);
  text("desired_enc_count_1: ", end_x+10, 43);
  text("enc_count_1: ", end_x+10, 63);

  text(print_desired_dphi_deg_s_1, end_x+170, 23);
  text(print_desired_enc_count_1, end_x+170, 43);
  text(print_enc_count_1, end_x+170, 63);
}

void serialEvent(Serial p) {
  String res = p.readStringUntil('\n');
  if (res == null || !flag_receive_data) return;
  res = trim(res);
  String outputs[] = split(res, ' ');
  if (outputs.length == 3) {
    desired_dphi_deg_s_1[buf_index] = float(outputs[0]);
    desired_enc_count_1[buf_index] = int(outputs[1]);
    enc_count_1[buf_index] = int(outputs[2]);
    print_desired_dphi_deg_s_1 = desired_dphi_deg_s_1[buf_index];
    print_desired_enc_count_1 = desired_enc_count_1[buf_index];
    print_enc_count_1 = enc_count_1[buf_index];
    //print(desired_dphi_deg_s_1[buf_index], " ");
    //print(desired_enc_count_1[buf_index], " ");
    //println(enc_count_1[buf_index]);
    buf_index = (buf_index + 1) % BUF_SIZE;
  }
}

void saveCSV() {
  for (int i = 0; i < BUF_SIZE; i++) {
    file.print(desired_dphi_deg_s_1[i]);
    file.print(",");
    file.print(desired_enc_count_1[i]);
    file.print(",");
    file.println(enc_count_1[i]);
  }
  file.flush();
  file.close();
  println("saved the data to result.csv.");
}

void keyPressed() {
  switch (key) {
    case ' ':
      flag_receive_data = !flag_receive_data;
      break;
    case 'g':
      saveCSV();
      break;
    default:
      port.write(key);
  }
}
