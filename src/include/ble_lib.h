#ifndef __BLE_LIB_H__
#define __BLE_LIB_H__ 1
#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif



typedef struct __BLE_DEVICE{
	uint64_t addr;
	char addr_s[18];
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



void ble_lib_free_device_list(ble_device_list_t* l);



#ifdef __cplusplus
}
#endif
#endif
