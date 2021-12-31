#define s0 8       //Module pins wiring
#define s1 9
#define s2 10
#define s3 11
#define out 12
//init
int in1=2,in2=4;
int pwm1=3;
int in3=7,in4=5;
int pwm2=6;
int c1=A0; // WIDE LEFT
int c2=A1;  //LEFT
int c3=A2; //RIGHT
int c4=A3; //WIDE RIGHT
//0 white
//1 black
int v1,v2,v3,v4;
int data=0;        //This is where we're going to stock our values
int start=0;
// Variables
int red = 0;
int green = 0;
int blue = 0;
int turn = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(s1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
  forward(255,255);

}

void loop() {
  data=0;
  Serial.println(data);
  color();
  detect();
  // put your main code here, to run repeatedly:
  v1=digitalRead(c1); // wide left
  v2=digitalRead(c2); //left
  v3=digitalRead(c3); //right
  v4=digitalRead(c4); //wide right
  //affichage moniteur serie
  Serial.print(v1);
  Serial.print(v2);
  Serial.print(v3);
  Serial.println(v4);
  
  if(data==1){ //detection of color red
    forward(0,0); //stop
    turn=1;
    delay(5000); //wait 5 seconds
    do{
      right(90,90); // flip to the right until touch v2==1 black
      v2=digitalRead(c2); //left
    }while(v2==0);
  }
  if(v1==0 && v2==0 && v3==1 && v4==0){ // slightly turn to the right
    right(60,60);
    forward(90,90);
  }
  else if(v1==0 && v2==1 && v3==0 && v4==0){ //slightly turn to the left
    left(60,60);
    forward(90,90);
  }
  else if(v1==0 && v2==0 && v3==0 && v4==0){ // if all are white move forward
    forward(80,80);
  }
  else if(v1==1 && v2==1 && v3==1 && v4==1){ //solution start
      if(start==0){ //first detection of all black
        do{
          forward(60,60);
          v1=digitalRead(c1); // wide left
          v4=digitalRead(c4); //wide right
        }while(v1==1 && v4==1);
      start=1;
      }
      else{  //second detection of all black
        do{
          right(80,80);
          v3=digitalRead(c3); // right
          v4=digitalRead(c4); //wide right
        }while(v3+v4>0);
      }
  }
  else if(v1==0 && v2==0 && v3==0 && v4==1 && turn==0){ // M solution first right part before color
    forward(150,150);
  }
  else if(v1==1 && v2==0 && v3==0 && v4==0 && turn==1){ // M solution first left after color
    do{
      left(90,90);
      v1=digitalRead(c1); //right
      v2=digitalRead(c2); //wide right
    }while(v2+v1>0);
  }
  else if(v1==0 && v2==0 && v3==0 && v4==1 && turn==1){ // M solution second  right part after color
    do{
      right(90,90);
      v3=digitalRead(c3); // wide left
      v4=digitalRead(c4); //left
    }while(v3+v4>0);
  }  
  else if(v1==0 && v2==0 && v3==1 && v4==1){ //solution L
    do{
      right(90,90);
      v3=digitalRead(c3); //right
      v4=digitalRead(c4); //wide right      
    }while(v3+v4>0);
  }  
  else if(v1==1 && v2==1 && v3==0 && v4==0){ //Solution L
    do{
      left(90,90);
      v1=digitalRead(c1); //right
      v2=digitalRead(c2); //wide right      
    }while(v1+v2>0);
  }  
}
void forward(int a, int b)
{
 digitalWrite(in1,1);
 digitalWrite(in2,0);
 analogWrite(pwm1,a);
 digitalWrite(in3,1);
 digitalWrite(in4,0);
 analogWrite(pwm2,b);

 }
 void left(int a, int b){
  
 digitalWrite(in1,0);
 digitalWrite(in2,1);
 analogWrite(pwm1,a);
 digitalWrite(in3,1);
 digitalWrite(in4,0);
 analogWrite(pwm2,b);
 }
 void right(int a, int b){
 digitalWrite(in1,1);
 digitalWrite(in2,0);
 analogWrite(pwm1,a);
 digitalWrite(in3,0);
 digitalWrite(in4,1);
 analogWrite(pwm2,b);

 }
void back(int a, int b)
{
 digitalWrite(in1,0);
 digitalWrite(in2,1);
 analogWrite(pwm1,a);
 digitalWrite(in3,0);
 digitalWrite(in4,1);
 analogWrite(pwm2,b);

 }
 
void color()
{
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
void detect()
{
  if (red < blue && red < green && red < 25)
  {
    if (green - blue >= 9 && green - blue <= 15 && green - ( 2 * red ) >= 2 )
    {
      Serial.println(" - (Red Color)");
      data=1;
    }
    else if (blue - red >= 10 && blue - red <= 17 &&  green - ( 2 * red ) <= 4)
    {

      Serial.println(" - (Pink Color)");
      data=1;
    }
  }

  else if (green < red && green < blue && green < 25)
  {
    data=3;
    Serial.println(" - (Green Color)");
  }

  else if ((red > green &&  blue < green) && blue < 25 && red > 40)
  {

    Serial.println(" - (Blue Color)");
  }

  else if (red - (2 * blue) >= -2 && red - (2 * blue) <= 5 && green - red < 10)
  {

    Serial.println(" - (Purple Color)");
  }
  else if (blue < red && blue < green && (blue && red && green) < 25)
  {

    if (red - green <= 5 && red - green >= 0 && ((green - blue) || (red - blue)) < 5 && blue < 50)
    {
  
      Serial.println(" - (White Color)");
    }
  }
}
