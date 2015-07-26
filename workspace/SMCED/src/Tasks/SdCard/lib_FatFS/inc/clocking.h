
/*
 * Can't get clocking from the RTC to work with debugger.
 * make sure that this is not compiled ...
 */
#undef LPC_CLOCK_FROM_RTC

#define LPC_INTRC_OSC 0
#define LPC_MAIN_OSC  1
#ifdef LPC_CLOCK_FROM_RTC
	#define LPC_RTC_OSC   2
#endif

/*
 * Wahhh... this should really be defined only in FreeRTOSConfig.h !!!
 * but since we possibly are not using FreeRTOS in this app it is redefined here.
 *
 * This could need a proper fix ...
 */
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ 1000
#endif

/*
 * Clocking interface:
 *
 * LPC_UseOscillator(osc) - select which oscillator to use
 *
 * 		Parameter: osc = LPC_MAIN_OSC - use crystal oscillator
 * 		                 LPC_INTRC_OSC - use internal RC oscillator
 *
 *		Use this function to change from one to the other oscillator
 *		or to disable the PLL and revert back to the base frequency.
 *		When switching back to the INTRC oscillator, the main oscillator
 *		is switched off when it is not being used by the USB PLL(1).
 *
 * LPC_SetPLL0(cclk) - program PLL0 to use a different CPU frequency
 *
 * 		Parameter: cclk = the CPU frequency in Hz
 * 		                  Only use defined values from the table in clocking.c
 *		Return   :    1 - cclk is set to the given frequency
 *		              0 - cclk is not changed
 *
 * LPC_EnablePLL1() - Enable PLL1 generating the 48 MHz USB clock
 *
 * LPC_DisablePLL1() - Disable PLL1 and switch off the 48 MHz USB clock
 *                     will also switch of the main crystal oscillator when
 *                     it is not being used to generate CCLK (CPU clock)
 *
 * PLEASE NOTE
 * -----------
 *
 * Both LPC_UseOscillator() and LPC_SetPLL0() reprogram the systick to generate
 * a 1 ms. interrupt. This is currently not configurable.
 * When frequently changing frequencies, you might notice a drift in time due
 * to the fact that the systick is restarted.
 */
//int LPC_UseOscillator(uint8_t osc);
//int LPC_SetPLL0(uint32_t cclk);
//int LPC_EnablePLL1(void);
//int LPC_DisablePLL1(void);
