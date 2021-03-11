#include <ble_lib.h>
#include <stdio.h>



int main(int argc,const char** argv){
	(void)argc;
	(void)argv;
	ble_lib_init();
	ble_device_list_t* l=ble_lib_enum_devices(5000);
	for (uint64_t i=0;i<l->l;i++){
		printf("BLE Device:\n  Address: %s\n  Service UUIDs (%u)%c\n",l->dt[i].addr_s,l->dt[i].s_uuid_l,(l->dt[i].s_uuid_l?':':' '));
		for (uint32_t j=0;j<l->dt[i].s_uuid_l;j++){
			printf("    %s\n",(l->dt[i].s_uuid+j)->s);
		}
	}
	ble_lib_free_device_list(l);
	return 0;
}
