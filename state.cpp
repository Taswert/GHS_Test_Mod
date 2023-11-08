#include "state.h"

SettingStruct& setting() {
	static SettingStruct inst;
	return inst;
}