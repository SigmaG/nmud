#include "windows.h"

struct input_struct {
	struct windows *W;
	int sock;
};

void* handle_keypresses(void* arg);