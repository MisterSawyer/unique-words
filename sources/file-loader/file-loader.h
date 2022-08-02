#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <filesystem>

class FileLoader
{
public:
	explicit FileLoader(const std::string & fileName);
	explicit FileLoader(std::filesystem::path && path);

	~FileLoader() = default;

	FileLoader(const FileLoader& other) = delete;
	FileLoader(const FileLoader&& other) noexcept = delete;
	FileLoader& operator=(const FileLoader& other) = delete;
	FileLoader& operator=(FileLoader && other) noexcept = delete;
	// ---

	std::streamsize GetFileLength() const;
	std::filesystem::path GetFilePath()const;

	/*
		Default implementation uses std::is_space function
	*/
	void DivideIntoBlocks(const std::size_t& blocks, const char & optionalSeparator = 0);
	const std::vector<std::pair<std::size_t, std::size_t>> & GetBlocks() const;

	bool Good() const;

protected:

private:
	FileLoader();
	std::streamsize CalculateFileLength();

	bool m_initialized;
	std::filesystem::path m_path;
	std::streamsize m_fileLength;

	std::vector<std::pair<std::size_t, std::size_t>> m_blocksRanges;
};

#endif