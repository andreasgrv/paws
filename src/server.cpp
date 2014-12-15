#include "wikinet.h"
#include "reader.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
const double notfound = -1;

int main(int argc, const char* argv[]){
	int depth;
	string mode;
	bool debug = false;
	if(argc == 3){
		depth = atoi(argv[1]);
		mode = argv[2];
		if(mode!="boolean" && mode!="count" && mode!="countdepth" && mode!="dispersion" && mode!="dispersion+" && mode!="concentration+" && mode!="robinhood" && mode!="recursive"){
			cout<<"mode selected not valid, please choose a mode from : boolean, count, countdepth, dispersion, dispersion+, concentration+, robinhood or recursive"<<endl;
			return 1;
		}
	}
	else if(argc == 4){
		depth = atoi(argv[1]);
		mode = argv[2];
		if(mode!="boolean" && mode!="count" && mode!="countdepth" && mode!="dispersion" && mode!="dispersion+" && mode!="concentration+" && mode!="robinhood" && mode!="recursive"){
			cout<<"mode selected not valid, please choose a mode from : boolean, count, countdepth, dispersion, dispersion+, concentration+, robinhood or recursive"<<endl;
			return 1;
		}
		string dbg = argv[3];
		if(dbg == "-debug"){
			debug = true;
		}
	}
	else{
		cout<<"Please pass an integer as depth and a string as mode (boolean, count, countdepth, dispersion, dispersion+, concentration+, robinhood or recursive)"<<endl;
		return 1;
	}

	string datapath {"data/"};
	string filename {"wnsyns.tsv"};
	vector<vector<string>> words {};

	words = reader::read(datapath + filename, words);
	
	wiki::WikiNet net {words};

	while(1){
		for (string line; getline(cin, line);){
			istringstream sline {line};
			vector<std::string> words {};
			string target, word1, word2;
			bool usethreshold = false;
			double threshold = 0.0;
			while(sline.good()){
				std::getline(sline,target,' ');
				words.push_back(target);
			}
			if(words.size() == 1){
				if(words[0] == "exit"){
					cout<<"Byee!!"<<endl;
					return 0;
				}
				else{
					cout<<"Format should be : word1 word2 threshold, where words are strings and threshold is double, all separated by gaps or the word exit alone, to end the programme"<<endl;
					break;
				}
			}
			else if(words.size() == 2){
				word1 = words[0];
				word2 = words[1];
			}
			else if(words.size() == 3){
				word1 = words[0];
				word2 = words[1];
				threshold = atof(words[2].c_str());
				usethreshold = true;
			}
			else{
				cout<<"Format should be : word1 word2 threshold, where words are strings and threshold is double, all separated by gaps or the word exit alone, to end the programme"<<endl;
				break;
			}

			if(mode == "boolean"){
				bool weight = 1;
				sv::SparseVec<bool> v1;
				sv::SparseVec<bool> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getScore(net, v1, node1, depth, depth+1, weight, wiki::booleanScore);
				wiki::getScore(net, v2, node2, depth, depth+1, weight, wiki::booleanScore);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "count"){
				int weight = 1;
				sv::SparseVec<int> v1;
				sv::SparseVec<int> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getScore(net, v1, node1, depth, depth+1, weight, wiki::countScore);
				wiki::getScore(net, v2, node2, depth, depth+1, weight, wiki::countScore);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "countdepth"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getScore(net, v1, node1, depth, depth+1, weight, wiki::recursiveSimilarity);
				wiki::getScore(net, v2, node2, depth, depth+1, weight, wiki::recursiveSimilarity);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "dispersion"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getDispersionScore(net, v1, node1, depth, weight);
				wiki::getDispersionScore(net, v2, node2, depth, weight);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "dispersion+"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getIterativeDispersionScore(net, v1, node1, depth, weight);
				wiki::getIterativeDispersionScore(net, v2, node2, depth, weight);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "concentration+"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getIterativeConcentrationScore(net, v1, node1, depth, weight);
				wiki::getIterativeConcentrationScore(net, v2, node2, depth, weight);
//				v1.normalize();
//				v2.normalize();
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "robinhood"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getRobinHoodScore(net, v1, node1, depth, weight);
				wiki::getRobinHoodScore(net, v2, node2, depth, weight);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}
			if(mode == "recursive"){
				double weight = 1.0;
				sv::SparseVec<double> v1;
				sv::SparseVec<double> v2;
				wiki::WordNode* node1 = net.getNode(word1);
				wiki::WordNode* node2 = net.getNode(word2);
				if(node1 == nullptr || node2 == nullptr){
					cout<<notfound<<endl;
					continue;
				}
				wiki::getRecursiveScore(net, v1, node1, node1, depth, depth+1, weight, wiki::recursiveSimilarity);
				wiki::getRecursiveScore(net, v2, node2, node2, depth, depth+1, weight, wiki::recursiveSimilarity);
				if(usethreshold){
					v1.threshold(threshold);
				}
				if(usethreshold){
					v2.threshold(threshold);
				}
				if(debug){
					for(auto each: v1){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl<<endl;
					for(auto each: v2){
						cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
					}
					cout<<endl;
				}
				cout<<cosSim(v1,v2)<<endl;
			}

		}
	}
}
