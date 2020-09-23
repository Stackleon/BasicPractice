#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>

using namespace std;


#define ELEMENT_SIZE 3
#define CARPLAY_ELEMENT_ID_FLAG 0x00
#define CARPLAY_ELEMENT_ID_NAME 0x01
#define CARPLAY_ELEMENT_ID_MANUFACTURE 0x02



struct Element {
    const uint8_t* payload;
    uint8_t len;
    uint8_t id;
};

struct CarplayIE {
    uint8_t id;
    int len;
    const uint8_t* oui;
    uint8_t sub_type;
    const Element* elements;
};

static uint8_t OUI[3] = {0x00, 0xA0, 0x40};

/**
 * 在使用uint8_t 作为长度的时候。通过cout无法输出长度，需要调查。
 * 
 * 
*/
void putCarplayBeacon() {
    CarplayIE carplayIE;
    memset(&carplayIE, 0 ,sizeof(carplayIE));
    carplayIE.id = 0xDD;
    carplayIE.oui = OUI;
    carplayIE.sub_type = 0x00;

    Element elements_p[ELEMENT_SIZE];
    int len = 0;
    for(int i = 0; i < ELEMENT_SIZE; i++) {
        Element element;
        memset(&element, 0, sizeof(element));
        vector<uint8_t> payload;
        switch (i)
        {
        case CARPLAY_ELEMENT_ID_FLAG:
            element.id = CARPLAY_ELEMENT_ID_FLAG;
            payload = {0x00, 0x40};
            element.payload = payload.data();
            element.len = payload.size();
            len += (element.len + 2);
            break;
        case CARPLAY_ELEMENT_ID_NAME:
            element.id = CARPLAY_ELEMENT_ID_FLAG;
            payload = {0x46, 0x6f, 0x72, 0x64};
            element.payload = payload.data();
            element.len = payload.size();
            len += (element.len + 2);
            break;
        case CARPLAY_ELEMENT_ID_MANUFACTURE:
            element.id = CARPLAY_ELEMENT_ID_FLAG;
            payload = {0x59, 0x61, 0x6e, 0x66, 0x65, 0x6e, 0x67};
            element.payload = payload.data();
            element.len = payload.size();
            len += (element.len + 2);
            break;    
        default:
            break;
        }
        elements_p[i] = element;
    }
    carplayIE.elements = elements_p;
    carplayIE.len = len;
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
    putCarplayBeacon();
    return 0;
}



