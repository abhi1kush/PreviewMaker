//============================================================================
// Name        : video_preview_main.cpp
// Author      : Abhishek
// Version     :
// Copyright   : Your copyright notice
// Description : main file.
//============================================================================

#include <iostream>
#include "Box.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!";
	BoxHeader boxHeaderObj;
	FullBoxHeader fullHeaderObj;
	cout <<sizeof(unsigned char) <<" "<<sizeof(boxHeaderObj)<<" "<<sizeof(fullHeaderObj);
	return 0;
}
