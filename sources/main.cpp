#include "tests/tests.h"
#include "pipeline/pipeline.h"


int main(int argc, char* argv[])
{
	std::unordered_map<ArgumentType, std::string> inputArguments = ParseArguments(argc, argv);

	if (inputArguments.find(ArgumentType::TEST) != inputArguments.end()) {
		Test(inputArguments);
	}
	else
	{
		Pipeline task;
		if (!task.OnInit(inputArguments))return -1;
		task.Run();
		task.OnExit();
	}

	return 0;
}