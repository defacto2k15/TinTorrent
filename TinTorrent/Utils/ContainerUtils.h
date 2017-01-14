//
// Created by defacto on 22.12.16.
//

#ifndef TINTORRENT_CONTAINERUTILS_H
#define TINTORRENT_CONTAINERUTILS_H

#include <vector>
#include <functional>
#include <experimental/optional>
#include <algorithm>

class ContainerUtils {
public:
	template<typename T>
	static std::experimental::optional<T> GetElem( std::vector<T> &files, std::function<bool(T &elem)> checkingFunction ){
		auto res = std::find_if(begin(files), end(files), checkingFunction);
		if( res == end(files)){
			return std::experimental::optional<T>();
		} else {
			return std::experimental::optional<T>(*res);
		}
	}

	template<typename T>
	static bool Contains( std::vector<T> &inVec, std::function<bool(T &elem)> checkingFunction ){
		return std::find_if( begin(inVec), end(inVec), checkingFunction) != end(inVec);
	}

	template<typename T>
	static bool Contains( std::vector<T> &inVec, T &elem ){
		return std::find_if( begin(inVec), end(inVec), [&elem](T &other){ return elem == other;}) != end(inVec);
	}

	template<typename T>
	static std::vector<T> getDifference( std::vector<T> &firstVec, std::vector<T> &secondVec, std::function<bool(T&, T&)> checkingFunc){
		std::vector<T> outVec;
		std::copy_if(begin(firstVec), end(firstVec), std::back_inserter(outVec),
		             [&secondVec, checkingFunc]( T &elem1){
			             return !ContainerUtils::Contains<T>( secondVec, [&elem1, checkingFunc]( T &elem2){
				             return checkingFunc( elem1, elem2);
			             });
		             });
		return outVec;
	}

	template< typename T>
	static void remove( std::vector<T> &vec, T &elemToRemove){
		vec.erase(std::remove(vec.begin(), vec.end(), elemToRemove), vec.end());
	}

	template <typename T>
	static void addWithoutDuplicate( std::vector<T> &inVec, T elem ){
		if( !Contains(inVec, elem)){
			inVec.push_back(elem);
		}
	}

	template <typename T>
	static void removeIfPresent( std::vector<T> &inVec, T elem ){
		if( Contains(inVec, elem)){
			remove(inVec, elem);
		}
	}
};


#endif //TINTORRENT_CONTAINERUTILS_H
