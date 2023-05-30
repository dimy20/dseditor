#pragma once
#include <iostream>

#define PANIC(v, s) do{                   \
	if(v){                                  \
		std::cerr << "Error: " << s << "\n";  \
		exit(1);                              \
	}                                       \
}while(0);
