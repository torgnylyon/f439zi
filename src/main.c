#include "main.h"
#include "clock.h"

int glov1 = 3;
int glov2;

static int stav1 = 25;
static int stav2;

int
main(void)
{
	init_clock();
	while (1) {
		stav1++;
		stav2++;
		glov1++;
		glov2++;
	}
}
