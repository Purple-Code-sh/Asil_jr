#include <Servo.h>

#define line_L A4
#define line_R A2
#define servo_B A5

int startPin = 10;

int side_L = 0;
int front_L = 1;
int front_R = 2;
int side_R = 4;

int motorL_pwm = 3;
int motorL_dir = 12;

int motorR_pwm = 11;
int motorR_dir = 13;

int v_Line;
int v_Front;
int v_Side;

int proportionalUsed;

Servo FLAG;

void setup()
{
  Serial.begin(9600);
  FLAG.attach(servo_B);

  pinMode(startPin, INPUT);
  pinMode(line_L, INPUT);
  pinMode(line_R, INPUT);
  pinMode(side_L, INPUT);
  pinMode(side_R, INPUT);
  pinMode(front_L, INPUT);
  pinMode(front_R, INPUT);

  pinMode(motorL_pwm, OUTPUT);
  pinMode(motorL_dir, OUTPUT);
  pinMode(motorR_pwm, OUTPUT);
  pinMode(motorR_dir, OUTPUT);

  v_Line = 0;
  v_Front = 0;
  v_Side = 0;

  proportionalUsed = 0;

  FLAG.write(90);
  delay(5000);

}

void loop()
{
start:
  if (digitalRead(startPin) == HIGH) {
    Serial.println("Encendido");
    FLAG.write(180);
    // ---------- Line ----------
    //v_Line = lineSensors();

    //  if (v_Line == 3)
    //  {
    //    Serial.println("Linea - ambos");
    //    goBack(70, 220, 350);
    //    goLeft(false, 255, 300, 1);
    //    goto start;
    //  }
    //  else if (v_Line == 1)
    //  {
    //    Serial.println("Linea - izquierda");
    //    goBack(70, 220, 350);
    //    goLeft(false, 255, 300, 1);
    //    goto start;
    //  }
    //  else if (v_Line == 2)
    //  {
    //    Serial.println("Linea - derecha");
    //    goBack(70, 220, 350);
    //    goRight(false, 255, 500, 1);
    //    goto start;
    //  }

    // ---------- Front ----------

    v_Front = frontSensors();
    if (v_Front == 3)
    {
      if (proportionalUsed == 0) {
        Serial.println("Enfrente - ambos");
        goForward_proportional(80);
        proportionalUsed = 1;
        goto start;
      } else if (proportionalUsed == 1) {
        goForward(255, 255, 30);
        goto start;
      }
    }
    else if (v_Front == 1)
    {
      Serial.println("Enfrente - izquierda");
      goLeft(false, 70, 20, 1);
      proportionalUsed = 0;
      goto start;
    }
    else if (v_Front == 2)
    {
      Serial.println("Enfrente - derecha");
      goRight(false, 70, 20, 1);
      proportionalUsed = 0;
      goto start;
    }

    // ---------- Side ----------

    v_Side = sideSensors();
    if (v_Side == 1)
    {
      Serial.println("Lado - izquierda");
      goFull_left();
      proportionalUsed = 0;
      goto start;
    }
    else if (v_Side == 2)
    {
      Serial.println("Lado - derecha");
      goFull_right();
      proportionalUsed = 0;
      goto start;
    }
    else
    {
      Serial.println("Ninguno");
      goLeft_proportional(10);
      delay(150);
      proportionalUsed = 0;
      goto start;
    }
  } else {
    Serial.println("Apagado");
    stopi(false);
    proportionalUsed = 0;
    FLAG.write(90);
  }



} // <<<---loop end


// ------------------------- Sensors Functions ------------------------- .

//int lineSensors()
//{
//  int lineL = digitalRead(line_L);
//  int lineR = !digitalRead(line_R) * 2;
//
//  int addition_Line = lineL + lineR;
//
//  return addition_Line;
//}

int lineSensors()
{
  int lineStatus;

  int valLeft = analogRead(line_L);
  int valRight = analogRead(line_R);

  if (valLeft > 600 && valRight > 600) {
    lineStatus = 3;
  } else if (valLeft > 600) {
    lineStatus = 1;
  } else if (valRight > 600) {
    lineStatus = 2;
  } else {
    lineStatus = 0;
  }

  return lineStatus;
}

int frontSensors()
{
  int frontL = digitalRead(front_L);
  int frontR = digitalRead(front_R) * 2;

  int addition_Front = frontL + frontR;

  return addition_Front;
}

int sideSensors()
{
  int sideL = digitalRead(side_L);
  int sideR = digitalRead(side_R) * 2;

  int addition_Side = sideL + sideR;

  return addition_Side;
}

