#include <gtest/gtest.h>

#include "CommandAnalysis.h"
#include "CdCommand.h"

class CdCommandUnit : public ::testing::Test
{
public:
	virtual void SetUp() 
	{
		cmdAnalysis = new CommandAnalysis;
		cdCmd = new CdCommand(cmdAnalysis);
	}
	virtual void TearDown() 
	{
		delete cdCmd;
		delete cmdAnalysis;
	}

protected:
	CommandAnalysis* cmdAnalysis;
	CdCommand* cdCmd;
};

TEST_F(CdCommandUnit, CdCmdTest)
{

}