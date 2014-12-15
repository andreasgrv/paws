#include "wikinet.h"
#include <cmath>

namespace wiki{

	unsigned int WordNode::current = 0;

	/* -------- Word Node stuff -----------*/

	WordNode::WordNode(){
		active = false;
		links = new std::set<WordNode*>();
		index = current++;
	}

	//adds link to both -aka- graph is not directed
	void WordNode::addLink(WordNode* w){
		links->insert(w);
		//w->getLinks().insert(this);// this aint directed now.
	}

	std::set<WordNode*>& WordNode::getLinks(){
		return *links;
	}


	WikiNet::WikiNet(std::vector<std::vector<std::string>>& words){
		wordmap = new std::unordered_map<std::string,WordNode*>();
		indexmap = new std::unordered_map<int,std::string>();
		bool first;
		std::string target;
		for(auto each : words){
			first = true; // Ready for next iteration
			for(auto word : each){
				if(first){ // Add Word
					addWord(word);
					target = word;
					first = false;
				}
				else{	// Add Definitions
					addWord(word);
					WordNode* add = (*wordmap)[word];
					(*wordmap)[target]->addLink(add);
				}
			}
		}
	}

	WikiNet::~WikiNet(){
		for(auto each : *wordmap){
			delete each.second;
		}
		delete wordmap;
		delete indexmap;
	}

	void WikiNet::addWord(std::string word){
		WordNode* locate = getNode(word);
		if(locate == nullptr){
			WordNode* wn = new WordNode();
			wordmap->insert({{word,wn}});
			indexmap->insert({{wn->getIndex(),word}});
		}
	}

	WordNode* WikiNet::getNode(std::string word){
		std::unordered_map<std::string,WordNode*>::const_iterator found = wordmap->find(word);
		if(found == wordmap->end()){
			return nullptr;
		}
		return (*wordmap)[word];
	}

	std::string& WikiNet::getWord(int index){
		std::unordered_map<int, std::string>::const_iterator found = indexmap->find(index);
		if(found == indexmap->end()){
			throw "Index does not exist";
		}
		return (*indexmap)[index];
	}

	int WikiNet::getSize(){
		return wordmap->size();
	}

	// Scoring Functions
	void booleanScore(bool& target, bool& weight, int depth){
		target = 1;
	}

	void countScore(int& target, int& weight, int depth){
		target++;
	}

	void countDepth(double& target, double& weight, int depth){
		target += 1.0/depth;
	}

	void countDisperse(double& target, double& weight, int depth){
		target += weight;
	}

	// Helper Functions
	void calcNumChildren(WordNode* w, int depth, int& aggregate){
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			int size = w->getLinks().size();
			aggregate += 1;
			for(auto each : next){
				calcNumChildren(each, depth, aggregate);
			}
			return;
		}
		return;
	}
}
