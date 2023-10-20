#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include <gd.h>

namespace gd
{
	class LevelBrowserLayer : public cocos2d::CCLayer {
	public:
		static auto create(GJSearchObject* obj) {
			return reinterpret_cast<LevelBrowserLayer * (__fastcall*)(GJSearchObject*)>(base + 0x88a60)(obj); //0x894f0 GD 1.92
		}
	};
}

#endif