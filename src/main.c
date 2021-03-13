#include <ble_lib.h>
#include <stdio.h>



void* found_cb(ble_device_t* dv,void* arg){
	printf("BLE Device:\n  Address: %s\n  Manufacturer Data (%u)%c\n",dv->addr_s,dv->manufacturer_data.l,(dv->manufacturer_data.l?':':' '));
	for (uint32_t j=0;j<dv->manufacturer_data.l;j++){
		printf("    ManufacturerID: %u, Data: [%u] ",(dv->manufacturer_data.data+j)->id,(dv->manufacturer_data.data+j)->l);
		for (uint32_t k=0;k<(dv->manufacturer_data.data+j)->l;k++){
			printf("%.2hhx",*((dv->manufacturer_data.data+j)->dt+k));
		}
		putchar('\n');
	}
	printf("  Service UUIDs (%u)%c\n",dv->services.l,(dv->services.l?':':' '));
	for (uint32_t j=0;j<dv->services.l;j++){
		printf("    %s\n",(dv->services.uuids+j)->s);
	}
	if (dv->addr==0x1653b3c599||dv->addr==0x90842b5ace22){
		return (void*)dv;
	}
	ble_lib_free_device(dv);
	return NULL;
}



void read_cb(ble_characteristic_notification_data_t dt,void* arg){
	printf("Data (%llu): [%u] ",dt.tm,dt.l);
	for (uint32_t i=0;i<dt.l;i++){
		printf("%.2hhx",*(dt.bf+i));
	}
	putchar('\n');
}



int main(int argc,const char** argv){
	ble_lib_init();
	ble_device_t* dv=(ble_device_t*)ble_lib_enum_devices(5000,found_cb,NULL);
	if (!dv){
		printf("No Device Found!\n");
		return 0;
	}
	ble_connected_device_t* cdv=ble_lib_connect_device(dv);
	for (uint32_t i=0;i<cdv->services.l;i++){
		printf("Service#%lu: %s:\n",i,(cdv->services.data+i)->uuid.s);
		ble_lib_load_characteristics(cdv->services.data+i);
		for (uint32_t j=0;j<(cdv->services.data+i)->characteristics.l;j++){
			printf("  Characteristic#%lu: %s (Flags: %#.3x)\n",j,((cdv->services.data+i)->characteristics.data+j)->uuid.s,((cdv->services.data+i)->characteristics.data+j)->f);
			if (((cdv->services.data+i)->characteristics.data+j)->f&BLE_CHRACTERISTIC_FLAG_NOTIFY){
				ble_lib_register_characteristic_notification((cdv->services.data+i)->characteristics.data+j,read_cb,NULL);
			}
		}
	}
	getchar();
	ble_lib_disconnect_device(cdv);
	ble_lib_free_device(dv);
	return 0;
}
