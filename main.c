#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdio.h>
#include <string.h>

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#define CIRCLE_X 15
#define CIRCLE_Y_1 45
#define CIRCLE_Y_2 100
#define CIRCLE_RADIUS 12
#define X_INCREMENT 8

// Initialization function for graphics
void initializeGraphics(Graphics_Context* g_sContext_p) {
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCmss12);

    // Clear the screen
    Graphics_clearDisplay(g_sContext_p);
}

// Function to restart timer
void restartTimer() {
    Timer32_setCount(TIMER32_0_BASE, 3000000); // 1 second
    Timer32_startTimer(TIMER32_0_BASE, true);
}

// Function to check if timer is expired
bool isTimerExpired() {
    return (Timer32_getValue(TIMER32_0_BASE) == 0);
}

int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initializations
    Graphics_Context g_sContext; // Create context
    initializeGraphics(&g_sContext);

    // Draw initial graphics
    Graphics_drawString(&g_sContext, (int8_t*) "Draw, then Erase", -1, 20, 10, true);
    Graphics_drawString(&g_sContext, (int8_t*) "Erase, then Draw", -1, 20, 65, true);
    Graphics_fillCircle(&g_sContext, CIRCLE_X, CIRCLE_Y_1, CIRCLE_RADIUS);
    Graphics_fillCircle(&g_sContext, CIRCLE_X, CIRCLE_Y_2, CIRCLE_RADIUS);

    // Initialize and start a one-shot timer
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    restartTimer();

    // Start demo
    int32_t loop = 0;
    while(1) {
        if(isTimerExpired() && (loop < 10)) {
            // Draw then erase
            Graphics_fillCircle(&g_sContext, CIRCLE_X + (loop+1)*X_INCREMENT, CIRCLE_Y_1, CIRCLE_RADIUS);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(&g_sContext, CIRCLE_X + loop*X_INCREMENT, CIRCLE_Y_1, CIRCLE_RADIUS);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

            // Erase then draw
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(&g_sContext, CIRCLE_X + loop*X_INCREMENT, CIRCLE_Y_2, CIRCLE_RADIUS);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
            Graphics_fillCircle(&g_sContext, CIRCLE_X + (loop+1)*X_INCREMENT, CIRCLE_Y_2, CIRCLE_RADIUS);

            loop++;
            restartTimer();
        }
    }
}
