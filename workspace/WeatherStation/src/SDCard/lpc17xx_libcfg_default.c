#include "lpc17xx_libcfg_default.h"


void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

#if 1
	//xprintf("!! check failed in file %s, line %ld", file, line);
#else
	/* mthomas: avoid unused waring: */
	(void) file;
	(void) line;

	/* Infinite loop */
	while (1)
		;
#endif
}
