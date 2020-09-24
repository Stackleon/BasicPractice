#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>

using namespace std;


#define MAX_PROPERTY_VALUE_SIZE 96

#define CARPLAY_ELEMENT_SIZE 7

#define CARPLAY_ELEMENT_ID_FLAGS 0x00
#define CARPLAY_ELEMENT_ID_NAME 0x01
#define CARPLAY_ELEMENT_ID_MANUFACTURER 0x02
#define CARPLAY_ELEMENT_ID_MODEL 0x03
#define CARPLAY_ELEMENT_ID_OUI 0x04
#define CARPLAY_ELEMENT_ID_BLUETOOTH_MAC 0x05 //
#define CARPLAY_ELEMENT_ID_DEVICE_ID 0x06

struct CarplayElement {
	uint8_t id;
	int len;
	const uint8_t* payload;
};

struct CarplayOverallElement {
	uint8_t id;
	int len;
	const uint8_t* OUI;
	uint8_t sub_type;
	const CarplayElement* elements;
};

std::string createBeaconForCarplay(const std::string& normal_conf) {
	/*
	const char bt_addr[MAX_PROPERTY_VALUE_SIZE]
	int read_bt_addr_state = property_get("persist.bluetooth.address", bt_addr);
	if (read_bt_addr_state <= 0) {
		return normal_conf;
	}
	wpa_printf(MSG_DEBUG, " get bluetooth address:%s", bt_addr);
	std::vector<char> bt_addr_vector;
	char* addr_bit = NULL
	do {
		char* addr_bit = std::strtok(bt_addr,":");
		bt_addr_vector.push_back(*addr_bit);
		//TODO should check bt address big-endian/little-endian
	} while(addr_bit != NULL)
	*/

	CarplayOverallElement overall_elements;
	memset(&overall_elements, 0, sizeof(overall_elements));

    const uint8_t apple_oui[3] = {0x00, 0xA0, 0x40};

	overall_elements.id = 0xDD;
	overall_elements.OUI = apple_oui;
	overall_elements.sub_type = 0x00;

	CarplayElement elements[CARPLAY_ELEMENT_SIZE];
	int payload_size = 0;
	int element_default_len = 2;
	for(int i = 0; i < CARPLAY_ELEMENT_SIZE; i++) {
		CarplayElement element;
		vector<uint8_t> payload;
		memset(&element, 0, sizeof(element));
		switch (i) 
		{
			case CARPLAY_ELEMENT_ID_FLAGS:
			element.id = CARPLAY_ELEMENT_ID_FLAGS;
			payload = {0x00, 0x40};
			element.payload = payload.data();
			element.len = payload.size();
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_NAME:
			element.id = CARPLAY_ELEMENT_ID_NAME;
			payload = {0x46, 0x6f, 0x72, 0x64};
			element.payload = payload.data();
			element.len = payload.size();
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_MANUFACTURER:
			element.id = CARPLAY_ELEMENT_ID_MANUFACTURER;
			payload = {0x59, 0x61, 0x6e, 0x66, 0x65, 0x6e, 0x67};
			element.payload = payload.data();
			element.len = payload.size();
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_MODEL:
			element.id = CARPLAY_ELEMENT_ID_MODEL;
			payload = {0x50, 0x55, 0x31, 0x37, 0x32, 0x33, 0x55, 0x41};
			element.len = payload.size();
			element.payload = payload.data();
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_OUI:
			element.id = CARPLAY_ELEMENT_ID_OUI;
			element.len = payload.size();
			element.payload = apple_oui;
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_BLUETOOTH_MAC:
			element.id = CARPLAY_ELEMENT_ID_BLUETOOTH_MAC+1;
			payload = {0x50, 0x55, 0x31, 0x37, 0x32, 0x33};
			element.len = payload.size();
			element.payload = payload.data();
			payload_size += (payload.size() + element_default_len);
			break;
			case CARPLAY_ELEMENT_ID_DEVICE_ID:
			element.id = CARPLAY_ELEMENT_ID_DEVICE_ID+1;
			payload = {0x50, 0x55, 0x31, 0x37, 0x32, 0x33};
			element.len = payload.size();
			element.payload = payload.data();
			payload_size += (payload.size() + element_default_len);
			break;
			default:
			break;
		}
		elements[i] = element;
	}
	overall_elements.len = payload_size;
	overall_elements.elements = elements;
}

/**
* split a string
*/
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


/***
 * 对结构体中指针变量进行赋值。
 * 如上
 * 
*/
void cal_struct_size() {
    Element element;
    const char* welcome = "Hello_World_pp";
    //strncpy(element.name, welcome, 11);
    
    /**
     * 可以用vector作为容器
     * 

    vector<uint8_t> value = {0x11, 0x22, 0x33};
    element.payload = value.data();
    */
   
   /***
    * 注意 Java中 数组和 C/C++中的用法不一样。此处需要注意。
    * Java 
    * int[] b = new int[8];
    * 
    * C/C++
    * int value[8];
    * 
    const uint8_t value_2[] = {0x55, 0x66, 0x45};
    element.payload = value_2;
    */

    cout << "payload:" << element.payload[1] << " sizeof:" << sizeof(element)<< endl;
}


int main() {
    createBeaconForCarplay();
    return 0;
}



