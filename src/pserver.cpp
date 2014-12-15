#include "wikinet.h"
#include "reader.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;
const int pcomp = 2;
const unordered_set<string> options = {"count", "countdepth",
	"dispersion", "dispersion+", "concentration+", "robinhood", "recursive"};

int main(int argc, const char* argv[]){
	int depth;
	string mode;
	bool debug = false;
	if(argc >= 3 && argc <= 4){
		depth = atoi(argv[1]);
		mode = argv[2];
		if(options.find(mode) == options.end()){
			cout<<"mode selected not valid, please choose a mode from : "
				"boolean, count, countdepth, dispersion, dispersion+, "
				"concentration+, robinhood or recursive"<<endl;
			return 1;
		}
		if(argc == 4){
			string dbg = argv[3];
			if(dbg == "-debug"){
				debug = true;
			}
		}
	}
	else{
		cout<<"Please pass an integer as depth and a string as mode "
			"(boolean, count, countdepth, dispersion, dispersion+, "
			"concentration+, robinhood or recursive)"<<endl;
		return 1;
	}

	string datapath {"data/"};
	string filename {"wnsyns.tsv"};
	vector<vector<string>> defs {};

	defs = reader::read(datapath + filename, defs);

	wiki::WikiNet net {defs};

	while(1){
		// Read text from stdin
		vector< vector<string> > phrase{};
		bool usethreshold = false;
		double threshold = 0.0;
		for(int numphrase=0; numphrase < pcomp; numphrase++){
			for (string line; getline(cin, line);){
				istringstream sline {line};
				vector<string> words{};
				string target{};
				while(sline.good()){
					getline(sline,target,' ');
					words.push_back(target);
				}
				if(words.size() == 1){
					if(words[0] == "exit"){
						cout<<"Byee!!"<<endl;
						return 0;
					}
					phrase.push_back(words);
					break;
				}
				else if(words.size() >= 2){
					phrase.push_back(words);
					break;
				}
				else{
					cout<<"Format should be : phrase1 \\n phrase2 \\n,"
						" where words are strings , all separated by gaps"
						" or the word exit alone, to end the programme"<<endl;
					break;
				}
			}
		}

		if(mode == "boolean"){
			bool weight = 1;
			vector< sv::SparseVec<bool> > vecs;
			sv::SparseVec<bool> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<bool> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getScore(net, vec, node1, depth, depth+1,
							weight, wiki::booleanScore);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "count"){
			int weight = 1;
			vector< sv::SparseVec<int> > vecs;
			sv::SparseVec<int> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<int> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getScore(net, vec, node1, depth,
							depth+1, weight, wiki::countScore);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "countdepth"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getScore(net, vec, node1, depth,
							depth+1, weight, wiki::recursiveSimilarity);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "recursive"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getRecursiveScore(net, vec, node1, node1, depth,
							depth+1, weight, wiki::recursiveSimilarity);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "dispersion"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getDispersionScore(net, vec, node1, depth, weight);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "dispersion+"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getIterativeDispersionScore(net, vec, node1, depth, weight);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "concentration+"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getIterativeConcentrationScore(net, vec, node1, depth, weight);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

		if(mode == "robinhood"){
			double weight = 1.0;
			vector< sv::SparseVec<double> > vecs;
			sv::SparseVec<double> total;
			for(int numphrase=0; numphrase < pcomp; numphrase++){
				sv::SparseVec<double> vec;
				for(auto each : phrase[numphrase]){
					wiki::WordNode* node1 = net.getNode(each);
					if(node1 == nullptr){
						net.addWord(each);	
						node1 = net.getNode(each);
					}
					wiki::getRobinHoodScore(net, vec, node1, depth, weight);
					if(usethreshold){
						vec.threshold(threshold);
					}
					if(debug){
						for(auto each: vec){
							cout<<net.getWord(each.first)<<" : "<<each.second<<' ';
						}
						cout<<endl;
					}
					total += vec;
				}
				vecs.push_back(vec);
			}
			cout<<cosSim(vecs[0],vecs[1])<<endl;
		}

	}
}
