#include "CommandFactory.h"

#include "Command.h"
#include "DirCommand.h"
#include "CdCommand.h"
#include "MdCommand.h"
#include "DelCommand.h"
#include "RdCommand.h"
#include "MkCommand.h"
#include "MoveCommand.h"
#include "CopyCommand.h"
#include "RenCommand.h"
#include "SaveCommand.h"
#include "LoadCommand.h"
#include "ExitCommand.h"
#include "CommandAnalysis.h"

using namespace std;

CommandFactory::CommandFactory()
{
	_cmdType = -1;
	_cmdAnalysis = new CommandAnalysis;
}

CommandFactory::~CommandFactory()
{
	delete _cmdAnalysis;
}

Command* CommandFactory::createCmd(string cmdStr)
{
	Command* cmd = nullptr;

	_cmdType = _cmdAnalysis->cmdTypeAnalysis(cmdStr);

	switch (_cmdType)
	{
	case 0:
		cmd = new DirCommand(_cmdAnalysis);
		break;
	case 1:
		cmd = new CdCommand(_cmdAnalysis);
		break;
	case 2:
		cmd = new MdCommand(_cmdAnalysis);
		break;
	case 3:
		cmd = new CopyCommand(_cmdAnalysis);
		break;
	case 4:
		cmd = new DelCommand(_cmdAnalysis);
		break;
	case 5: 
		cmd = new RdCommand(_cmdAnalysis);
		break;
	case 6:
		cmd = new RenCommand(_cmdAnalysis);
		break;
	case 7:
		cmd = new MoveCommand(_cmdAnalysis);
		break;
	case 8:
		cmd = new LoadCommand(_cmdAnalysis);
		break;
	case 9:
		cmd = new MkCommand(_cmdAnalysis);
		break;
	case 10:
		cmd = new SaveCommand(_cmdAnalysis);
		break;
	case 11:
		cmd = new ExitCommand(_cmdAnalysis);
		break;
	default:
		cmd = nullptr;
		break;
	}

	return cmd;
}
