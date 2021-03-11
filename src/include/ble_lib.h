#ifndef __BLE_LIB_H__
#define __BLE_LIB_H__ 1
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif



typedef struct __BLE_DEVICE_MANUFACTURER_DATA{
	uint32_t l;
	uint8_t* dt;
} ble_device_manufacturer_data_t;



typedef struct __BLE_DEVICE_MANUFACTURER_DATA_LIST{
	uint32_t l;
	ble_device_manufacturer_data_t* data;
} ble_device_manufacturer_data_list_t;



typedef struct __BLE_GUID{
	uint64_t a;
	uint64_t b;
	char s[37];
} ble_guid_t;



typedef struct __BLE_DEVICE_SERVICE_LIST{
	uint32_t l;
	ble_guid_t* uuids;
} ble_device_service_list_t;



typedef struct __BLE_DEVICE{
	uint64_t addr;
	char addr_s[18];
	ble_device_manufacturer_data_list_t manufacturer_data;
	ble_device_service_list_t services;
} ble_device_t;



typedef struct __BLE_CONNECTED_DEVICE{
	ble_device_t* dv;
} ble_connected_device_t;



typedef void* (*ble_device_found_t)(ble_device_t* dv);



void ble_lib_init(void);



void* ble_lib_enum_devices(uint32_t tm,ble_device_found_t cb);



ble_connected_device_t* ble_lib_connect_device(ble_device_t* dv);



void ble_lib_disconnect_device(ble_connected_device_t* cdv);



void ble_lib_free_device(ble_device_t* dv);



#ifdef __cplusplus
}
#endif
#endif
