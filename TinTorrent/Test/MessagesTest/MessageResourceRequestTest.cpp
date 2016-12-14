//
// Created by defacto on 12.12.16.
//

#include <gtest/gtest.h>
#include "../../Common/InMemoryBuffer.h"
#include "../../ModelEntities/Resource.h"
#include "../../TinProtocolLib/Messages/MessageResourceRequest.h"

class MessageResourceRequestTest: public ::testing::Test {
public:
	MessageResourceRequestTest() : ::testing::Test(){
	}

};

TEST_F( MessageResourceRequestTest, SerializationOfRequestWorks) {
	Resource resource(L"fileName.txt", 2312);

	MessageResourceRequest request1(resource);
	json j = request1.toJson();
	MessageResourceRequest request2(j);
	ASSERT_EQ(request1, request2);
}

