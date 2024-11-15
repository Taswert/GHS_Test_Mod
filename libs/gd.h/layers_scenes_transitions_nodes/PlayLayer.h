#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;

	class PlayLayer : public GJBaseGameLayer {
	public:
		void resetLevel() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf03f0)(this); //0xf1f20 - GHS 1.92
		}
		void destroyPlayer(PlayerObject* selfig) {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*, PlayerObject*)>(base + 0xee990)(this, selfig);
		}
		void resume() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1e10)(this);
		}
		void onQuit() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0xf1fe0)(this);
		}

		auto layer() {
			return from<cocos2d::CCLayer*>(this, 0x2D4);
		}

		auto getFirstVisibleSection() {
			return from<int>(this, 0x1BC);
		}
		auto getLastVisibleSection() {
			return from<int>(this, 0x1C0);
		}

		auto getSections() {
			return from<cocos2d::CCArray*>(this, 0x170);
		}

		auto getObjects() {
			return from<cocos2d::CCArray*>(this, 0x184);
		}

		auto getLevelSettings() {
			return from <gd::LevelSettingsObject*>(this, 0x14C);
		}

		auto getStartPosObject() {
			return from <gd::StartPosObject*>(this, 0x148);
		}
		void setStartPosObject(gd::StartPosObject* startPos) {
			from <gd::StartPosObject*>(this, 0x148) = startPos;
		}

		auto getPlayerStartPosition() {
			return from<cocos2d::CCPoint>(this, 0x2E0);
		}
		void setPlayerStartPosition(cocos2d::CCPoint playerStartPosition) {
			from<cocos2d::CCPoint>(this, 0x2E0) = playerStartPosition;
		}


		/*auto getScene() {
			return from<cocos2d::CCScene*>(this, 0xa8);
		}*/
		// TODO: make these actual members
		auto player1() {
			return from<PlayerObject*>(this, 0x2a4); //2a4
		}
		auto player2() {
			return from<PlayerObject*>(this, 0x2a8); //2a8
		}
		auto levelLength() {
			return from<float>(this, 0x1d0); //1d0
		}
		auto attemptsLabel() {
			return from<cocos2d::CCLabelBMFont*>(this, 0x1d8); //1d8
		}
		auto attemptsCount() {
			return from<int>(this, 0x2e8); 
		}
		auto jumpsCount() {
			return from<int>(this, 0x2ec); 
		}
		auto getStartPos() {
			return from<cocos2d::CCPoint*>(this, 0x2e0);
		}
		auto getLastRunPercent() {
			return from<int>(this, 0x2fc);
		}
		auto getClkTimer() {
			return from<float>(this, 0x2f4);
		}
		void setAttempts(int atts) {
			from<int>(this, 0x2e8) = atts; 
		}
		auto getAudioEffectsLayer() {
			return from<gd::AudioEffectsLayer*>(this, 0x198);
		}
	};
}

#endif