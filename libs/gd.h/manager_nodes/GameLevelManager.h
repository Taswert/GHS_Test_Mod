#ifndef __GAMELEVELMANAGER_H__
#define __GAMELEVELMANAGER_H__

#include <gd.h>

namespace gd {

class GJGameLevel;

class GameLevelManager : public cocos2d::CCNode {
    protected:

    public:
        inline static GameLevelManager* sharedState() {
            return reinterpret_cast<GameLevelManager*(__stdcall*)()>( gd::base + 0x55190)(); //1.91
        }

        inline static gd::GJGameLevel* createNewLevel() {
            return reinterpret_cast<gd::GJGameLevel*(__stdcall*)()>( gd::base + 0xa0db0 )(); //2.1
        }

        cocos2d::CCArray* getSavedLevels() {
            return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(GameLevelManager*)>( gd::base + 0x56f80)(this);
        }

        virtual void levelUpdate(GJGameLevel* level) {
            return reinterpret_cast<void(__fastcall*)(GJGameLevel*)>(gd::base + 0x5ae70)(level);
        }
};

}

#endif
