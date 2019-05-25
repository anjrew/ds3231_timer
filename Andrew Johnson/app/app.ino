
#include <Wire.h>
#include "ds3231.h"

#define BUFF_MAX 128


// TIME CONSTANTS
int SECOND = 1000;
int MINUTE = 60 * SECOND;
int HOUR = 60 * MINUTE;


const int actionPeriod = 15; //minutes
const int humidifierDuration = 2; //Minutes
const int waitingPeriod = 15; // Seconds
const int ventingDuration = 15; //SECOND;





uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1 * SECOND;

void parse_cmd(char *cmd, int cmdsize);

struct ts t;

void setup()
{

  // Time setup
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  memset(recv, 0, BUFF_MAX);
  Serial.println("GET time");

  //             TssmmhhWDDMMYYYY aka set time

  //      parse_cmd("T003320120082018" , 16);

}

void logTime() {

    Serial.print(" - LCD show time & date - ");
    Serial.print("Date: ");
    Serial.print(t.mday);
    Serial.print("-");
    Serial.print(t.mon);
    Serial.print("-");
    Serial.print(t.year);
    Serial.print("Time: ");

    if (t.hour < 10) {
        Serial.print("0");
        Serial.print(t.hour);
    } else {
        Serial.print(t.hour);
    }

    Serial.print(":");
    if (t.min < 10) {
        Serial.print("0");
        Serial.print(t.min);
    } else {
        Serial.print(t.min);
    }

    Serial.print(":");

    if (t.sec < 10) {
        Serial.print("0");
        Serial.print(t.sec);
    } else {
        Serial.print(t.sec);
    }
}

void loop()
{
  char in;
  char buff[BUFF_MAX];
  unsigned long now = millis();


  // show time once in a while
  if ((now - prev > interval) && (Serial.available() <= 0)) {
    DS3231_get(&t);

    // there is a compile time option in the library to include unixtime support
#ifdef CONFIG_UNIXTIME
#ifdef __AVR__
    snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year)
#error AVR
#else
    snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %d", t.year),
#endif
             t.mon, t.mday, t.hour, t.min, t.sec, t.unixtime);
#else
    snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
#endif

    Serial.println(buff);
    t.hour t.min


  if (Serial.available() > 0) {
    in = Serial.read();

    if ((in == 10 || in == 13) && (recv_size > 0)) {
      parse_cmd(recv, recv_size);
      recv_size = 0;
      recv[0] = 0;
    } else if (in < 48 || in > 122) {
      ;       // ignore ~[0-9A-Za-z]
    } else if (recv_size > BUFF_MAX - 2) {   // drop lines that are too long
      // drop
      recv_size = 0;
      recv[0] = 0;
    } else if (recv_size < BUFF_MAX - 2) {
      recv[recv_size] = in;
      recv[recv_size + 1] = 0;
      recv_size += 1;
    }

  }
}
