#include "pch.h"
#include "lvl-share.hpp"
//#include <matdash.hpp>
//#include <matdash/minhook.hpp>
#include "gd.h"
#include "utils.hpp"
#include <fstream>
#include "matplist.hpp"
#include "nfd.h"
#include "cocos2d.h"

/*
template <class S>
void dump_level(gd::GJGameLevel* level, S& stream) {
	const auto song_key = level->m_songID ?
		format("<k>k45</k><i>{}</i>", level->m_songID) :
		level->m_audioTrack ?
		format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";
	// encode it twice because gdshare does too
	// why? i dont know fod is stupid
	const auto encoded_desc = base64_encode(base64_encode(level->m_levelDesc.sv()));
	format_to(stream, R"(<d>
	<k>kCEK</k><i>4</i>
	<k>k2</k><s>{}</s>
	<k>k3</k><s>{}</s>
	<k>k4</k><s>{}</s>
	{}
	<k>k13</k><t/>
	<k>k21</k><i>2</i>
	<k>k50</k><i>24</i>
</d>)", level->m_levelName.sv(), encoded_desc, level->m_levelString.sv(), song_key);
}

template <class S>
gd::GJGameLevel* import_level(S& stream) {
	auto data = matplist::parse(stream);
	auto level = gd::GameLevelManager::sharedState()->createNewLevel();
	for (const auto& [key, var] : data) {
		if (!std::holds_alternative<matplist::Value>(var)) continue;
		matplist::Value value = std::get<matplist::Value>(var);
		using namespace std::literals;
		if (key == "k2"sv) {
			level->m_levelName = value.value;
		}
		else if (key == "k3"sv) {
			level->m_levelDesc = base64_decode(base64_decode(value.value));
		}
		else if (key == "k4"sv) {
			level->m_levelString = value.value;
		}
		else if (key == "k8"sv) {
			const auto text = value.value;
			level->m_audioTrack = std::stoi(text);
		}
		else if (key == "k45"sv) {
			const auto text = value.value;
			level->m_songID = std::stoi(text);
		}
	}
	return level;
}

bool(__thiscall* EditLevelLayer_init)(gd::EditLevelLayer* self, gd::GJGameLevel* level);
bool __fastcall EditLevelLayer_init_H(gd::EditLevelLayer* self, void* edx, gd::GJGameLevel* level) {
	if (!EditLevelLayer_init(self, level)) return false;
	auto menu = CCMenu::create();
	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto* const level = reinterpret_cast<gd::EditLevelLayer*>(self)->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			gd::FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 320.f, false, 0)->show();
		}
	};
	auto button = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("export", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
		nullptr, self, to_handler<SEL_MenuHandler, handler>
	);
	menu->setZOrder(1);
	menu->setPosition(35, 69);
	menu->addChild(button);
	self->addChild(menu);
	return true;
}

//GJSearchObject
//CCMenuItemSpriteExtra
//ButtonSprite
bool(__thiscall* LevelBrowserLayer_init)(gd::LevelBrowserLayer* self, gd::GJSearchObject* obj);
bool __fastcall LevelBrowserLayer_init_H(gd::LevelBrowserLayer* self, void* edx, gd::GJSearchObject* obj) {
	if (!LevelBrowserLayer_init(self, obj)) return false;

	if (obj->m_type == gd::SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(cocos2d::CCDirector::sharedDirector()->getWinSize().width - 30.f, 85);
		self->addChild(menu);
		constexpr auto handler = [](CCObject*, CCObject*) {
			nfdchar_t* path = nullptr;
			if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
				std::ifstream file(path);
				auto* const level = import_level(file);
				free(path);
				if (!level) {
					gd::FLAlertLayer::create(nullptr, "Error", "Failed to import", "OK", nullptr, 320.f, false, 0)->show();
					return;
				}
				auto scene = gd::EditLevelLayer::scene(level);
				cocos2d::CCDirector::sharedDirector()->pushScene(scene);
			}
		};

		auto button = gd::CCMenuItemSpriteExtra::create(
			gd::ButtonSprite::create("import", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
			nullptr, self, to_handler<SEL_MenuHandler, handler>
		);
		menu->setZOrder(1);
		menu->addChild(button);
	}

	return true;
}

void lvl_share::init() {

	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x3b320), //0x3b5a0 - GD 1.92 
		reinterpret_cast<void**>(&EditLevelLayer_init_H),
		reinterpret_cast<void**>(&EditLevelLayer_init));

	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x88b00), //0x89590 - GD 1.92
		reinterpret_cast<void**>(&LevelBrowserLayer_init_H),
		reinterpret_cast<void**>(&LevelBrowserLayer_init));
}
*/
