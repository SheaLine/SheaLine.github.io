// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
//#include <string.h>


//CSE13E Support Library
#include "BOARD.h"
#include "Leds.h"
#include "Oled.h"
#include "Buttons.h"
#include "Adc.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Constants
#define TRUE 1
#define FALSE 0
#define MINUTE 60
#define LONG_PRESS 5
#define SIZE_1 27
#define SIZE_2_3 21
#define SIZE_4 14
#define SIZE_OVEN 83
#define SIZE_TEMP 4
#define SIZE_MODE 6
#define SIZE_TIME 5
#define LED_AMOUNT 8
#define DEFAULT_TEMP 350
#define SECOND 5
#define DEGREES 300



// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define OvenTop_OFF OVEN_TOP_OFF,OVEN_TOP_OFF,OVEN_TOP_OFF,OVEN_TOP_OFF,OVEN_TOP_OFF
#define OvenTop_ON OVEN_TOP_ON,OVEN_TOP_ON,OVEN_TOP_ON,OVEN_TOP_ON,OVEN_TOP_ON
#define OvenBottom_OFF OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF
#define OvenBottom_ON OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON
#define TIME_RANGE (((AdcRead() >> 2 )& 0xFF) + 1)
#define TEMP_RANGE (((AdcRead() >> 2) & 0xFF) + DEGREES)


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
} OvenState;

typedef enum {
    BAKE, TOAST, BROIL
}OvenMode;

typedef enum {
    TIME, TEMP
}SettingsSelector;

typedef struct {
    OvenState state; //which state the toaster is in
    //add more members to this struct
    OvenMode mode; // whether we are baking toasting or broiling
    SettingsSelector selector; // whether we are changing the time or the temp when baking
    int16_t cooking_time_left; // how much time is left while cooking, in seconds
    int16_t initial_cook_time; // the cook time that is set before cooking, in seconds
    int16_t temp; // the temperature in degrees F
} OvenData;

typedef struct free_running_timer {
    int16_t freetime;
    int16_t Stored_Ftime;
}Timer;

// **** Declare any datatypes here ****
void getSecToMin(int seconds);
static OvenData Oven;
static Timer Ftimer; 
// **** Define any module-level, global, or external variables here **** 
static char cooktime[SIZE_TIME] = "0:01";
//static int16_t Stored_Ftime;
static int hz_counter = 0;
static int shift_counter = 1;
static char BitMask = 0x01;
static int normal = 0;
static int count = 0;

