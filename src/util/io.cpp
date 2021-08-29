#include "io.hpp"

std::string readFile(std::string path)
{
	return readFile(path.c_str());
}

std::string readFile(const char* path)
{
	std::ifstream file;
	std::stringstream stream;

	file.open(path);
	stream << file.rdbuf();
	file.close();

	return stream.str();
}