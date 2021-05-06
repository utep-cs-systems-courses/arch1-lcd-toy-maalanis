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



u_char LOWER_BOUNDARY, UPPER_BOUNDARY, LEFT_BOUNDARY, RIGHT_BOUNDARY;
u_char width, height;

  AbRectOutline fieldOutline ={
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2-10, screenHeight/3-10}
};
  
Layer layer2 = {
  (AbShape *)&arrow30,
  {screenWidth/2+40, screenHeight/2+10}, 	    /* position */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0,
};

Layer fieldLayer = {
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_BLACK,
  &layer2
};

typedef struct MovLayer_s{
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
  
}MovLayer;

MovLayer ml1 = {&fieldLayer, {1,1}, 0};
MovLayer ml2 = {&layer2, {2,0}, &ml1};
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

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      }	/**< if outside of fence */
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}


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
  
  for(;;){
    while(!redrawScreen){
      or_sr(0x10);
    }
    redrawScreen=0;
    movLayerDraw(&ml2, &layer2);
    
  }
  
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
