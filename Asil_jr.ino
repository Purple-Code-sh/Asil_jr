#define line_L A4
#define line_R A2

int startPin = 10;

int frontSensor = 1;
int side_L = 0;
int side_R = 4;

int motorL_pwm = 3;
int motorL_dir = 12;

int motorR_pwm = 11;
int motorR_dir = 13;

int proportionalUsed;

void setup()
{
  Serial.begin(9600);

  pinMode(startPin, INPUT);
  pinMode(line_L, INPUT);
  pinMode(line_R, INPUT);
  pinMode(side_L, INPUT);
  pinMode(side_R, INPUT);
  pinMode(frontSensor, INPUT);

  pinMode(motorL_pwm, OUTPUT);
  pinMode(motorL_dir, OUTPUT);
  pinMode(motorR_pwm, OUTPUT);
  pinMode(motorR_dir, OUTPUT);

  proportionalUsed = 0;
}

void loop()
{
start:
  if (digitalRead(startPin) == HIGH) {
    Serial.println("Encendido");
    // ---------- Line ----------
    //v_Line = lineSensors();

    //  if (v_Line == 3)
    //  {
    //    Serial.println("Linea - ambos");
    //    proportionalUsed = 0;
    //    goBack(70, 220, 350);
    //    goto start;
    //  }
    //  else if (v_Line == 1)
    //  {
    //    Serial.println("Linea - izquierda");
    //    proportionalUsed = 0;
    //    goBack(70, 220, 350);
    //    goto start;
    //  }
    //  else if (v_Line == 2)
    //  {
    //    Serial.println("Linea - derecha");
    //    proportionalUsed = 0;
    //    goBack(70, 220, 350);
    //    goRight(false, 255, 500, 1);
    //    goto start;
    //  }

    // ---------- Front ----------

    if (digitalRead(frontSensor) == 1)
    {
      Serial.println("Enfrente");
      if (proportionalUsed == 0) {
        goForward_proportional(80);
        proportionalUsed = 1;
        goto start;
      } else if (proportionalUsed == 1) {
        goForward(255, 255, 40);
        goto start;
      }
    }

    // ---------- Side ----------

    if (digitalRead(side_L) == 1)
    {
      Serial.println("Lado - izquierda");
      proportionalUsed = 0;
      goFull_left();
      goto start;
    }
    else if (digitalRead(side_R) == 1)
    {
      Serial.println("Lado - derecha");
      proportionalUsed = 0;
      goFull_right();
      goto start;
    }
    else
    {
      Serial.println("Ninguno");
      proportionalUsed = 0;
      goForward(60, 60, 40);
      delay(200);21
      goto start;
    }
    
  } else {
    Serial.println("Robot Apagado");
    proportionalUsed = 0;
    stopi(false);
  }

} // <<<---loop end


// ------------------------- Sensors Functions ------------------------- .

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

// ------------------------- Test Functions ------------------------- .

void motorsTest1()
{
  delay(3000);

  goBack(70, 220, 350);
  delay(2000);

  goForward(255, 255, 80);
  delay(2000);

  goFull_left();
  delay(300);
  stopi(false);
  delay(2000);

  goFull_right();
  delay(300);
  stopi(false);
  delay(2000);
}
