/*
 * SWO.c
 *
 *  Created on: Aug 18, 2021
 *      Author: Oleg Volkov
 */

#include <swo.h>

/*--------------------------SWO--------------------------------*/
int _write(int file, char *ptr, int len) {
	for (int DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/*--------------------------SWO--------------------------------*/
