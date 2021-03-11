#include <ble_lib.h>
#include <windows.h>
#include <Windows.Foundation.h>
#include <Windows.Devices.Bluetooth.h>
#include <Windows.Devices.Bluetooth.Advertisement.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl.h>
#include <robuffer.h>
#include <inspectable.h>
#include <cstdio>
#include <cstdlib>
#include <ppltasks.h>
#include <pplawait.h>



typedef struct ___CPP_BLE_CONNECTED_DEVICE{
	ble_device_t* dv;
	Windows::Devices::Bluetooth::BluetoothLEDevice^ _dt;
} _cpp_ble_connected_device_t;



char _hex(uint8_t v){
	return v+(v<10?48:87);
}



uint8_t _dehex(char c){
	return c-(c>96?87:48);
}



void _init_lib_cpp(void){
	CoInitializeSecurity(nullptr,-1,nullptr,nullptr,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IDENTIFY,NULL,EOAC_NONE,nullptr);
}



void* _enum_dev_cpp(uint32_t tm,ble_device_found_t cb){
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ ble_w=ref new Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();
	uint64_t* _al=(uint64_t*)(void*)0;
	uint64_t** al=&_al;
	uint64_t _all=0;
	uint64_t* all=&_all;
	void* _r=(void*)0;
	void** r=&_r;
	ble_w->ScanningMode=Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode::Active;
	ble_w->Received+=ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^>([=](Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher^ w,Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs^ ea){
		if (!ea->BluetoothAddress){
			return;
		}
		for (uint64_t i=0;i<*all;i++){
			if (*((*al)+i)==ea->BluetoothAddress){
				return;
			}
		}
		(*all)++;
		*al=(uint64_t*)realloc(*al,(*all)*sizeof(uint64_t));
		*((*al)+(*all)-1)=ea->BluetoothAddress;
		uint64_t tl=0;
		for (uint32_t i=0;i<ea->Advertisement->ManufacturerData->Size;i++){
			tl+=ea->Advertisement->ManufacturerData->GetAt(i)->Data->Length;
		}
		ble_device_t dv={
			ea->BluetoothAddress,
			{
				_hex((uint8_t)(ea->BluetoothAddress>>44)),
				_hex((uint8_t)((ea->BluetoothAddress>>40)&0xf)),
				':',
				_hex((uint8_t)((ea->BluetoothAddress>>36)&0xf)),
				_hex((uint8_t)((ea->BluetoothAddress>>32)&0xf)),
				':',
				_hex((uint8_t)((ea->BluetoothAddress>>28)&0xf)),
				_hex((uint8_t)((ea->BluetoothAddress>>24)&0xf)),
				':',
				_hex((uint8_t)((ea->BluetoothAddress>>20)&0xf)),
				_hex((uint8_t)((ea->BluetoothAddress>>16)&0xf)),
				':',
				_hex((uint8_t)((ea->BluetoothAddress>>12)&0xf)),
				_hex((uint8_t)((ea->BluetoothAddress>>8)&0xf)),
				':',
				_hex((uint8_t)((ea->BluetoothAddress>>4)&0xf)),
				_hex((uint8_t)(ea->BluetoothAddress&0xf)),
				0
			},
			{
				ea->Advertisement->ManufacturerData->Size,
				(ea->Advertisement->ManufacturerData->Size?(ble_device_manufacturer_data_t*)malloc(ea->Advertisement->ManufacturerData->Size*sizeof(ble_device_manufacturer_data_t)+tl*sizeof(uint8_t)):(ble_device_manufacturer_data_t*)(void*)0)
			},
			{
				ea->Advertisement->ServiceUuids->Size,
				(ea->Advertisement->ServiceUuids->Size?(ble_guid_t*)malloc(ea->Advertisement->ServiceUuids->Size*sizeof(ble_guid_t)):(ble_guid_t*)(void*)0)
			}
		};
		ble_device_manufacturer_data_t* mdp=dv.manufacturer_data.data;
		uint8_t* mdpp=(uint8_t*)(void*)((uint64_t)(void*)dv.manufacturer_data.data+ea->Advertisement->ManufacturerData->Size*sizeof(ble_device_manufacturer_data_t));
		ble_guid_t* glp=dv.services.uuids;
		for (uint32_t i=0;i<ea->Advertisement->ManufacturerData->Size;i++){
			mdp->l=ea->Advertisement->ManufacturerData->GetAt(i)->Data->Length;
			Microsoft::WRL::ComPtr<Windows::Storage::Streams::IBufferByteAccess> dt;
			reinterpret_cast<IInspectable*>(ea->Advertisement->ManufacturerData->GetAt(i)->Data)->QueryInterface(IID_PPV_ARGS(&dt));
			mdp->dt=mdpp;
			dt->Buffer((byte**)&(mdp->dt));
			mdp++;
			mdpp+=mdp->l;
		}
		for (uint32_t i=0;i<ea->Advertisement->ServiceUuids->Size;i++){
			Platform::String^ s=ea->Advertisement->ServiceUuids->GetAt(i).ToString();
			const char16* dt=s->Data();
			if (s->Length()!=38){
				printf("WRONG GUID LENGTH: %lu\n",s->Length());
			}
			else{
				uint32_t j=1;
				while (j<37){
					glp->s[j-1]=(char)(*(dt+j));
					if (j==9||j==14||j==19||j==24){
						j++;
						continue;
					}
					if (j<19){
						glp->a=(glp->a<<4)|_dehex((uint8_t)(char)(*(dt+j)));
					}
					else{
						glp->b=(glp->b<<4)|_dehex((uint8_t)(char)(*(dt+j)));
					}
					j++;
				}
				glp->s[36]=0;
				glp++;
			}
		}
		if ((*r=cb(&dv))){
			ble_w->Stop();
		}
	});
	ble_w->Start();
	while (tm){
		tm-=100;
		Sleep(100);
		if (ble_w->Status!=Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus::Started){
			return *r;
		}
	}
	ble_w->Stop();
	return *r;
}



