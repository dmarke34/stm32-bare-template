#include <stdint.h>

uint32_t flag;            /* imagine an ISR sets this to 1 */
volatile uint32_t vflag;  /* same, but volatile */

void wait_plain(void)    { while (!flag)  { } }
void wait_volatile(void) { while (!vflag) { } }