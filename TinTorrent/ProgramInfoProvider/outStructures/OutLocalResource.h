//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_OUTLOCALRESOURCE_H
#define TINTORRENT_OUTLOCALRESOURCE_H


#include <ModelEntities/Resource.h>

class OutLocalResource {
public:
	Resource resource;
	uint8_t percentDownloaded;

	OutLocalResource(const Resource &resource, uint8_t percentDownloaded);
};


#endif //TINTORRENT_OUTLOCALRESOURCE_H
