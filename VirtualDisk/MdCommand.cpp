#include "MdCommand.h"
#include "Folder.h"
#include "CommandAnalysis.h"

#include "windows.h"

using namespace std;

MdCommand::MdCommand(CommandAnalysis* cmdAnalysis):
Command(cmdAnalysis)
{

}

MdCommand::~MdCommand()
{

}

void MdCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElementVec = _cmdAnalysis->getCmdElementVec();
	for (int i = 0; i < cmdElementVec.size(); ++i)
	{
		Folder* startFolder = _cmdAnalysis->splitPath(cmdElementVec[i].getElementStr(), _rootFolder, _curFolder);

		vector<string> strVec = _cmdAnalysis->getFileNameVec();
		Node* node = _cmdAnalysis->pathMatch();
		if (nullptr != node)
		{
			cout << "存在重名文件" << endl;
			continue;
		}

		for (int j = 0; j < strVec.size(); ++j)
		{
			Node* node = startFolder->containNode(strVec[j]);
			Folder* folder = dynamic_cast<Folder*>(node);

			if (nullptr == folder)
			{
				SYSTEMTIME st = {0};
				GetLocalTime(&st);

				Date date(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
				Folder* newFolder = new Folder(strVec[j]);
				newFolder->setDate(date);
				newFolder->setParent(startFolder);
				startFolder = newFolder;
			}
			else
			{
				startFolder = folder;
			}
		}
	}
}
