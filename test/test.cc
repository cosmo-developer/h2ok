#include<iostream>
#include "h2ok.hpp"
#include <Windows.h>


int main(const int argc, const char** argv) {

	H2OKContext* Context = nullptr;

	if (H2OKFAILED(CreateContext(&Context))) {
		std::cerr << "Failed to Create H2OKContext" << std::endl;
		return -1;
	}

	if (H2OKNULL(Context)) {
		std::cerr << "Context is Null" << std::endl;
		return -2;
	}

	H2OKRunHookCallbackRegistrar* registrar=nullptr;

	if (H2OKFAILED(GetDefaultH2OKRunHookCallbackRegistrar(&registrar))) {
		return -3;
	}

	if (H2OKNULL(registrar)) {
		return -4;
	}

	if (H2OKFAILED(RegisterH2OKRunHookCallback(registrar, [](PH2OKContext context)-> void{
		std::cout << "Hello World!" << std::endl;
		H2OKRunHookCallbackRegistrar* registrar = nullptr;

		if (H2OKFAILED(GetDefaultH2OKRunHookCallbackRegistrar(&registrar))) {
			PostQuitMessage(0);
		}

		DestroyContext(registrar);
	}))) {
		return -7;
	}
	if (H2OKFAILED(RegisterH2OKRunHookCallback(registrar, [](PH2OKContext context)-> void {}))) {
		return -7;
	}
	if (H2OKFAILED(RegisterH2OKRunHookCallback(registrar, [](PH2OKContext context)-> void {}))) {
		return -7;
	}



	H2OKRunHookCallback callback1 = nullptr;
	H2OKRunHookCallback callback2 = nullptr;
	H2OKRunHookCallback callback3 = nullptr;


	if (H2OKFAILED(GetH2OKRunHookCallback(registrar, 1, &callback1)) || H2OKFAILED(GetH2OKRunHookCallback(registrar, 1, &callback2)) || H2OKFAILED(GetH2OKRunHookCallback(registrar, 1, &callback3))) {
		return -5;
	}

	if (!H2OKSAME(callback1, callback2) || !H2OKSAME(callback1, callback3) || !H2OKSAME(callback2, callback3)) {
		return -9;
	}

	if (H2OKFAILED(SetCurrentContext(registrar, Context))) {
		return -10;
	}

	RunHook(Context);

	return 0;
}