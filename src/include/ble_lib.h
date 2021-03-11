#ifndef __BLE_LIB_H__
#define __BLE_LIB_H__ 1
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif



typedef struct __BLE_GUID{
	uint64_t a;
	uint64_t b;
	char s[37];
} ble_guid_t;



typedef struct __BLE_DEVICE{
	uint64_t addr;
	char addr_s[18];
	uint32_t s_uuid_l;
	ble_guid_t* s_uuid;
} ble_device_t;



#pragma warning(push)
#pragma warning(disable:4200)
typedef struct __BLE_DEVICE_LIST{
	uint64_t l;
	ble_device_t dt[];
} ble_device_list_t;
#pragma warning(pop)



void ble_lib_init(void);



ble_device_list_t* ble_lib_enum_devices(uint32_t tm);



void ble_lib_print_guid(ble_guid_t g);



void ble_lib_free_device_list(ble_device_list_t* l);



#ifdef __cplusplus
}
#endif
#endif
