//
// Created by defacto on 08.12.16.
//

#include "MessageStartSendingRequest.h"
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::OK(std::string("Ok"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::FIRST(std::string("First"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::WRONG_SEGMENT(std::string("WrongSegment"));
MessageStartSendingRequest::PreviousStatus MessageStartSendingRequest::PreviousStatus::WRONG_SIZE(std::string("WrongSize"));
