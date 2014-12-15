#ifndef WIKINET_H
#define WIKINET_H

#include <unordered_map>
#include <string>
#include <set>
#include <vector>
#include <numeric>
#include "sparsevec.h"

namespace wiki{

	class WordNode{
		public:
			WordNode();
			~WordNode(){ delete links;}

			unsigned int getIndex(){return index;};
			static unsigned int getCurrent(){return current;};
			void addLink(WordNode* target);
			std::set<WordNode*>& getLinks();

		private:
			static unsigned int current;
			unsigned int index;
			bool active;
			std::set<WordNode*>* links;
			std::set<WordNode*>* sentence;
	};

	class WikiNet{
		public:
			WikiNet(): wordmap{new std::unordered_map<std::string,WordNode*>()},
				indexmap{new std::unordered_map<int,std::string>()}{}
			WikiNet(std::vector<std::vector<std::string>>& words);
			~WikiNet();

			void addWord(std::string word);
			WordNode* getNode(std::string word);
			std::string& getWord(int index);
			int getSize();
		private:
			std::unordered_map<std::string,WordNode*>* wordmap;
			std::unordered_map<int,std::string>* indexmap;
	};

	// Scoring Functions
	void booleanScore(bool& target, bool& weight, int depth);
	void countScore(int& target, int& weight, int depth);
	void countDepth(double& target, double& weight, int depth);
	void countDisperse(double& target, double& weight, int depth);

	// Helper Functions 
	
	void calcNumChildren(WordNode* w, int depth, int& aggregate);

	template <typename T>
	T getSimilarity(WikiNet& net, WordNode* node1, WordNode* node2,
		   	int depth, T& weight, void (*scoreFunc)(T& target, T& weight, int depth) );

	template <typename T>
	T getSimilarity(WikiNet& net, std::string word1, std::string word2,
		   	int depth, T& weight, void (*scoreFunc)(T& target, T& weight, int depth) );

	template <typename T>
	T getSimilarity(WikiNet& net, std::string word1, std::string word2,
		   	int depth, T& weight, const double thresh,
		   	void (*scoreFunc)(T& target, T& weight, int depth) );

