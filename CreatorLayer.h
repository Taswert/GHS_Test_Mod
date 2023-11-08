#pragma once
#include "pch.h"

namespace CreatorLayer
{
	inline bool(__thiscall* CreatorLayer_init)(gd::CreatorLayer* self);
	bool __fastcall CreatorLayer_init_H(gd::CreatorLayer* self);

	inline bool(__thiscall* CreatorLayer_menu)();
	void __fastcall CreatorLayer_menu_H();

	inline bool(__thiscall* GJListLayer_editing_init)();
	void __fastcall GJListLayer_editing_init_H();

	class Callback
	{
	public:
		void onHallOfFame(CCObject*);
	};
}
