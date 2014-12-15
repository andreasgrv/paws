#include "sparsevec.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace sv{

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator=(const SparseVec& x){
			vec = new std::map<int,T>();
			vec->insert(x.cbegin(),x.cend());
			return (*this);
		}

	template <typename T>
		typename std::map<int,T>::iterator SparseVec<T>::begin() const{
			return (*vec).begin();
		}

	template <typename T>
		typename std::map<int,T>::iterator SparseVec<T>::end() const{
			return (*vec).end();
		}

	template <typename T>
		typename std::map<int,T>::const_iterator SparseVec<T>::cbegin() const {
			return (*vec).cbegin();
		}

	template <typename T>
		typename std::map<int,T>::const_iterator SparseVec<T>::cend() const {
			return (*vec).cend();
		}

	template <typename T>
		T& SparseVec<T>::operator[](int index){
			return (*vec)[index];
		}

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator-(){
			for(auto each : (*this)){
				(*vec)[each.first] = -each.second;
			}	
			return (*this);
		}

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator+=(const SparseVec<T>& target){
			for( auto each : target){
				(*vec)[each.first] += each.second;
			}
			return (*this);
		}

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator-=(const SparseVec<T>& target){
			for( auto each : target){
				(*vec)[each.first] -= each.second;
			}
			return (*this);
		}

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator*=(const SparseVec<T>& target){
			for( auto each : target){
				for(auto other: (*this)){
					if(each.first == other.first){
						(*vec)[each.first] *= each.second;
					}
				}
			}
			return (*this);
		}

	template <typename T>
		SparseVec<T>& SparseVec<T>::operator/=(const SparseVec<T>& target){
			for( auto each : target){
				for(auto other: (*this)){
					if(each.first == other.first){
						if(each.second == 0){
							throw std::overflow_error("Divide by zero exception");
						}
						(*vec)[each.first] /= each.second;
					}
				}
			}
			return (*this);
		}

	template <typename T>
		void SparseVec<T>::threshold(T limit){
			auto it = (*this).begin();
			while(it != (*this).end()){
				if((*it).second < limit){
					it = this->vec->erase(it);
				}
				else{
					++it;
				}
			}
		}

	template <typename T>
		void SparseVec<T>::normalize(){
			T max = (*std::max_element(this->begin(), this->end(),
						[](const std::pair<int, unsigned>& p1, const std::pair<int, unsigned>& p2)
						{ return p1.second < p2.second; })).second;
			for(auto &each : (*this)){
				each.second /= max;
			}
		}

	template <typename T>
		std::map<int,T>& SparseVec<T>::getMap(){
			return (*vec);
		}

	template <typename T>
		T sum(const SparseVec<T>& sv1, const SparseVec<T>& sv2){
			T result = 0;
			sv1+=sv2;
			for(auto each: sv1){
				result += each.second;
			}
			return result;
		}


	template <typename T>
		T mult(const SparseVec<T>& sv1, const SparseVec<T>& sv2){
			T result = 0;
			for(auto each: sv1){
				for(auto other: sv2){
					if(each.first == other.first){
						result += each.second*other.second;
					}
				}
			}
			return result;
		}

	template <typename T>
		double cosSim(const SparseVec<T>& sv1, const SparseVec<T>& sv2){
			double dotprod = 0.0;
			double norm1 = 0;
			double norm2 = 0;
			bool countonce = false;
			for(auto each : sv1){
				norm1 += pow(each.second,2);
				for(auto other : sv2){
					if(!countonce){
						norm2 += pow(other.second,2);
					}		
					if(each.first == other.first){
						dotprod += each.second*other.second;
					}
				}
				countonce = true;
			}
			double denominator = (sqrt(norm1) * sqrt(norm2));
			if(denominator == 0){
				return 0;
			}
			return dotprod / denominator;
		}
}
