#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

char getchar() {
    char c;
#if defined(__is_libk)
	c = terminal_getchar();
#else
	// TODO: Implement stdio and the write system call.
#endif
	return c;
}