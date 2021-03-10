#include <ble_lib.h>
#include <windows.h>
#include <Windows.Foundation.h>
#include <Windows.Devices.Bluetooth.Advertisement.h>
#include <wrl/wrappers/corewrappers.h>
#include <cstdio>
#include <cstdlib>



char _hex(uint8_t v){
	return v+(v<10?48:87);
}



void _init_lib_cpp(void){
	CoInitializeSecurity(nullptr,-1,nullptr,nullptr,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IDENTIFY,NULL,EOAC_NONE,nullptr);
}



ble_device_list_t* _enum_dev_cpp(uint32_t tm){
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ ble_w=ref new Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();
	ble_device_list_t* _dl=(ble_device_list_t*)malloc(sizeof(ble_device_list_t));
	ble_device_list_t** dl=&_dl;
	_dl->l=0;
	ble_w->ScanningMode=Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode::Active;
	ble_w->Received+=ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^>([=](Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ w,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^ ea){
		for (uint64_t i=0;i<(*dl)->l;i++){
			if ((*dl)->dt[i].addr==ea->BluetoothAddress){
				return;
			}
		}
		(*dl)->l++;
		*dl=(ble_device_list_t*)realloc(*dl,sizeof(ble_device_list_t)+(*dl)->l*sizeof(ble_device_t));
		(*dl)->dt[(*dl)->l-1].addr=ea->BluetoothAddress;
		(*dl)->dt[(*dl)->l-1].addr_s[0]=_hex((uint8_t)(ea->BluetoothAddress>>44));
		(*dl)->dt[(*dl)->l-1].addr_s[1]=_hex((uint8_t)((ea->BluetoothAddress>>40)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[2]=':';
		(*dl)->dt[(*dl)->l-1].addr_s[3]=_hex((uint8_t)((ea->BluetoothAddress>>36)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[4]=_hex((uint8_t)((ea->BluetoothAddress>>32)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[5]=':';
		(*dl)->dt[(*dl)->l-1].addr_s[6]=_hex((uint8_t)((ea->BluetoothAddress>>28)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[7]=_hex((uint8_t)((ea->BluetoothAddress>>24)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[8]=':';
		(*dl)->dt[(*dl)->l-1].addr_s[9]=_hex((uint8_t)((ea->BluetoothAddress>>20)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[10]=_hex((uint8_t)((ea->BluetoothAddress>>16)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[11]=':';
		(*dl)->dt[(*dl)->l-1].addr_s[12]=_hex((uint8_t)((ea->BluetoothAddress>>12)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[13]=_hex((uint8_t)((ea->BluetoothAddress>>8)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[14]=':';
		(*dl)->dt[(*dl)->l-1].addr_s[15]=_hex((uint8_t)((ea->BluetoothAddress>>4)&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[16]=_hex((uint8_t)(ea->BluetoothAddress&0xf));
		(*dl)->dt[(*dl)->l-1].addr_s[17]=0;
	});
	ble_w->Start();
	Sleep(tm);
	ble_w->Stop();
	return _dl;
}



extern "C" void ble_lib_init(void){
	_init_lib_cpp();
}



extern "C" ble_device_list_t* ble_lib_enum_devices(uint32_t tm){
	return _enum_dev_cpp(tm);
}



extern "C" void ble_lib_free_device_list(ble_device_list_t* l){
	free(l);
}
