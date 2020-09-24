#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include <stdint.h>
#include <sstream>
#include <iomanip>

using namespace std;



#define MAX_PROPERTY_VALUE_SIZE 96

#define CARPLAY_ELEMENT_SIZE 7

#define CARPLAY_ELEMENT_ID_FLAGS 0x00
#define CARPLAY_ELEMENT_ID_NAME 0x01
#define CARPLAY_ELEMENT_ID_MANUFACTURE 0x02
#define CARPLAY_ELEMENT_ID_MODEL 0x03
#define CARPLAY_ELEMENT_ID_OUI 0x04
#define CARPLAY_ELEMENT_ID_BLUETOOTH_MAC 0x05 //
#define CARPLAY_ELEMENT_ID_DEVICE_ID 0x06

struct CarplayElement {
	uint8_t id;
	uint8_t len;
	uint8_t* payload;
};

struct CarplayOverallElement {
	uint8_t id;
	uint8_t len;
	uint8_t* OUI;
	uint8_t sub_type;
	const CarplayElement* elements;
};

#define NAME "Ford"
#define MANUFACTURE "Yanfeng,Automotive"
#define BT_ADDRESS "3E:A0:20:5A:20:71"
#define MODEL {0x50, 0x55, 0x31, 0x37, 0x32, 0x33, 0x55, 0x41}
#define OUI_SIZE 3


std::vector<uint8_t> convertBtaddress(const std::string &s, char c) {
    std::vector<uint8_t> components;
    size_t startPos = 0;
    size_t matchPos;
    while ((matchPos = s.find(c, startPos)) != std::string::npos) {
		char* temp = nullptr;
		const char* target = s.substr(startPos, matchPos - startPos).c_str();
		uint8_t value = (uint8_t) strtol( target, &temp, 16);
        components.push_back(value);
        startPos = matchPos + 1;
    }

    if (startPos <= s.length()) {
		char* temp = nullptr;
		const char* target = s.substr(startPos).c_str();
		uint8_t value = (uint8_t)strtol(target, &temp, 16);
        components.push_back(value);
    }
    return components;
}

std::vector<uint8_t> convertStr(const std::string & src) { 
	std::vector<uint8_t> target;
	target.assign(src.begin(), src.end());
	return target;
}

void printf(const std::vector<uint8_t> src) {
	cout << "src size:" << src.size() << endl;
	cout << "[";
	for(uint8_t a : src) {
		printf("%02x", a);
	}
	cout << "]" << endl;
}

std::string convertChar(const uint8_t* from, const uint8_t length) {
	std::string src(from, from+length);
	std::stringstream ss;
	ss << std::hex;
	ss << std::setfill('0');
	for (uint8_t b : src) {
		ss << std::setw(2) << static_cast<unsigned int>(b);
	}
	const std::string src_as_string = ss.str();
	return src_as_string;
}

void convertCarplayElement(stringstream& ss, const CarplayElement& from) {
	ss << std::setw(2) << static_cast<unsigned int>(from.id);
	ss << std::setw(2) << static_cast<unsigned int>(from.len);

	const uint8_t* value = const_cast<uint8_t *>(from.payload);
	printf("[%02x-%02x-", from.id, from.len);
	for(int i = 0; i < from.len; i++) {	
		printf("%02x", *value);
		ss << std::setw(2) << static_cast<unsigned int>(*value);
		value++;
	}
	printf("]\n");
	free(from.payload);
}

std::string convertCarplayIE(const CarplayOverallElement& from, uint8_t element_size) {
	std::stringstream ss;
	ss << std::hex;
	ss << std::setfill('0');
	ss << std::setw(2) << static_cast<unsigned int>(from.id);
	ss << std::setw(2) << static_cast<unsigned int>(from.len);

	const uint8_t* oui = from.OUI;
	for(int i = 0; i < 3; i++) {
		ss << std::setw(2) << static_cast<unsigned int>(*oui);
		oui++;
	}
	free(from.OUI);

	ss << std::setw(2) << static_cast<unsigned int>(from.sub_type);
	const CarplayElement* elements = from.elements;
	for (int i = 0; i< element_size; i++) {
		convertCarplayElement(ss, *elements);
		elements++;
	}
	const std::string result = ss.str();
	return result;
}

std::string createBeaconForCarplay() {
	const std::vector<uint8_t> bt_addr_vector = convertBtaddress(BT_ADDRESS, ':');
	//printf(bt_addr_vector);


	CarplayOverallElement overall_elements;
	memset(&overall_elements, 0, sizeof(overall_elements));

    const uint8_t apple_oui[OUI_SIZE] = {0x00, 0xA0, 0x40};

	overall_elements.id = 0xDD;
	overall_elements.OUI = (uint8_t *)malloc(sizeof(uint8_t)*OUI_SIZE);
	memcpy(overall_elements.OUI, apple_oui, OUI_SIZE);
	overall_elements.sub_type = 0x00;

	CarplayElement elements[CARPLAY_ELEMENT_SIZE];
	int payload_size = 0;
	int element_default_len = 2;
	for(int i = 0; i < CARPLAY_ELEMENT_SIZE; i++) {
		CarplayElement element;
		vector<uint8_t> value;
		uint8_t* payload = nullptr;

		memset(&element, 0, sizeof(element));
		switch (i) 
		{
			case CARPLAY_ELEMENT_ID_FLAGS:
			element.id = CARPLAY_ELEMENT_ID_FLAGS;
			value = {0x00, 0x40};
			
			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*value.size());
			memcpy(element.payload, value.data(), value.size());

			element.len = value.size();
			payload_size += (value.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_NAME:
			element.id = CARPLAY_ELEMENT_ID_NAME;
			value = convertStr(NAME);

			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*value.size());
			memcpy(element.payload, value.data(), value.size());

			element.len = value.size();
			payload_size += (value.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_MANUFACTURE:
			element.id = CARPLAY_ELEMENT_ID_MANUFACTURE;
			value = convertStr(MANUFACTURE);

			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*value.size());
			memcpy(element.payload, value.data(), value.size());

			element.len = value.size();
			payload_size += (value.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_MODEL:
			element.id = CARPLAY_ELEMENT_ID_MODEL;
			value = MODEL;

			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*value.size());
			memcpy(element.payload, value.data(), value.size());

			element.len = value.size();
			payload_size += (value.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_OUI:
			element.id = CARPLAY_ELEMENT_ID_OUI;

			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*OUI_SIZE);
			memcpy(element.payload, apple_oui, OUI_SIZE);

			element.len = OUI_SIZE;
			payload_size += (OUI_SIZE + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_BLUETOOTH_MAC:
			case CARPLAY_ELEMENT_ID_DEVICE_ID:
			element.id = i+1;
			value = bt_addr_vector;

			element.payload = (uint8_t*)malloc(sizeof(uint8_t)*value.size());
			memcpy(element.payload, value.data(), value.size());
			
			element.len = value.size();
			payload_size += (value.size() + element_default_len);
			break;

			default:
			break;
		}
		elements[i] = element;
	}
	overall_elements.len = payload_size;
	overall_elements.elements = elements;
	cout << convertCarplayIE(overall_elements, CARPLAY_ELEMENT_SIZE) << endl;
}

int main() {
//printf(convertStr(NAME));

const uint8_t src[3] = {0x11, 0xA0, 0x20};
/**
cout << convertChar(src, 3) << endl;
struct CarplayOverallElement ie;
memset(&ie, 0, sizeof(ie));
cout << convertStruct((uint8_t*)&ie, 3) << endl;
**/

createBeaconForCarplay();

return 0;
}
