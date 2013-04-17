#include <panel.h>
#include <stdarg.h>

int display_error(char* err,...) {
	va_list ap;
	va_start(ap,err);
	vfprintf(stderr,err,ap);
}