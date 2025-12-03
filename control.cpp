#include <iostream>
#include <conio.h>

#include "control.h"
bool ifchangedo(char actual, char expected, float& var, float amount) {
	if (actual == expected) {
		var = var + amount;
		return true;
	}
	else {
		return false;
	}
}