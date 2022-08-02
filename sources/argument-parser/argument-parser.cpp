#include "argument-parser.h"

std::unordered_map<ArgumentType, std::string> ParseArguments(const int argc, char* argv[])
{
	if (argc < 2 || argc > 3)return std::unordered_map<ArgumentType, std::string>();

	std::unordered_map<ArgumentType, std::string> returnValue;

	for (int i = 1; i < argc; ++i)
	{
		std::string str(argv[i]);
	
		auto pos = str.find("-t");
		if (pos != str.npos)
		{
			pos = str.find_first_of("0123456789");

			if (pos != str.npos) {
				returnValue[ArgumentType::THREADS] = str.substr(pos);
			}
		}
		else if((pos = str.find("-x")) != str.npos)
		{
			returnValue[ArgumentType::TEST] = "true";
		}
		else
		{
			returnValue[ArgumentType::FILE_NAME] = str;
		}
	}

	return returnValue;
}

template <>
std::optional<std::size_t> ConvertArgument(const std::string& str)
{
	std::size_t argument = 0;

	try {
		argument = std::stoi(str);
	}
	catch (const std::invalid_argument& ex)
	{
		std::cerr << "Invalid argument " << ex.what();
		return {};
	}
	catch (const std::out_of_range& ex)
	{
		std::cerr << "Argument out of possible range " << ex.what();
		return {};
	}
	catch (...)
	{
		std::cerr << "Unknown error";
		return {};
	}

	return std::optional{ argument };
}