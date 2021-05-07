#include <msp430.h>
#include "stateMachines.h"
#include "buzzer.h"
#include "led.h"
#include "switches.h"

//char power = 1;

char sound_on()		/* always toggle! */
{
  static char sound = 0;

  switch (sound) {

  case 0:

    buzzer_set_period(1000);
    sound = 1;
    break;
  case 1:
    buzzer_set_period(2000);
    sound = 2;
    break;
  case 2:
    buzzer_set_period(2500);
    sound = 3;
    break;
  case 3:
    buzzer_set_period(3000);
    sound = 0;
    break;
    
    
  }

  return 1;

  /*
  static char state = 0;

  switch (state) {
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }

  return 1;			/* always changes an led */
 
}


char sound_off()	/* only toggle green if red is on!  */
{
  
  /*
  char changed = 0;
  if (red_on) {
    green_on ^= 1;
    changed = 1;
  }
  return changed;
 */
  buzzer_set_period(0);
  return 1;
}



void state_advance()		/* alternate between toggling red & green */

{
  sound_on();
}



void state_advance_flip()
{
  buzzer_set_period(1000);
  static char led = 0;
  if(led) {
    green_on = 1;
    red_on = 0;
    color = 1;
    led = 0;
  } else {
    green_on = 0;
    red_on = 1;
    led = 1;
    color = 0;
  }
  switch_state_changed = 1;
  led_changed = 1;
  led_update();

}
