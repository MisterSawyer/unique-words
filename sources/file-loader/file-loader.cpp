#include "file-loader.h"
#include <fstream>
#include <utility>
#include <iostream>
#include <cmath>

FileLoader::FileLoader()
{
	m_initialized = false;
	m_path = "";
	m_fileLength = 0;
}

FileLoader::FileLoader(std::filesystem::path && path) : FileLoader()
{
	m_path = std::move(path);
	if (CalculateFileLength() != 0)m_initialized = true;
}

FileLoader::FileLoader(const std::string& fileName) : FileLoader(std::filesystem::path(fileName))
{
}

std::streamsize FileLoader::GetFileLength() const
{
	return m_fileLength;
}

std::filesystem::path FileLoader::GetFilePath() const
{
	return m_path;
}

void FileLoader::DivideIntoBlocks(const std::size_t & blocks, const char& optionalSeparator)
{
	if (m_fileLength == 0)return;
	if (blocks == 0)
	{
		std::cerr << "Number of blocks cannot be equal to 0.";
		return;
	}
	m_blocksRanges.clear();
	m_blocksRanges.reserve(blocks);

	const std::size_t charactersInBlock = (std::size_t)std::ceil((double)m_fileLength / (double)blocks);

	std::ifstream file(m_path);

	/*
		Using STD convention
		block is a range described by:
		[start; end)
	*/
	
	std::size_t blockStart = 0;
	std::size_t blockEnd = charactersInBlock;

	while (blockStart <= (std::size_t)m_fileLength)
	{
		while (!file.eof() && !std::isspace(file.seekg(blockEnd).peek()))
		{
			blockEnd++;
		}
		m_blocksRanges.push_back(std::make_pair(blockStart, blockEnd));
		blockStart = blockEnd;
		blockEnd = blockStart + charactersInBlock;
	}
	file.close();
}

const std::vector<std::pair<std::size_t, std::size_t>>& FileLoader::GetBlocks() const
{
	return m_blocksRanges;
}

bool FileLoader::Good() const
{
	return m_initialized;
}

std::streamsize FileLoader::CalculateFileLength()
{
	// Simple tellg on end might not be enough:
	// "tellg does not report the size of the file, nor the offset from the beginning in bytes.
	// It reports a token value which can later be used to seek to the same place, and nothing more.
	// (It's not even guaranteed that you can convert the type to an integral type.)"
	// https://port70.net/~nsz/c/c11/n1570.html#7.21.9.4p2

	m_fileLength = 0;

	std::ifstream file(m_path);
	if (!file.good())return m_fileLength;

	while (file.ignore(std::numeric_limits<std::streamsize>::max()))
	{
		m_fileLength += file.gcount();
	}

	file.close();

	return m_fileLength;
}
