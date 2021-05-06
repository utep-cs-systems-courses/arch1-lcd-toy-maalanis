#include "msp430.h"
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"
//#include "p2switches.h"
#include "buzzer.h"
#include "switches.h"
#include "led.h"
#include "led.c"
#include "stateMachines.h"

AbRect rect10 = {abRectGetBounds, abRectCheck, 10,10};
AbRArrow arrow30 = {abRArrowGetBounds, abRArrowCheck, 30};

#define GREEN_LED BIT6;

Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}};


Layer layer2 = {
  (AbShape *)&arrow30,
  {screenWidth/2+40, screenHeight/2+10}, 	    /* position */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};
/*
Layer layer1 = {
  (AbShape *)&rect10,
  {screenWidth/2, screenHeight/2}, 	    /* position */
/* {0,0}, {0,0},				    /* last & next pos */
	  /* COLOR_RED,
  &layer2,
};
Layer layer0 = {
  (AbShape *)&rect10,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /* position */
/* {0,0}, {0,0},				    /* last & next pos */
	  /* COLOR_ORANGE,
  &layer1,
};

*/
u_int bgColor = COLOR_BLUE;
int redrawScreen= 1;
static int box_color = 0;
static char blink_state=0;
int
main()
{

  P1DIR |= GREEN_LED;
  P1OUT |= GREEN_LED;
  
  configureClocks();
  lcd_init();
  shapeInit();
  buzzer_init();
  switch_init();
  Vec2 rectPos = screenCenter, circlePos = {30,screenHeight - 30};

  clearScreen(COLOR_BLUE);
  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);
  shapeInit();
  
  layerInit(&layer2);
  layerDraw(&layer2);
  //  enableWDTInterrupts();
  or_sr(0x8);
  drawString5x7(20,20, "HELLO", COLOR_GREEN, COLOR_RED);
  /*
  for(;;){
    while(!redrawScreen){
      or_sr(0x10);
    }
    redrawScreen=1;
    
    
  }
  */
  /*
  while(redrawScreen){
  if(color){
    // buzzer_set_period(1000);
    layer2.color = COLOR_RED;
    redrawScreen=0;
    // layerDraw(&layer0);
  }
  if(!color){
    //buzzer_set_period(2000);
    layer2.color = COLOR_WHITE;
    redrawScreen=0;
    //layerDraw(&layer0);
  }
  
  // clearScreen(COLOR_BLUE);
  // layerDraw(&layer0);
  }

  //  drawRectOutline(&layer0->pos[0], &layer0->pos[1], &layer0->shape->width, &layer0->shape->height, COLOR_BLACK);
  */
}


void wdt_c_handler()
{
  
  //    buzzer_set_period(2000);
  static short count = 0;
  //   P3OUT |= GREEN_LED;
  count++;
  redrawScreen=1;
  if(count == 75){
    //    buzzer_set_period(2000);
    if(power){
    //    led_flags ^=1;
      if(box_color){
	layer2.color= COLOR_RED;
	box_color = 0;
	layerDraw(&layer2);
      }
      else if(!box_color){
	layer2.color=COLOR_WHITE;
	box_color = 1;
	layerDraw(&layer2);
      }
    } else {
     
    }
    
    if(power_sound){
      sound_on();
    }
    if(!power_sound){
      sound_off();
    }
    if(blink){
      if(blink_state){
	led_update();
	blink_state= 0;
      }
      if(!blink_state){
       
	blink_state=1;
      }
      
    }
    if(!blink){
      //      P1OUT = LED_GREEN;
    }
 
    count = 0;
    redrawScreen=1;
  }
      }
