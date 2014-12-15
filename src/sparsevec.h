#ifndef SPARSEVEC_H
#define SPARSEVEC_H

#include <map>

namespace sv{

	template <typename T>
		class SparseVec{
			public:
				SparseVec(): vec{ new std::map<int,T>()}{};
				SparseVec(const SparseVec& x): vec{ new std::map<int,T>()}{
					vec->insert(x.cbegin(),x.cend());
				};
				SparseVec(std::initializer_list<std::pair<int,T>> lst):
					vec{ new std::map<int,T>(lst.begin(),lst.end() ) } {}
				~SparseVec(){ delete vec;};
				typename std::map<int,T>::iterator begin() const;
				typename std::map<int,T>::iterator end() const;
				typename std::map<int,T>::const_iterator cbegin() const ;
				typename std::map<int,T>::const_iterator cend() const ;
				SparseVec& operator=(const SparseVec& x);
				T& operator[](int index);
				SparseVec& operator-();
				SparseVec& operator+=(const SparseVec& other);
				SparseVec& operator-=(const SparseVec& other);
				SparseVec& operator*=(const SparseVec& other);
				SparseVec& operator/=(const SparseVec& other);

				void threshold(T limit);
				void normalize();
				std::map<int,T>& getMap();
			private:
				std::map<int,T>* vec;
		};

	template<typename T>
		T sum(const SparseVec<T>& sv1, const SparseVec<T>& sv2);

	template<typename T>
		T mult(const SparseVec<T>& sv1, const SparseVec<T>& sv2);

	template<typename T>
		double cosSim(const SparseVec<T>& sv1, const SparseVec<T>& sv2);

};

#include "sparsevecimpl.h"
#endif
