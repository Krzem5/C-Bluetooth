#include <ble_lib.h>
#include <stdio.h>



void* cb(ble_device_t* dv){
	printf("BLE Device:\n  Address: %s\n  Manufacturer Data (%u)%c\n",dv->addr_s,dv->manufacturer_data.l,(dv->manufacturer_data.l?':':' '));
	for (uint32_t j=0;j<dv->manufacturer_data.l;j++){
		printf("    (%u) ",(dv->manufacturer_data.data+j)->l);
		for (uint32_t k=0;k<(dv->manufacturer_data.data+j)->l;k++){
			printf("%.2hhx",*((dv->manufacturer_data.data+j)->dt+k));
		}
		putchar('\n');
	}
	printf("  Service UUIDs (%u)%c\n",dv->services.l,(dv->services.l?':':' '));
	for (uint32_t j=0;j<dv->services.l;j++){
		printf("    %s\n",(dv->services.uuids+j)->s);
	}
	if (dv->addr_s[0]=='0'){
		return (void*)dv;
	}
	ble_lib_free_device(dv);
	return NULL;
}



int main(int argc,const char** argv){
	(void)argc;
	(void)argv;
	ble_lib_init();
	ble_device_t* dv=(ble_device_t*)ble_lib_enum_devices(5000,cb);
	ble_connected_device_t* cdv=ble_lib_connect_device(dv);
	ble_lib_disconnect_device(cdv);
	ble_lib_free_device(dv);
	return 0;
}
