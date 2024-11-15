#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>

namespace gd {

class EditButtonBar : public cocos2d::CCNode {};
class GJRotationControl : public cocos2d::CCLayer {};
class GJScaleControl : public cocos2d::CCLayer {};

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class Slider;

class EditorUI : public cocos2d::CCLayer {
public:
    auto pasteObjects(const std::string& str) {
        return reinterpret_cast<cocos2d::CCArray * (__thiscall*)(EditorUI*, gd::string)>(base + 0x492a0)(this, str);
    }

    auto& clipboard() {
        return from<gd::string>(this, 0x264);
    }

    void updateButtons() {
        reinterpret_cast<void(__thiscall*)(EditorUI*)>(base + 0x41290)(this);
    }

    bool isPlayback() {
        return from<bool>(this, 0x134);
    }

    void updateZoom(float amt) {
        reinterpret_cast<void(__vectorcall*)(float, float, EditorUI*)>(base + 0x48c30)(0.f, amt, this);
    }
    
    LevelEditorLayer* getLevelEditorLayer() {
        return from<LevelEditorLayer*>(this, 0xAC);
    }

    void selectObjects(cocos2d::CCArray* arr) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, cocos2d::CCArray*)>(base + 0x47e80)(this, arr); //0xf1f20 - GHS 1.92
    }

    void selectObject(gd::GameObject* obj) {
        return reinterpret_cast<void(__thiscall*)(EditorUI*, gd::GameObject*)>(base + 0x47df0)(this, obj);
    }

    cocos2d::CCArray* getSomeObjects() {
        return from<cocos2d::CCArray*>(this, 0x1E8);
    }

    cocos2d::CCArray* getSelectedObjectsOfCCArray() {
        auto output = cocos2d::CCArray::create();
        gd::GameObject* single = from<gd::GameObject*>(this, 0x258);
        if (single)
        {
            output->addObject(reinterpret_cast<cocos2d::CCObject*>(single));
            return output;
        }
        return from<cocos2d::CCArray*>(this, 0x18c);
    }

    cocos2d::CCArray* getAllObjects() {
        return from<cocos2d::CCArray*>(this, 0x224); //no
    }

    std::vector<GameObject*> getSelectedObjects() {
        const auto single = from<GameObject*>(this, 0x258);
        if (single) return { single };
        const auto selectedArr = from<cocos2d::CCArray*>(this, 0x18c);
        if (!selectedArr) return {};
        std::vector<GameObject*> output;
        for (size_t i = 0; i < selectedArr->count(); ++i)
            output.push_back(reinterpret_cast<GameObject*>(selectedArr->objectAtIndex(i)));
        return output;
    }

    gd::GameObject* getSingleSelectedObj() {
        return from<gd::GameObject*>(this, 0x258);
    }
    
};
/*
class EditorUI : public cocos2d::CCLayer,
    public gd::FLAlertLayerProtocol,
    public gd::ColorSelectDelegate,
    public gd::GJRotationControlDelegate,
    public gd::GJScaleControlDelegate,
    public gd::MusicDownloadDelegate {

    public:
        EditButtonBar* m_pEditButtonBar;
        PAD(0x4)
        cocos2d::CCArray* m_pUnknownArray;
        PAD(0x28)
        cocos2d::CCLabelBMFont* m_pUnknownLabel;
        GJRotationControl* m_pRotationControl;
        PAD(0x10)
        GJScaleControl* m_pScaleControl;
        cocos2d::CCDictionary* m_pUnknownDict;
        EditButtonBar* m_pEditButtonBar2;
        EditButtonBar* m_pEditButtonBar3;
        Slider* m_pPositionSlider;
        PAD(0x20)
        cocos2d::CCArray* m_pUnknownArray2;
        PAD(0x8)
        cocos2d::CCArray* m_pUnknownArray3;
        cocos2d::CCMenu* m_pUnknownMenu;
        cocos2d::CCArray* m_pUnknownArray4;
        CCMenuItemSpriteExtra* m_pButton0;
        CCMenuItemSpriteExtra* m_pButton1;
        CCMenuItemSpriteExtra* m_pButton2;
        CCMenuItemSpriteExtra* m_pButton3;
        CCMenuItemSpriteExtra* m_pButton4;
        CCMenuItemSpriteExtra* m_pButton5;
        CCMenuItemSpriteExtra* m_pButton6;
        CCMenuItemSpriteExtra* m_pButton7;
        CCMenuItemSpriteExtra* m_pButton8;
        CCMenuItemSpriteExtra* m_pButton9;
        CCMenuItemSpriteExtra* m_pButton10;
        CCMenuItemSpriteExtra* m_pButton11;
        CCMenuItemSpriteExtra* m_pButton12;
        CCMenuItemSpriteExtra* m_pButton13;
        CCMenuItemSpriteExtra* m_pButton14;
        CCMenuItemSpriteExtra* m_pButton15;
        CCMenuItemSpriteExtra* m_pButton16;
        CCMenuItemSpriteExtra* m_pButton17;
        CCMenuItemSpriteExtra* m_pButton18;
        CCMenuItemSpriteExtra* m_pButton19;
        CCMenuItemSpriteExtra* m_pButton20;
        CCMenuItemSpriteExtra* m_pButton21;
        CCMenuItemSpriteExtra* m_pButton22;
        CCMenuItemSpriteExtra* m_pButton23;
        CCMenuItemSpriteExtra* m_pButton24;
        CCMenuItemSpriteExtra* m_pButton25;
        CCMenuItemSpriteExtra* m_pButton26;
        CCMenuItemToggler* m_pUnknownToggler;
        cocos2d::CCArray* m_pUnknownArray5;
        cocos2d::CCMenu* m_pUnknownMenu2;
        cocos2d::CCArray* m_pUnknownArray6;
        cocos2d::CCSprite* m_pIdkSprite0;
        cocos2d::CCSprite* m_pIdkSprite1;
        CCMenuItemSpriteExtra* m_pButton27;
        CCMenuItemSpriteExtra* m_pButton28;
        CCMenuItemSpriteExtra* m_pButton29;
        CCMenuItemSpriteExtra* m_pButton30;
        CCMenuItemSpriteExtra* m_pButton31;
        CCMenuItemSpriteExtra* m_pButton32;
        cocos2d::CCLabelBMFont* m_pCurrentLayerLabel;
        CCMenuItemSpriteExtra* m_pButton33;
        CCMenuItemSpriteExtra* m_pButton34;
        CCMenuItemSpriteExtra* m_pButton35;
        PAD(0xc)
        cocos2d::CCArray* m_pUnknownArray7;
        cocos2d::CCArray* m_pUnknownArray8;
        cocos2d::CCArray* m_pUnknownArray9;
        PAD(0x4)
        LevelEditorLayer* m_pEditorLayer;
    
    public:
        cocos2d::CCArray* pasteObjects(std::string const& _str) {
            auto ret = reinterpret_cast<cocos2d::CCArray*(__thiscall*)(
                EditorUI*, std::string
            )>(
                base + 0x88240
            )(
                this, _str
            );

            return ret;
        }

        void deselectAll() {
            reinterpret_cast<void(__fastcall*)(EditorUI*)>(
                base + 0x86af0
            )(this);
        }

        void selectObjects(cocos2d::CCArray* objs, bool idk) {
            reinterpret_cast<void(__thiscall*)(
                EditorUI*, cocos2d::CCArray*, bool
            )>(
                base + 0x864a0
            )(
                this, objs, idk
            );
        }

        cocos2d::CCArray* getSelectedObjects() {
            return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(EditorUI*)>(
                base + 0x86900
            )(this);
        }
};
*/

}

#endif
