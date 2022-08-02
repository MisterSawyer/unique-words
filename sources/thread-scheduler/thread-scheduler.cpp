#include <fstream>

#include "thread-scheduler.h"

bool ThreadFunction(ConcurentSet<std::string>& concurentSet, const std::pair<std::size_t, std::size_t>& dataBlock, const std::filesystem::path& filePath)
{
	std::ifstream file(filePath);
	if (!file)return false;

	file.seekg(dataBlock.first);
	std::string input;
	while (!file.eof() && (std::size_t)file.tellg() < dataBlock.second)
	{
		file >> input;
		concurentSet.Insert(input);
	}

	return true;
}

void ThreadScheduler::Start(ConcurentSet<std::string> & concurentSet, const FileLoader& loader)
{
	for (const auto& block : loader.GetBlocks())
	{
		std::thread th{ ThreadFunction, std::ref(concurentSet), block, loader.GetFilePath()};
		m_threads.push_back(std::move(th));
	}
}

void ThreadScheduler::Synchronize()
{
	while (!m_threads.empty())
	{
		for (auto it = m_threads.begin(); it != m_threads.end(); it++)
		{
			if (!it->joinable())continue;

			it->join();
			m_threads.erase(it);
			break;
		}
	}
}
