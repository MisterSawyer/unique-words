#ifndef TESTS_H
#define TESTS_H
#include <string>
#include <unordered_map>

#include "../argument-parser/argument-parser.h"

void Test(std::unordered_map<ArgumentType, std::string> args);
std::size_t GetUniqueWordsTrie(const std::string& name);
std::size_t GetUniqueWordsSTD(const std::string& name);

void GenerateTestFile(std::size_t bytes, const std::string& name);

#endif