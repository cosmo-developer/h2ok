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

	if (H2OKFAILED(RunHook(Context, [](H2OKContext* context) {
			
		}))) {
		return -3;
	}
	MSG msg;
	


	return 0;
}