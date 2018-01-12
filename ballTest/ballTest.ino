#include <Charliplexing.h>
#include <avr/pgmspace.h>

int8_t x, y, dx, dy;
int8_t sh1y, sh2y, s1, s2;


/* ---------------------------------------------------------------------------*/
/* Arduino setup func
*/
void setup() {
  LedSign::Init(DOUBLE_BUFFER);

  x = 3;
  y = 7;
  dx = 1;
  dy = 1;

  randomSeed(analogRead(0));

}

void loop() {
  int8_t randommove;
  // The Ball shall bounce on the walls :
  if (x == 12 || x == 1) dx = -dx;
  if (y == 8 || y == 0) dy = -dy;


  // Clear the non-active screen
  LedSign::Clear();

  // Move the BALL :
  x = x + dx;
  y = y + dy;

  // Draw the ball :
  LedSign::Set(x, y, 1);
  LedSign::Flip();

  // Display the bitmap some times
  delay(200);

  // loop :)

}
