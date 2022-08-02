#ifndef PIPELINE_H
#define PIPELINE_H

#include <unordered_map>
#include <memory>

#include "../argument-parser/argument-parser.h"
#include "../file-loader/file-loader.h"
#include "../concurent-set/concurent-set.h"

class Pipeline
{
public:
	bool OnInit(const std::unordered_map<ArgumentType, std::string>& args);
	void Run();
	void OnExit();
private:
	std::unordered_map<ArgumentType, std::string> m_inputArguments;
	std::unique_ptr<FileLoader> m_loader;
	std::unique_ptr<ConcurentSet<std::string>> m_concurentSet;
	std::size_t m_numberOfBlocks;
};

#endif