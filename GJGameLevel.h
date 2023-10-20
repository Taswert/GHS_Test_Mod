#pragma once
#include "pch.h"

namespace GJGameLevel
{
	inline bool(__thiscall* GJGameLevel_init)(cocos2d::CCLayer* self, gd::GJGameLevel* idk);
	bool __fastcall GJGameLevel_init_hook(cocos2d::CCLayer* self, void* edx, gd::GJGameLevel* idk);

	

	class Callback
	{
	public:
		void passButton(CCObject*);
		void rateButton(CCObject*);
		void unrateButton(CCObject*);
	};
}

