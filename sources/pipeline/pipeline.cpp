#include "pipeline.h"
#include "../thread-scheduler/thread-scheduler.h"
#include "../utils/utils.h" // printHelp

bool Pipeline::OnInit(const std::unordered_map<ArgumentType, std::string>& args)
{
	m_inputArguments = args;

	//Try to parse input arguments
	if (m_inputArguments == std::unordered_map<ArgumentType, std::string>())
	{
		std::cout << "Incorrect number of arguments. Expected 1, 2  or 3 arguments.";
		printHelp();
		return false;
	}

	if (m_inputArguments.find(ArgumentType::FILE_NAME) == m_inputArguments.end())
	{
		std::cout << "Incorrect file name.";
		printHelp();
		return false;
	}

	//Create file loader
	m_loader = std::make_unique<FileLoader>(m_inputArguments.at(ArgumentType::FILE_NAME));
	if (!m_loader->Good())
	{
		std::cerr << "Cannot load file " << m_inputArguments.at(ArgumentType::FILE_NAME) << std::endl;
		return false;
	}

	//Set number of threads / blocks
	m_numberOfBlocks = 50;
	if (m_inputArguments.find(ArgumentType::THREADS) != m_inputArguments.end())
	{
		auto argumentConversion = ConvertArgument<std::size_t>(m_inputArguments.at(ArgumentType::THREADS));
		if (argumentConversion.has_value())
		{
			m_numberOfBlocks = argumentConversion.value();
		}
	}
	std::cout << "Running with " << m_numberOfBlocks << " threads" << std::endl;

	// Divide into blocks
	m_loader->DivideIntoBlocks(m_numberOfBlocks);

	//Create concurent set
	m_concurentSet = std::make_unique<ConcurentSet<std::string>>(m_numberOfBlocks);
	m_loader->GetFilePath();
	m_loader->GetFileLength();

	return true;
}

void Pipeline::Run()
{
	ThreadScheduler scheduler;
	// Create threads and assign them to specyfic blocks
	scheduler.Start(*m_concurentSet, *m_loader);

	//Join threads
	scheduler.Synchronize();
}

void Pipeline::OnExit()
{
	//Accumulate results from blocks
	std::cout << "Number of distinct words: " << m_concurentSet->GetSize();
}