//Events
static uint8_t Event_ADC;
static uint8_t TIMER_TICK;
static uint8_t Event_BUTT_PRESS; 
static uint8_t Event_BUTT3_DOWN;
static uint8_t Event_BUTT3_UP;
static uint8_t Event_BUTT4_DOWN;
static uint8_t Event_BUTT4_UP;
// **** Put any helper functions here ****
void getSecToMin(int TotalSeconds)
{
    int16_t seconds = TotalSeconds % MINUTE;
    int16_t minutes = TotalSeconds / MINUTE;
    
    if (seconds > 9)
        sprintf(cooktime, "%u:%u", minutes, seconds); //puts the time into cooktime string
    else 
        sprintf(cooktime, "%u:0%u", minutes, seconds); //adds in the filler 0 when seconds single digits
}

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData){
    //update OLED here
    char OvenLine_1[SIZE_1];
    char OvenLine_2[SIZE_2_3];
    char OvenLine_3[SIZE_2_3];
    char OvenLine_4[SIZE_4];
    char Oven_str[SIZE_OVEN];
    char temp_str[SIZE_TEMP];
    char mode[SIZE_MODE];
    
    //Mode is put into string
    if(ovenData.mode == BAKE)
        sprintf(mode, "BAKE");
    else if(ovenData.mode == TOAST)
        sprintf(mode, "TOAST");
    else if(ovenData.mode == BROIL)
        sprintf(mode, "BROIL");
    
    //temp is put into string
    sprintf(temp_str, "%u", Oven.temp);
    
    
    switch (ovenData.mode)
    {
        case (BAKE):
            //shows oven is turned on oven when cooking
            if (ovenData.state == COOKING)
            {
                sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_ON, mode);
                sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
                sprintf(OvenLine_3, "|-----|  TEMP: %s%sF\n", temp_str, DEGREE_SYMBOL);
                sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_ON);
            }
            
            else
            {
                //shows oven off when not cooking and the appropriate ">" for each selector
               if(ovenData.selector == TIME)
                {
                    sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_OFF, mode);
                    sprintf(OvenLine_2, "|     | >Time: %s\n", cooktime);
                    sprintf(OvenLine_3, "|-----|  TEMP: %s%sF\n", temp_str, DEGREE_SYMBOL);
                    sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_OFF);
                }
                if (ovenData.selector == TEMP)
                {
                    sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_OFF, mode);
                    sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
                    sprintf(OvenLine_3, "|-----| >TEMP: %s%sF\n", temp_str, DEGREE_SYMBOL);
                    sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_OFF);
                } 
            }
            break;
        case (TOAST):
            //shows oven is turned on oven when cooking and doesn't show temp 
           if (ovenData.state == COOKING)
           {
               sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_OFF, mode);
               sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
               sprintf(OvenLine_3, "|-----|\n");
               sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_ON);
           }
           //shows Oven off when not cooking and no temp
           else
           {
               sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_OFF, mode);
               sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
               sprintf(OvenLine_3, "|-----|\n");
               sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_OFF);
           }
           break;
        case (BROIL):
            //shows oven on when cooking and shows temp at 500
            if(ovenData.state == COOKING)
            {
                sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_ON, mode);
                sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
                sprintf(OvenLine_3, "|-----|  TEMP: 500%sF\n", DEGREE_SYMBOL);
                sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_OFF);
            }
            //shows oven off when cooking and shows temp at 500
            else
            {
                sprintf(OvenLine_1, "|%s%s%s%s%s| MODE: %s\n",OvenTop_OFF, mode);
                sprintf(OvenLine_2, "|     |  Time: %s\n", cooktime);
                sprintf(OvenLine_3, "|-----|  TEMP: 500%sF\n", DEGREE_SYMBOL);
                sprintf(OvenLine_4, "|%s%s%s%s%s|\n", OvenBottom_OFF);
            }
    }
    sprintf(Oven_str, "%s%s%s%s", OvenLine_1, OvenLine_2, OvenLine_3, OvenLine_4); //combines the lines of the display to one string 
    OledClear(OLED_COLOR_BLACK); //clears to get rid of lingering chars
    OledDrawString(Oven_str);
    OledUpdate();

}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    //write your SM logic here.
    while(TRUE)
    {
        switch(Oven.state)
        {
            case (SETUP):
                if (Event_ADC)
                {
                    if (Oven.selector == TIME)
                    {
                        Oven.initial_cook_time = TIME_RANGE; //makes the cooktime range from 1 - 256 seconds
                        Oven.cooking_time_left = Oven.initial_cook_time; //sets the time left to the same as the set init time
                        getSecToMin(Oven.initial_cook_time);  //displays initial time
                    }
                        
                    if (Oven.selector == TEMP)
                        Oven.temp = TEMP_RANGE; //makes temp range from 300 - 555 degrees
                    updateOvenOLED(Oven);                   
                }
                if(Event_BUTT3_DOWN)
                {
                    Oven.state = SELECTOR_CHANGE_PENDING; 
                    Ftimer.Stored_Ftime = Ftimer.freetime; //stores freetime
                    //printf("buttonDOWN\n");
                    return;
                }
                
                if(Event_BUTT4_DOWN)
                {
                    Oven.state = COOKING;
                    Ftimer.Stored_Ftime = Ftimer.freetime;
                    LEDS_SET(0xFF); //turns on all LEDS
                    updateOvenOLED(Oven);
                    return;
                }
                
            case (SELECTOR_CHANGE_PENDING):
                if(Event_BUTT3_UP)
                {
                    //printf("buttonUP\n");
                    //printf("%d", Ftimer.freetime);
                    //printf("Stored: %d", Ftimer.Stored_Ftime);
                    if (Ftimer.freetime - Ftimer.Stored_Ftime < LONG_PRESS) //will run if not a long press
                    {
                        //printf("ShortPress\n");
                        
                        //toggles through the modes
                        switch (Oven.mode)
                        {
                            case (BAKE):
                                Oven.mode = TOAST;
                                break;                                
                            case (TOAST):
                                Oven.mode = BROIL;
                                break;
                            case (BROIL):
                                Oven.mode = BAKE;
                                break;            
                        }                                                                                            
                    }
                    else //will run if long press
                    {
                        //switches the selector
                        switch (Oven.selector)
                        {
                            case (TIME):
                                Oven.selector = TEMP;
                                break;
                            case (TEMP):
                                Oven.selector = TIME;
                                break;                                    
                        }
                    }
                }
                updateOvenOLED(Oven);
                Oven.state = SETUP;
                return;
                //Long press does not get detected consistently  if a break statement is put here
                
            case (COOKING):
                if (TIMER_TICK)
                {
                    Oven.cooking_time_left = Oven.cooking_time_left - 1; // decrements time left
                    int16_t interval = Oven.initial_cook_time/LED_AMOUNT; // finds interval to turn light off
                    if (Oven.cooking_time_left % interval == 0) // every time an interval has passed turn off a light
                    {
                        LEDS_SET(LEDS_GET() ^ BitMask);
                        BitMask = BitMask << 1;
                    }
                    if (Oven.cooking_time_left > 0)
                    {
                        getSecToMin(Oven.cooking_time_left);       //if cooking not done display time left                  
                    }
                    //when cooking done
                    else
                    {
                        Oven.state = ALERT; //back to setup
                        Oven.cooking_time_left = Oven.initial_cook_time; //reset time left
                        getSecToMin(Oven.initial_cook_time); // display initial time
                        BitMask = 0x01;    //reset bit mask                    
                    }
                    updateOvenOLED(Oven);
                    return;
                }
                if (Event_BUTT4_DOWN)
                {
                    Ftimer.Stored_Ftime = Ftimer.freetime; //store freetime
                    Oven.state = RESET_PENDING;
                    return;
                }
                
            case(RESET_PENDING):
                if (TIMER_TICK)
                    if (Ftimer.freetime - Ftimer.Stored_Ftime >= LONG_PRESS) //runs if long press on BTN 4
                    {
                        //resets all settings
                        //printf("long press");
                        Oven.state = SETUP;
                        Oven.cooking_time_left = Oven.initial_cook_time;
                        getSecToMin(Oven.initial_cook_time);
                        updateOvenOLED(Oven);
                        BitMask = 0x01;
                        LEDS_SET(0x00);
                        //return;
                    }
                //if accidental BTN4 press
                    else
                    {
                        Oven.state = COOKING;
                        return;
                        
                    }
                break;
               
            case (ALERT):
                
                    if (TIMER_TICK)
                    {
                        
                        if (normal == 0)
                            normal = 1;
                        else 
                            normal = 0;
                        if (normal == 1)
                            OledSetDisplayInverted();
                        
                        if (normal == 0)
                            OledSetDisplayNormal();
                        count++;
                        if (count == 8)
                        {
                            Oven.state = SETUP;
                            count = 0;
                        }
                        
                        
                    }
                    return;
                 
               // Oven.state = SETUP;
                
        }
                
    }
}


