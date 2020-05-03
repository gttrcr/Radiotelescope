#include "Define.h"
#include "Utils.h"

int raEncoderPos = 0;
int raEncoderLast = LOW;
int decEncoderPos = 0;
int decEncoderLast = LOW;

bool track = false;

struct Coordinates
{
  float ra;
  float dec;

  inline float AbsDistance(Coordinates mP)
  {
    return sqrt(pow(mP.ra - ra, 2) + pow(mP.dec - dec, 2));
  }
} currentPosition, newPosition;

void setup()
{  
  Serial.begin(BAUD);
  Serial.setTimeout(50);

  pinMode(RA_MOTOR_PIN1, OUTPUT);
  pinMode(RA_MOTOR_PIN2, OUTPUT);
  pinMode(DEC_MOTOR_PIN1, OUTPUT);
  pinMode(DEC_MOTOR_PIN2, OUTPUT);

  pinMode(13, OUTPUT);
  analogWrite(13, OUTPUT);

  currentPosition.ra = RA_PARK_POSITION;
  currentPosition.dec = DEC_PARK_POSITION;
  newPosition.ra = RA_PARK_POSITION;
  newPosition.dec = DEC_PARK_POSITION;
}

bool acceso = false;
void loop()
{
  if (Serial.available())
  {
    Serial.println(Serial.readString());
    Serial.println(acceso?"acceso":"spento");
    if(acceso)
    {
      digitalWrite(13, LOW);
      acceso = false;
    }
    else
    {
      digitalWrite(13, HIGH);
      acceso = true;
    }
  }
    //ProcessCommand(Serial.readString());

  /*
  if(currentPosition.AbsDistance(newPosition) > MARGIN_FOR_SLEWING)
    Slew();
  else
    Stop();

  if(track)
    Track();

  UpdatePosition();
  */
}

void ProcessCommand(String message)
{
  char buf[COMPLETE_MESSAGE_LENGTH];
  message.toCharArray(buf, COMPLETE_MESSAGE_LENGTH);
  char *p = buf;
  char *str;
  String messageArray[NUMBER_COMMANDS_IN_MESSAGE];
  int i = 0;
  while ((str = strtok_r(p, ";", &p)) != NULL)
    messageArray[i++] = str;
  
  String command = messageArray[0].substring(0, STRING_COMMAND_LENGTH);
  if(command == RA_COMMAND)
    newPosition.ra = value(messageArray);

  command = messageArray[1].substring(0, STRING_COMMAND_LENGTH);
  if(command == DEC_COMMAND)
    newPosition.dec = value(messageArray + 1);

  command = messageArray[2].substring(0, STRING_COMMAND_LENGTH);
  if(command == TRACK_COMMAND)
    track = (value(messageArray + 2) == 1);
  else if(command == PARK_COMMAND)
  {
    newPosition.ra = RA_PARK_POSITION;
    newPosition.dec = DEC_PARK_POSITION;
  }

  Serial.print("RA: ");
  Serial.print(newPosition.ra);
  Serial.print(" - DEC: ");
  Serial.println(newPosition.dec);
}

void Slew()
{
  //Serial.println("Slew");
  if(newPosition.ra > currentPosition.ra)
  {
    digitalWrite(RA_MOTOR_PIN1, HIGH);
    digitalWrite(RA_MOTOR_PIN2, LOW);
  }
  else if(newPosition.ra < currentPosition.ra)
  {
    digitalWrite(RA_MOTOR_PIN1, LOW);
    digitalWrite(RA_MOTOR_PIN2, HIGH);    
  }

  if(newPosition.dec > currentPosition.dec)
  {
    digitalWrite(DEC_MOTOR_PIN1, HIGH);
    digitalWrite(DEC_MOTOR_PIN2, LOW);
  }
  else if(newPosition.dec < currentPosition.dec)
  {
    digitalWrite(DEC_MOTOR_PIN1, LOW);
    digitalWrite(DEC_MOTOR_PIN2, HIGH);   
  }
}

void Stop()
{
  digitalWrite(RA_MOTOR_PIN1, LOW);
  digitalWrite(RA_MOTOR_PIN2, LOW);
  digitalWrite(DEC_MOTOR_PIN1, LOW);
  digitalWrite(DEC_MOTOR_PIN2, LOW);  
}

void Track()
{
  Serial.println("track");
}

void UpdatePosition()
{
  int n = digitalRead(RA_ENCODER_PIN_A);
  if ((raEncoderLast == LOW) && (n == HIGH)) {
    if (digitalRead(RA_ENCODER_PIN_B) == LOW)
      raEncoderPos--;
    else
      raEncoderPos++;

    raEncoderPos %= RA_INPUT_PER_TURN_RATIO;
    currentPosition.ra = raEncoderPos * RA_DEGREE_EACH_INPUT;
    //Serial.println(currentPosition.ra, 8);
  }
  raEncoderLast = n;

  n = digitalRead(DEC_ENCODER_PIN_A);
  if ((decEncoderLast == LOW) && (n == HIGH)) {
    if (digitalRead(DEC_ENCODER_PIN_B) == LOW)
      decEncoderPos--;
    else
      decEncoderPos++;

    decEncoderPos %= DEC_INPUT_PER_TURN_RATIO;
    currentPosition.dec = decEncoderPos * DEC_DEGREE_EACH_INPUT;
    //Serial.println(currentPosition.dec, 8);
  }
  decEncoderLast = n;
}