// ------------------------- Stop Functions ------------------------- .

void stopi(bool reverse)
{
  if (reverse == false)
  {
    digitalWrite(motorL_dir, HIGH);
    digitalWrite(motorL_pwm, LOW);

    digitalWrite(motorR_dir, HIGH);
    digitalWrite(motorR_pwm, LOW);
  } else if (reverse == true)
  {
    digitalWrite(motorL_dir, LOW);
    digitalWrite(motorL_pwm, LOW);

    digitalWrite(motorR_dir, LOW);
    digitalWrite(motorR_pwm, LOW);
  }
}

// ------------------------- Motors Functions ------------------------- .

void goForward(int powerL_f, int powerR_f, int workTime_f)
{
  digitalWrite(motorL_dir, LOW);
  analogWrite(motorL_pwm, powerL_f);

  digitalWrite(motorR_dir, LOW);
  analogWrite(motorR_pwm, powerR_f);

  delay(workTime_f);
  stopi(false);
}

void goBack(int powerL_b, int powerR_b, int workTime_b)
{
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, powerL_b);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, powerR_b);

  delay(workTime_b);
  stopi(true);
}

void goRight(bool curve_r, int power_r, int workTime_r, int relation_r)
{
  if (curve_r == true)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, round(power_r / relation_r));

    delay(workTime_r);
    stopi(false);
  }
  else if (curve_r == false)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, HIGH);
    analogWrite(motorR_pwm, power_r);

    delay(workTime_r);
    stopi(false);
  }
}

void goLeft(bool curve_l, int power_l, int workTime_l, int relation_l)
{
  if (curve_l == true)
  {
    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, round(power_l / relation_l));

    delay(workTime_l);
    stopi(false);
  }
  else if (curve_l == false)
  {
    digitalWrite(motorL_dir, HIGH);
    analogWrite(motorL_pwm, power_l);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    delay(workTime_l);
    stopi(false);
  }
}

void goFull_left() {
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, 255);

  digitalWrite(motorR_dir, LOW);
  analogWrite(motorR_pwm, 255);
}

void goFull_right() {
  digitalWrite(motorL_dir, LOW);
  analogWrite(motorL_pwm, 255);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, 255);
}

// ------------------------- Proportional Functions ------------------------- .

void goForward_proportional(int workTime_fp) {
  for (int i = 5; i < 255; i = i + 50) {
    if (i < 200) {
      goForward(i, i, round(workTime_fp / 4));
    } else {
      goForward(i, i, workTime_fp);
    }
  }
}

void goBack_proportional(int workTime_bp) {
  for (int i = 5; i < 255; i = i + 50) {
    if (i < 100) {
      goBack(i, i, round(workTime_bp / 2));
    } else {
      goBack(i, i, workTime_bp);
    }
  }
}

void goRight_proportional(int workTime_rp) {
  for (int i = 5; i < 255; i = i + 50) {
    if (i < 100) {
      goRight(true, i, round(workTime_rp / 2), 3);
    } else {
      goRight(true, i, workTime_rp, 3);
    }
  }
}

void goLeft_proportional(int workTime_lp) {

  for (int i = 5; i < 255; i = i + 50) {
    if (i < 100) {
      goLeft(true, i, round(workTime_lp / 2), 3);
    } else {
      goLeft(true, i, workTime_lp, 3);
    }
  }
}


// ------------------------- Test Functions ------------------------- .

void motorsTest1(int workTimes, int pause_t)
{
  //NO TOCAR ALV !!!!!!!!!!

  //Valores ya revisados que si funcionan
  //Tiempos medidos para el tamaño del dojo
  goBack(70, 220, 350);
  delay(2000);
  goForward(255, 255, 80);
  delay(2000);
  goRight(false, 255, 500, 1);
  delay(2000);
  goLeft(false, 255, 300, 1);
}

void motorsTest2()
{
  Serial.println("Test: giro total Derecha e Izquierda");

  Serial.println("giro derecha");
  goRight(false, 255, 600, 1);
  delay(2000);
  Serial.println("giro izquierda");
  goLeft(false, 255, 400, 1);
}

void motorsTest3()
{
  Serial.println("Test: curva a la Derecha e Izquierda");

  Serial.println("curva a la izquierda");
  goLeft_proportional(50);
  delay(1000);
  Serial.println("curva a la derecha");
  goRight_proportional(60);
}

void motorsTest4() {
  Serial.println("Test: adelante y atras");

  Serial.println("Adelante");
  goForward_proportional(80);
  delay(2000);
  Serial.println("Atras");
  goBack_proportional(30);
}
