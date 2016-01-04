//#include "vld.h"
#include <iostream>
#include "VirtualDisk.h"
#include <string> 
#include <tchar.h>

#include <gtest/gtest.h>
//#include "CommandAnalysis_unittest.cpp"

using namespace std;

int main(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	/*
	string str;
	VirtualDisk vd;

	while(1)
	{
		cout << vd.getCurPath() << ">";
		getline(cin, str);

		if (!vd.executeCmd(str))
		{
			break;
		}

		str.clear();
	}
	return 0;*/

	return RUN_ALL_TESTS();
}