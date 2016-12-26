//
// Created by defacto on 21.12.16.
//

#ifndef TINTORRENT_MOCKFILEINFORMATIONCONSUMER_H
#define TINTORRENT_MOCKFILEINFORMATIONCONSUMER_H

#include "../../FileManager/FileInformationConsumer.h"
#include "gmock/gmock.h"

class MockFileInformationConsumer : public FileInformationConsumer{
public:
//	MOCK_METHOD1(initialFileCheck, void(std::shared_ptr<std::vector<FileInfo>> fileInfos) ); //todo delete
};
#endif //TINTORRENT_MOCKFILEINFORMATIONCONSUMER_H
