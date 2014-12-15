#include "reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>

void strip(char target, std::string& str){
	for(std::string::iterator it=str.begin(); it!=str.end() && *it == target; it++){
		str.erase(it);
		it--;
	}
	for(std::string::iterator it=str.end()-1; it!=str.begin() && *it == target; --it){
		str.erase(it);
	}
} 

std::vector<std::vector<std::string>>& reader::read(std::string file,
										std::vector<std::vector<std::string>>& lines){
	std::ifstream fs {file};
	if(fs){
		std::string line;
		while(fs.good()){
			std::getline(fs,line);	
			if( line.size() > 1 ){ //check this later, it is a bit clumsy
				std::istringstream sline {line};
				std::vector<std::string> words {};
				std::string target {};
				while(sline.good()){
					std::getline(sline,target,'\t');
					strip(' ',target);
					words.push_back(target);
				}
				lines.push_back(words);
			}
		}
		return lines;
	}
	else{
		throw "File not found";
	}
}