concurrency::task<void> _connect_dev_cpp(ble_device_t* dv,ble_connected_device_t** o){
	Windows::Devices::Bluetooth::BluetoothLEDevice^ d=co_await Windows::Devices::Bluetooth::BluetoothLEDevice::FromBluetoothAddressAsync(dv->addr);
	_cpp_ble_connected_device_t* cdv=(_cpp_ble_connected_device_t*)malloc(sizeof(_cpp_ble_connected_device_t));
	cdv->dv=dv;
	cdv->_dt=d;
	*o=(ble_connected_device_t*)cdv;
}



ble_connected_device_t* _connect_dev_wr_cpp(ble_device_t* dv){
	ble_connected_device_t* o;
	_connect_dev_cpp(dv,&o);
	return o;
}



void _diconnect_dev_cpp(ble_connected_device_t* cdv){
	_cpp_ble_connected_device_t* cdv_cpp=(_cpp_ble_connected_device_t*)cdv;
	cdv_cpp->_dt=nullptr;
	free(cdv_cpp);
}



extern "C" void ble_lib_init(void){
	_init_lib_cpp();
}



extern "C" void* ble_lib_enum_devices(uint32_t tm,ble_device_found_t cb){
	return _enum_dev_cpp(tm,cb);
}



extern "C" ble_connected_device_t* ble_lib_connect_device(ble_device_t* dv){
	return _connect_dev_wr_cpp(dv);
}



extern "C" void ble_lib_disconnect_device(ble_connected_device_t* cdv){
	_diconnect_dev_cpp(cdv);
}



extern "C" void ble_lib_free_device(ble_device_t* dv){
	if (dv->manufacturer_data.l){
		free(dv->manufacturer_data.data);
	}
	if (dv->services.l){
		free(dv->services.uuids);
	}
}
