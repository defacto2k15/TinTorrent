//
// Created by defacto on 08.12.16.
//

#include "MessageEnum.h"

MessageEnum::MessageEnum(const std::string &value) :value(value){
}

std::string MessageEnum::getValue() const {
	if( value.length()==0){
		throw std::logic_error("Message enum zainicjowany domyslnym konstruktorem, uzytecznym tylko do parsowania");
	}
	return value;
}

bool MessageEnum::operator==(const MessageEnum &other) const {
	return value == other.value;
}

void MessageEnum::parseJson(std::string jsonValue, std::vector<MessageEnum> possibleValues) {
	auto x = std::find_if(begin(possibleValues), end(possibleValues),
	                      [jsonValue](MessageEnum & messageEnum){ return messageEnum.getValue()==jsonValue;});
	if( x == possibleValues.end()){
		throw std::logic_error(std::string("Blad przy parsowaniu MessageEnum. Nie ma takiego o wartosci: ")+jsonValue);
	}
	value = jsonValue;
}

MessageEnum::~MessageEnum() {
}

std::ostream &operator<<(std::ostream &os, const MessageEnum &anEnum) {
	os << "value: " << anEnum.value;
	return os;
}

MessageEnum::MessageEnum() {
}
