#include "tests.h"
#include <chrono>
#include <unordered_set>
#include <fstream>

#include "../trie/trie.h"
#include "../pipeline/pipeline.h"

void Test(std::unordered_map<ArgumentType, std::string> args)
{
	constexpr std::size_t testSize = 10 * (1024 * 1024); //
	std::string fileName = "test_" + std::to_string(testSize) + ".txt";

	if (args.find(ArgumentType::FILE_NAME) == args.end()) {
		// there is no argument of file_name - so generate new one
		args[ArgumentType::FILE_NAME] = fileName;
		std::cout << "Generating " << testSize << " bytes test file (" << testSize / (1024 * 1024) << " MB) ... \n";
		GenerateTestFile(testSize, fileName);
		std::cout << "File " << fileName << " generated\n";
	}
	else
	{
		// use provided file_name
		fileName = args.at(ArgumentType::FILE_NAME);
		std::cout << "Using " << fileName << " test file \n";
	}

	{
		std::cout << "\n\n--- Concurent set --- \n";
		auto start = std::chrono::system_clock::now();
		Pipeline task;
		if (!task.OnInit(args))return;
		task.Run();
		auto end = std::chrono::system_clock::now();

		task.OnExit();
		std::cout << "\nTime:\t\t" << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
	}

	{
		std::cout << "\n\n--- STD unordered_set  --- \n";
		auto start = std::chrono::system_clock::now();
		std::cout << "Number of distinct words: " << GetUniqueWordsSTD(fileName);
		auto end = std::chrono::system_clock::now();
		std::cout << "\ntime:\t\t" << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
	}

	{
		std::cout << "\n\n--- Trie --- \n";
		auto start = std::chrono::system_clock::now();
		std::cout << "Number of distinct words: " << GetUniqueWordsTrie(fileName);
		auto end = std::chrono::system_clock::now();
		std::cout << "\ntime:\t\t" << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
	}
}

std::size_t GetUniqueWordsTrie(const std::string& name)
{
	TrieSet<char> trie('a', 26);
	std::ifstream file(name);

	std::string word;
	while (file >> word)
	{
		trie.Insert(word.data(), word.length());
	}

	return trie.GetSize();
}

std::size_t GetUniqueWordsSTD(const std::string& name)
{
	std::unordered_set<std::string> set;
	std::ifstream file(name);

	std::string word;
	while (file >> word)
	{
		set.insert(word);
	}

	return set.size();
}



void GenerateTestFile(std::size_t bytes, const std::string& name)
{
	std::ofstream file(name);

	while (bytes > 0)
	{
		if (rand() % 100 < 30)
		{
			// space
			file << " ";
		}
		else
		{
			//letter
			file << (char)((rand() % 26) + 'a');
		}

		bytes--;
	}
}