	template <typename T>
	void getScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, const int total, T& weight,
		   	void (*scoreFunc)(T& target, T& weight, int depth) );

	template <typename T>
	void getDispersionScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, const int total, T& weight);

	template <typename T>
	void getIterativeDispersionScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, const int total, T& weight);

	template <typename T>
	double getIterativeConcentrationScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, T& weight);

	template <typename T>
	void getRobinHoodScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w, int depth, T& weight);

	template <typename T>
	void getRecursiveScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* begin, WordNode* w,
		   	int depth, const int total, T& weight,
		   	void (*scoreFunc)(T& target, T& weight, int depth) );

	// Scoring Template Function
	template <typename T>
	void recursiveSimilarity(T& target, T& weight, int depth){
		target += weight/depth;
	}


	template <typename T>
	T getSimilarity(WikiNet& net, WordNode* node1, WordNode* node2, int depth, T& weight,
		   	void (*scoreFunc)(T& target, T& weight, int depth) ){
		sv::SparseVec<T> mask1;
		sv::SparseVec<T> mask2;
		wiki::getScore(net, mask1, node1, depth, depth + 1, weight, scoreFunc);
		wiki::getScore(net, mask2, node2, depth, depth + 1, weight, scoreFunc);
		T similarity = sv::cosSim(mask1,mask2);
		return similarity;
	}

	template <typename T>
	T getSimilarity(WikiNet& net, std::string word1, std::string word2, int depth, T& weight,
		   	void (*scoreFunc)(T& target, T& weight, int depth) ){
		wiki::WordNode* node1 = net.getNode(word1);
		wiki::WordNode* node2 = net.getNode(word2);
		sv::SparseVec<T> mask1;
		sv::SparseVec<T> mask2;
		wiki::getScore(net, mask1, node1, depth, depth + 1, weight, scoreFunc);
		wiki::getScore(net, mask2, node2, depth, depth + 1, weight, scoreFunc);
		T similarity = sv::cosSim(mask1,mask2);
		return similarity;
	}

	template <typename T>
	T getSimilarity(WikiNet& net, std::string word1, std::string word2, int depth,
		   	T& weight, const double thresh, void (*scoreFunc)(T& target, T& weight, int depth) ){
		wiki::WordNode* node1 = net.getNode(word1);
		wiki::WordNode* node2 = net.getNode(word2);
		sv::SparseVec<T> mask1;
		sv::SparseVec<T> mask2;
		wiki::getScore(net, mask1, node1, depth, depth + 1, weight, scoreFunc);
		wiki::getScore(net, mask2, node2, depth, depth + 1, weight, scoreFunc);
		mask1.threshold(thresh);
		mask2.threshold(thresh);
		T similarity = sv::cosSim(mask1,mask2);
		return similarity;
	}

	
	template <typename T>
	void getScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w, int depth, const int total,
		   	T& weight, void (*scoreFunc)(T& target, T& weight, int depth) ){
		const int score = total - depth;
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			for(auto each : next){
				getScore(net, v, each, depth, total, weight, scoreFunc);
				//getScore(net, v, each, depth, total, weight, scoreFunc);
			}
		}
		T& target = v[w->getIndex()];
		scoreFunc(target, weight, score);//use scoring function to update target
	}

	template <typename T>
	void getDispersionScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, T& weight){
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			int numlinks = w->getLinks().size();
			double dispersion = 1.0;
			if(numlinks != 0){
				dispersion = 1.0/numlinks;
			}
			for(auto each : next){
				getDispersionScore(net, v, each, depth, dispersion);
			}
		}
		v[w->getIndex()] += weight;
	}

	template <typename T>
	void getIterativeDispersionScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, T& weight){
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			int numlinks = w->getLinks().size();
			double dispersion = weight;
			if(numlinks != 0){
				dispersion = weight/numlinks;
			}
			for(auto each : next){
				getIterativeDispersionScore(net, v, each, depth, dispersion);
			}
		}
		v[w->getIndex()] += weight;
	}

	template <typename T>
	double getIterativeConcentrationScore(WikiNet& net, sv::SparseVec<T>& v,
		   	WordNode* w, int depth, T& weight){
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			int numlinks = w->getLinks().size();
			if(numlinks == 0){
				v[w->getIndex()] = weight;
				return weight;
			}
			double sumLower = 0;
			for(auto each : next){
				sumLower += getIterativeConcentrationScore(net, v, each, depth, weight);
			}
			v[w->getIndex()] = sumLower;
			return sumLower;
		}
		v[w->getIndex()] = weight;
		return weight;
	}

	template <typename T>
	void getRobinHoodScore(WikiNet& net, sv::SparseVec<T>& v, WordNode* w,
		   	int depth, T& weight){
		if(depth>0){
			std::set<WordNode*> next = w->getLinks();
			std::vector<int> children;
			for(auto each : next){
				int aggregate = 0;
				if(depth > 1){ //if depth == 1 we know there are no children
					//calcNum will always at least count itself, we don't want that
					calcNumChildren(each, depth, aggregate);
				}
				children.push_back(aggregate);
			}
			--depth;
			int totalchildren = std::accumulate(children.begin(), children.end(), 0);
			//THIS CODE IS UGLY! But i couldn't think of a better way
			double max = 0;
			double childweight;
			int index = 0;
			for(auto each : next){
				childweight = 1;
				if(children[index] != 0){
					childweight = (double(totalchildren)/children[index]);
				}
				if(childweight > max){
					max = childweight;
				}
				index++;
			}
			index = 0;
			for(auto each : next){
				childweight = 1.0/w->getLinks().size();
				if(children[index] != 0){
					childweight = (double(totalchildren)/(max*children[index]));
				}
				double newweight = weight*childweight;
				getRobinHoodScore(net, v, each, depth, newweight);
				index++;
			}
		}
		v[w->getIndex()] += weight;
	}

	template <typename T>
	void getRecursiveScore(WikiNet& net, sv::SparseVec<T>& v,WordNode* begin, WordNode* w,
		   	int depth, const int total, T& weight, void (*scoreFunc)(T& target, T& weight, int depth) ){
		const int score = total - depth;
		if(depth>0){
			--depth;
			std::set<WordNode*> next = w->getLinks();
			for(auto each : next){
				T similarity = 0.0;
				similarity = getSimilarity(net, begin, each, depth, weight, recursiveSimilarity);
				getRecursiveScore(net, v, begin, each, depth, total, similarity, scoreFunc);
			}
		}
		T& target = v[w->getIndex()];
		scoreFunc(target, weight, score);//use scoring function to update target
	}
}

#endif
