

#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
#include "stateMachines.h"
#include "shape.h"

char switch_state_down, switch_state_changed; /* effectively boolean */
char blink_mode = 0;
char box_color1=0;
char color=1;
char power = 0;
//extern char power;
static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;

  /* update switch interrupt to detect changes from current buttons */
      
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update();
  //  buzzer_set_period(1000);
}

void
switch_interrupt_handler()
{

  
  //  buzzer_set_period(1000);


  char p2val = switch_update_interrupt_sense();

  switch_state_down = (p2val & SW1 ) ? 0 : do_button1(p2val);
  switch_state_down = (p2val & SW2 ) ? 0 : do_button2(p2val);
  switch_state_down = (p2val & SW3 ) ? 0 : do_button3(p2val);
  //  switch_state_down = (p2val & SW4) ? 0 : do_button4(p2val);
  
  
  /*
  if(SW1){ 
    do_button1(p2val);
  } else if(SW2){
    do_button2(p2val);
  }
  */
  //switch_state_down = (p2val & SW1) ? 0 : 1; /* 0 when SW1 is up */
 
  // switch_state_changed = 1;
  //color ^=1;
  //led_update();
  
}

char 
do_button1(char p2val)
{
  if(power){
    power = 0;
    //or_sr(0x10);
    and_sr(~8);// turn off 
    color = 0;
    P1OUT &= (0xff - LEDS); // clear bits for off leds
    P1OUT |= LED_RED;
    led_update();
   
  } else {
    P1OUT &= (0xff - LEDS);
    P1OUT |= LED_GREEN;
    power =1;
    //and_sr(~8);
    or_sr(8);    //turn on
    
  }
    enableWDTInterrupts();
  if(!box_color1){
    //  buzzer_set_period(0);
    box_color1 = 1;
  }
  else if(box_color1){
    //    buzzer_set_period(3000);
    box_color1 = 0;
  }
  //switch_state_down = (p2val & SW1) ? 0 : 1;
  switch_state_changed = 1;
  color ^=1;  
  led_update();
  //  enableWDTInterrupts();
  return 1;
}
char
do_button2(char p2val)
{
  drawStar();
  return 1;
}
char power_sound=0;
char
do_button3(char p2val)
{
  if(power_sound){
    power_sound= 0;
  }
  else{
    power_sound=1;
  }
  return 1;
}
char blink=0;
char 
do_button4(char p2val)
{
  if(blink){
    if(blink_mode){
      P1OUT = LED_GREEN;
      blink_mode=0;
    }
    if(!blink_mode){
      P1OUT = 0;
      blink_mode=1;
    }
      
    blink = 0;
  } else{
    P1OUT = LED_GREEN;
  }
  
  return 1;
}
 
