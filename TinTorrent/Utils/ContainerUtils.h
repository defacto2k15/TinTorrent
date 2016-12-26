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
};


#endif //TINTORRENT_CONTAINERUTILS_H
