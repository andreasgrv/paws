#ifndef READER_H
#define READER_H
#include <vector>
#include <string>

namespace reader{
	std::vector<std::vector<std::string>>& read(std::string file,
			std::vector<std::vector<std::string>>& putmehere);
}
#endif
