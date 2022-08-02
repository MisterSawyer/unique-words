#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <unordered_map>
#include <optional>
#include <string>
#include <iostream>

enum class ArgumentType
{
	FILE_NAME, THREADS, TEST
};

std::unordered_map<ArgumentType, std::string> ParseArguments(const int argc, char* argv[]);

template <class T>
std::optional<T> ConvertArgument(const std::string& str);


#endif