//============================================================================
// Name        : Chisa.cpp
// Author      : PSI
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

namespace chisa {
namespace entrypoint {
namespace pc {

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}

}}}


int main() {
	return chisa::entrypoint::pc::main();
}

