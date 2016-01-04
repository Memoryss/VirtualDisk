#include "CommandAnalysis.h"
#include "CommandElement.h"

#include <string>
#include <fstream>
#include <gtest/gtest.h>
#include <tuple>

using namespace std;

class CommandAnalysisUnit : public ::testing::TestWithParam<std::tr1::tuple<char, char, char, char, char, char> >
{
public:
	virtual void SetUp(){}
	virtual void TearDown(){}
	
	void loadAndAnalysisFile(const char* fileName);
protected:
	CommandAnalysis cmdAnalysis;
	vector<string> srcStrVec;
	vector<vector<string> > desStrVecVec;
};

void CommandAnalysisUnit::loadAndAnalysisFile(const char* fileName)
{
	srcStrVec.clear();
	desStrVecVec.clear();

	ifstream testFile(fileName);
	if (!testFile.is_open())
	{
		return;
	}

	while (!testFile.eof())
	{
		string temp;
		getline(testFile,temp);
		string srcStr;
		vector<string> desStrVec;

		string::size_type position = temp.find('[');
		if (temp.npos != position)
		{
			position++;
			while (position < temp.size() && ']' != temp[position])
			{
				srcStr.push_back(temp[position]);
				position++;
			}

			position++;
		}

		int j = 0;
		while(position < temp.size())
		{
			string tempStr;
			while (position < temp.size() && ' ' != temp[position])
			{
				if ('\"' == temp[position])//双引号
				{
					++position;
					while (position < temp.size() && '\"' != temp[position])
					{
						tempStr.push_back(temp[position]);
						++position;
					}
					++position;
				}
				else
				{
					tempStr.push_back(temp[position]);
					position++;
				}
			}

			if (0 != tempStr.size())
			{
				desStrVec.push_back(tempStr);
			}
			position++;
		}

		srcStrVec.push_back(srcStr);
		desStrVecVec.push_back(desStrVec);
	}
}

TEST_F(CommandAnalysisUnit, SplitCmdTest)
{
	loadAndAnalysisFile("d:\\cmdunittest.txt");
	ASSERT_TRUE(srcStrVec.size() != 0) << "找不到测试文件";

	for (int i = 0; i < srcStrVec.size(); ++i)
	{
		cmdAnalysis.splitCmd(srcStrVec[i]);
		std::vector<CommandElement>& cmdElement = cmdAnalysis.getCmdElementVec();

		for (int j = 0; j < cmdElement.size(); ++j)
		{
			ASSERT_STREQ(cmdElement[j].getElementStr().c_str(), desStrVecVec[i][j].c_str()) << i << j;
		}
	}
}

TEST_F(CommandAnalysisUnit, SplitPathTest)
{
	loadAndAnalysisFile("d:\\pathunittest.txt");
	ASSERT_TRUE(srcStrVec.size() != 0) << "找不到测试文件";

	for (int i = 0; i < srcStrVec.size(); ++i)
	{
		cmdAnalysis.splitPath(srcStrVec[i], nullptr, nullptr);
		vector<string> fileNameVec = cmdAnalysis.getFileNameVec();

		for (int j = 0; j < fileNameVec.size(); ++j)
		{
			ASSERT_STREQ(fileNameVec[j].c_str(), desStrVecVec[i][j].c_str()) << i << j;
		}
	}
}

TEST_F(CommandAnalysisUnit, ParaTest)
{
	loadAndAnalysisFile("d:\\paraunittest.txt");
	ASSERT_TRUE(srcStrVec.size() != 0) << "找不到测试文件";

	for (int i = 0; i < srcStrVec.size(); ++i)
	{
		cmdAnalysis.splitCmd(srcStrVec[i]);
		vector<string> paraVec = cmdAnalysis.paraAnalysis();
		
		int num = atoi(desStrVecVec[i][0].c_str());
		ASSERT_EQ(paraVec.size(), num) << i;
	}
}

/*
TEST_F(CommandAnalysisUnit, CmdTypeTest)
{
	char ch1 = std::tr1::get<0>(GetParam());
	char ch2 = std::tr1::get<1>(GetParam());
	char ch3 = std::tr1::get<2>(GetParam());
	char ch4 = std::tr1::get<3>(GetParam());
	char ch5 = std::tr1::get<4>(GetParam());
	char ch6 = std::tr1::get<5>(GetParam());
	std::string str;
	str.push_back(ch1);
	str.push_back(ch2);
	str.push_back(ch3);
	str.push_back(ch4);
	str.push_back(ch5);
	str.push_back(ch6);
	ASSERT_EQ(-1, cmdAnalysis.cmdTypeAnalysis(str)) << str;
}

INSTANTIATE_TEST_CASE_P(CmdString, CommandAnalysisUnit, testing::Combine(testing::Values('*', ' ', 'd', 'c', 'm', 'r', 's', 'l', 'b'), testing::Values('*', ' ', 'i', 'd', 'e', 'o', 'k', 'a', 'b'), testing::Values('*', ' ', 'r', 'n', 'p', 'l', 'a', 'v', 'b'), testing::Values('*', ' ', 'y', 'i', 'd', 'e', 'b'), testing::Values('*', ' ', 'n', 'b'), testing::Values('*', ' ', 'k', 'b')));
*/