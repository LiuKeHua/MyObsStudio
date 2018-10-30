#include "callback/signal.h"

#include <iostream>


int main()
{
	auto itemDeselect = [](void *data, calldata_t *cd)
	{
		int * p = (int*)data;
		
		int curItem = (int)calldata_int(cd, "source");

		std::cout << *p << " , "<<curItem<<std::endl;
	};

	signal_handler_t* MySignal = signal_handler_create();

	signal_handler_add(MySignal, "void start(ptr output)");

	int num = 44;
	signal_handler_connect(MySignal, "start", itemDeselect, &num);

	struct calldata params = { 0 };
	calldata_init(&params);
	calldata_set_int(&params, "source", -66);

	signal_handler_signal(MySignal, "start", &params);

	return 0;
}