int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to sline's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here
    OledInit();
    ButtonsInit();
    AdcInit();
    LEDS_INIT();
    Oven.state = SETUP;
    Oven.temp = DEFAULT_TEMP;
    Oven.selector = TIME;
    updateOvenOLED(Oven);
    
   

    while (1){
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
        if (Event_ADC)
        {
            runOvenSM();
            Event_ADC = FALSE;
        }
        
        if (Event_BUTT3_DOWN)
        {
            runOvenSM();
            Event_BUTT3_DOWN = FALSE;
        }
        
        if (Event_BUTT3_UP)
        {
            runOvenSM();
            Event_BUTT3_UP = FALSE;
        }
        
        if (Event_BUTT4_DOWN)
        {
            runOvenSM();
            Event_BUTT4_DOWN = FALSE;
        }
        
        if (Event_BUTT4_UP)
        {
            runOvenSM();
            Event_BUTT4_UP = FALSE;
        }
        
        if (TIMER_TICK)
            runOvenSM();
            TIMER_TICK = FALSE;
        
                    
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    //add event-checking code here
    Ftimer.freetime = Ftimer.freetime + 1;
    
    hz_counter++;
    if (hz_counter == SECOND) // occurs every second
    {
        //printf("TICK\n");
        TIMER_TICK = TRUE;
        hz_counter = 0;
    }
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    

    //add event-checking code here
   
    
    Event_BUTT_PRESS = ButtonsCheckEvents();
    
    if (Oven.state != COOKING) //makes it so you cant switch modes while cooking or change the ADC
    {
         if (AdcChanged())
            Event_ADC = TRUE;
         
        if (Event_BUTT_PRESS & BUTTON_EVENT_3DOWN)
            Event_BUTT3_DOWN = TRUE;
    
        if (Event_BUTT_PRESS & BUTTON_EVENT_3UP)
            Event_BUTT3_UP = TRUE;        
    }
    
    if (Event_BUTT_PRESS & BUTTON_EVENT_4DOWN)
        Event_BUTT4_DOWN = TRUE;
    
    if (Event_BUTT_PRESS & BUTTON_EVENT_4UP)
        Event_BUTT4_UP = TRUE;   
    
}