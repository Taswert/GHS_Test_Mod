// dllmain.cpp : Defines the entry point for the DLL application.
#pragma once
#include "pch.h"
#include "PauseLayer.h"
#include "GJGameLevel.h"
#include "EditorPauseLayer.h"
#include "EditLevelLayer.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "menu.h"
#include "state.h"
#include "utils.hpp"
#include <fstream>
#include <fcntl.h>
#include <io.h>

//#include "PlayLayer.h"
std::time_t t;
SYSTEMTIME st;

float DeathTicks = 0;
float UpdateTicks = 0;
float NoclipAcc = 0;
float accuracy = 0;

bool isPlayerColliding = false;
bool isPlayerDead = false;
bool isPlayerColorGot = false;

bool isPressed = false;

int tick = 0;
int deathTick = 0;
double lastTime = 0;

ccColor3B playerColor1;
ccColor3B playerColor2;
ccColor3B playerColorG;

CCLayer* PauseLayerObject;
CCLayer* PlayLayerObject;
gd::EditorUI* editUI;

std::string savedClipboard;

DWORD cocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

//cocos2d::ccColor3B player1col, player2col;

bool(__thiscall* destroyPlayer)(CCNode* selfig);
void __fastcall destroyPlayer_H(CCNode* selfig)
{   
    isPlayerDead = true;
    destroyPlayer(selfig);
    
    
    if ((1 / cocos2d::CCDirector::sharedDirector()->getAnimationInterval()) < 60)
    {
        DeathTicks = DeathTicks - ((1 / cocos2d::CCDirector::sharedDirector()->getAnimationInterval()) / 60);
    }
    else 
    
    
    auto pl = gd::GameManager::sharedState()->getPlayLayer();
}

bool(__thiscall* restartLevel)(gd::PlayLayer* self);
void __fastcall restartLevel_H(gd::PlayLayer* self) {
    setting().beforeRestartCheatsCount = setting().cheatsCount;
    deathTick = 0;
    tick = 0;
    restartLevel(self);
}

bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(cocos2d::CCKeyboardDispatcher* self, int key, bool down);
void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSG_H(CCKeyboardDispatcher* self, void* edx, int key, bool down) {
    if (down) {
        if (key == 'R' && setting().onRetryBind) {
            auto pl = gd::GameManager::sharedState()->getPlayLayer();
            if (pl) {
                //here, should destroy pause smh
                //pl->removeMeAndCleanup();
                pl->resetLevel();
                if (PauseLayerObject)
                {
                    PauseLayerObject->removeMeAndCleanup();
                    PauseLayerObject = nullptr;
                }
                pl->resume();
                return;
            }
        }
    }
    CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
}

bool(__thiscall* PlayLayer_init)(gd::PlayLayer* self, gd::GJGameLevel* level);
bool __fastcall PlayLayer_init_H(gd::PlayLayer* self, void* edx, gd::GJGameLevel* level)
{
    PlayLayerObject = self;
    editUI = nullptr;
    PauseLayerObject = nullptr;
    isPlayerColorGot = false;
    deathTick = 0;
    tick = 0;
    if (!PlayLayer_init(self, level)) return false;
    auto size = CCDirector::sharedDirector()->getWinSize();
    lastTime = CCDirector::sharedDirector()->getTotalFrames() / CCDirector::sharedDirector()->getAnimationInterval();

    if (setting().onShowPercents)
    {
        const auto bar = gd::GameManager::sharedState()->getShowProgressBar();

        auto percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setScale(0.5f);
        percentLabel->setZOrder(5);
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });
        percentLabel->setTag(4571);
        self->addChild(percentLabel);
    }
    int labelCount = 0;

    if (setting().onCheatIndicator)
    {
        labelCount++;
        auto CheatIndicatorLabel = CCLabelBMFont::create(".", "bigFont.fnt");
        CheatIndicatorLabel->setScale(1.0f);
        CheatIndicatorLabel->setZOrder(5);
        CheatIndicatorLabel->setAnchorPoint({ 0.f, 0.f });
        CheatIndicatorLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        CheatIndicatorLabel->setTag(4572);
        //CheatIndicatorLabel->setAnchorPoint({ 0.5f, 0.5f });
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().CurrentNoclipByte, 1, 0);
        if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
        else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }
        if (setting().cheatsCount == 0 && !setting().onSafeMode) CheatIndicatorLabel->setColor({ 0, 255, 0 });
        else if (setting().cheatsCount == 0 && setting().onSafeMode) CheatIndicatorLabel->setColor({ 255, 255, 0 });
        else CheatIndicatorLabel->setColor({ 255, 0, 0 });

        //CheatIndicatorLabel->
        self->addChild(CheatIndicatorLabel);
    }
    
    if (setting().onMessage)
    {
        labelCount++;
        auto messageLabel = CCLabelBMFont::create(setting().message, "bigFont.fnt");
        messageLabel->setScale(0.40f);
        messageLabel->setZOrder(5);
        messageLabel->setAnchorPoint({ 0.f, 0.f });
        messageLabel->setOpacity(128);
        messageLabel->setTag(4577);
        messageLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(messageLabel);
    }
    
    if (setting().onFPSShow)
    {
        labelCount++;
        //int fps = 1 / CCDirector::sharedDirector()->getAnimationInterval();

        float fps = ImGui::GetIO().Framerate;
        auto FPSLabel = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        FPSLabel->setString(CCString::createWithFormat("FPS: %.f", fps)->getCString());
        FPSLabel->setScale(0.40f);
        FPSLabel->setZOrder(5);
        FPSLabel->setAnchorPoint({ 0.f, 0.f });
        FPSLabel->setOpacity(128);
        FPSLabel->setTag(4576);
        FPSLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(FPSLabel);
    }
    
    if (setting().onNoclipAcc)
    {
        labelCount++;
        auto noclipAccLabel = CCLabelBMFont::create("100.00%", "bigFont.fnt");
        noclipAccLabel->setString(CCString::createWithFormat("Time: %f", self->getClkTimer())->getCString());
        noclipAccLabel->setScale(0.40f);
        noclipAccLabel->setZOrder(5);
        noclipAccLabel->setAnchorPoint({ 0.f, 0.f });
        noclipAccLabel->setOpacity(128);
        noclipAccLabel->setTag(4578);
        noclipAccLabel->setPosition({ 6.f, size.height - 13.f * labelCount });
        self->addChild(noclipAccLabel);
    }
    
    if (setting().onGlobalTime)
    {
        labelCount++;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");

        auto GlobalTimeLabel = CCLabelBMFont::create(":", "bigFont.fnt");
        GlobalTimeLabel->setString(CCString::create(s.str().c_str())->getCString());
        GlobalTimeLabel->setScale(0.40f);
        GlobalTimeLabel->setZOrder(5);
        GlobalTimeLabel->setAnchorPoint({ 0.f, 0.f });
        GlobalTimeLabel->setOpacity(128);
        GlobalTimeLabel->setTag(4575);
        GlobalTimeLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(GlobalTimeLabel);
    }

    if (setting().onAttemptsLabel)
    {
        labelCount++;
        auto AttemptsLabel = CCLabelBMFont::create("Attempt 1", "bigFont.fnt");
        AttemptsLabel->setScale(0.40f);
        AttemptsLabel->setZOrder(5);
        AttemptsLabel->setAnchorPoint({ 0.f, 0.f });
        AttemptsLabel->setOpacity(128);
        AttemptsLabel->setTag(4573);
        AttemptsLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(AttemptsLabel);
    }

    if (setting().onJumpsLabel)
    {
        labelCount++;
        auto JumpsLabel = CCLabelBMFont::create("0 Jumps", "bigFont.fnt");
        JumpsLabel->setScale(0.40f);
        JumpsLabel->setZOrder(5);
        JumpsLabel->setAnchorPoint({ 0.f, 0.f });
        JumpsLabel->setOpacity(128);
        JumpsLabel->setTag(4574);
        JumpsLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
        self->addChild(JumpsLabel);
    }
    labelCount++;
    auto SafeModeLabel = CCLabelBMFont::create("Safe Mode", "bigFont.fnt");
    SafeModeLabel->setScale(0.40f);
    SafeModeLabel->setZOrder(5);
    SafeModeLabel->setAnchorPoint({ 0.f, 0.f });
    SafeModeLabel->setOpacity(128);
    SafeModeLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
    SafeModeLabel->setTag(45710);
    if (!(setting().onSafeMode && setting().onSafeModeLabel)) SafeModeLabel->setVisible(0);
    self->addChild(SafeModeLabel);
    /*
    labelCount++;
    auto deathLabel = CCLabelBMFont::create(" ", "bigFont.fnt");
    deathLabel->setString(CCString::createWithFormat("Dead - %d", isPlayerDead)->getCString());
    deathLabel->setScale(0.40f);
    deathLabel->setZOrder(5);
    deathLabel->setAnchorPoint({ 0.f, 0.f });
    deathLabel->setOpacity(128);
    deathLabel->setTag(4579);
    deathLabel->setPosition({ 5.f, size.height - 13.f * labelCount });
    self->addChild(deathLabel);
    */

    //this is absurd, but it works

    /*
    const char* test1 = "ayo";
    const char* icon = "checkpoint_01_001.png";
    auto achievement = gd::AchievementBar::create(test1, test1, icon);
    */
    
    //checkpoint_01_001.png

    ccDrawRect(CCPoint(0, 0), CCPoint(10, 10));

    auto rect = CCRect::CCRect();
    rect.setRect(150, 150, 100, 100);
    return true;
}

inline bool playerTouchesObject(CCRect* playerRect, CCRect* hitboxRect)
{
    float playerMaxX = playerRect->getMaxX();

}



bool(__thiscall* PlayLayer_update)(gd::PlayLayer* self, float dt);
void __fastcall PlayLayer_update_H(gd::PlayLayer* self, void*, float dt)
{
    PauseLayerObject = nullptr;
    PlayLayer_update(self, dt);
    auto size = CCDirector::sharedDirector()->getWinSize();
    float fps = ImGui::GetIO().Framerate;// = 1 / CCDirector::sharedDirector()->getAnimationInterval();
    float currentTime = CCDirector::sharedDirector()->getTotalFrames();
    const auto bar = gd::GameManager::sharedState()->getShowProgressBar();
    auto audioScale = self->player1()->getAudioScale() > 1.f ? 1.f : self->player1()->getAudioScale();
    
    auto percentLabel = self->getChildByTag(4571);
    auto CheatIndicatorLabel = self->getChildByTag(4572);
    auto AttemptsLabel = self->getChildByTag(4573);
    auto JumpsLabel = self->getChildByTag(4574);
    auto GlobalTimeLabel = self->getChildByTag(4575);
    auto FPSLabel = self->getChildByTag(4576);
    auto messageLabel = self->getChildByTag(4577);
    auto noclipAccLabel = self->getChildByTag(4578);
    auto deathLabel = self->getChildByTag(4579);
    

    

    if (percentLabel) {
        const auto value = self->player1()->getPositionX() / self->levelLength() * 100.f;

        percentLabel->setAnchorPoint({ bar ? 0.f : 0.5f, 0.5f });
        percentLabel->setPosition({ size.width / 2.f + (bar ? 107.2f : 0.f), size.height - 8.f });

        if (value < 100.0f) reinterpret_cast<CCLabelBMFont*>(percentLabel)->setString(CCString::createWithFormat("%.2f%%", value)->getCString());
        else reinterpret_cast<CCLabelBMFont*>(percentLabel)->setString(CCString::create("100.00%")->getCString());

    }
    
    //if (isPlayerDead)
    //{
    //    deathTick++;
    //    tick++;
    //}
    //else
    //{
    //    tick++;
    //}
    //isPlayerDead = false;
    /*
    if (noclipAccLabel)
    {
        accuracy = (tick / (tick - deathTick)) * 100;
        reinterpret_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("%.2f%%", accuracy)->getCString());
    }
    */
    
    if (noclipAccLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(noclipAccLabel)->setString(CCString::createWithFormat("Time: %f", self->getClkTimer())->getCString());
    }
    
    if (currentTime - lastTime >= 1.0)
    {
        fps = CCDirector::sharedDirector()->getAnimationInterval() / tick;
        tick = 0;
        lastTime += 1.0;
    }
    if (FPSLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(FPSLabel)->setString(CCString::createWithFormat("FPS: %.f", roundf(fps)/*CCDirector::sharedDirector()->getTotalFrames()*/)->getCString());
    }
    
    if (JumpsLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(JumpsLabel)->setString(CCString::createWithFormat(self->jumpsCount() == 1 ? "%d Jump" : "%d Jumps", self->jumpsCount())->getCString());
    }

    if (GlobalTimeLabel)
    {
        t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream s;
        s << std::put_time(&tm, "%H:%M:%S");
        reinterpret_cast<CCLabelBMFont*>(GlobalTimeLabel)->setString(CCString::create(s.str().c_str())->getCString());
    }

    if (AttemptsLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(AttemptsLabel)->setString(CCString::createWithFormat("Attempt %d", self->attemptsCount())->getCString());
    }

    if (messageLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(messageLabel)->setString(CCString::create(setting().message)->getCString());
    }

    if (deathLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(deathLabel)->setString(CCString::createWithFormat("Dead - %d", isPlayerDead)->getCString());
    }

    //self->destroyPlayer(self->player1());
    //some fun stuff
    /*
    self->attemptsLabel()->setPositionX(self->player1()->getPositionX());
    self->attemptsLabel()->setPositionY(self->player1()->getPositionY()+35);
    self->attemptsLabel()->setScale(0.75f);
    self->attemptsLabel()->setString(CCString::createWithFormat(self->attemptsCount() == 1 ? "I haven't died!" : self->attemptsCount() == 2 ? "Died %d time..." : "Died %d times...", self->attemptsCount()-1)->getCString());
    */
    

    /*
    auto rect = self->player1()->getTextureRect();
    auto rectP = &rect;

    if (gd::PlayerObject::collidedWithObject(rectP))
    {
        DeathTicks = DeathTicks + 4;
    }
    UpdateTicks = UpdateTicks + 4;


    if (UpdateTicksLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(UpdateTicksLabel)->setString(CCString::createWithFormat("Update Ticks: %.f", UpdateTicks)->getCString());
    }
    
    if (AliveTicksLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(AliveTicksLabel)->setString(CCString::createWithFormat("Alive Ticks: %.f", DeathTicks)->getCString());
    }
    

    NoclipAcc = DeathTicks;
    NoclipAcc = NoclipAcc / UpdateTicks * 100;

    if (NoclipAccLabel)
    {
        reinterpret_cast<CCLabelBMFont*>(NoclipAccLabel)->setString(CCString::createWithFormat("Noclip Accuracy: %.2f%%", NoclipAcc)->getCString());
    }
    */
    if (isPlayerColorGot == false)
    {
        playerColor1 = self->player1()->getFirstColor();
        playerColor2 = self->player1()->getSecondColor();
        playerColorG = self->player1()->getGlowColor();
        isPlayerColorGot = true;
    }
    if (setting().onDualSameCol)
    {
        self->player2()->setColor(self->player1()->getColor());
        self->player2()->setSecondColor(self->player1()->getSecondColor());

        self->player2()->setGlowColor(self->player1()->getGlowColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColorSecondVehicle());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getColorGlowVehicle());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getWaveTrailColor());
    }
    else
    {
        self->player2()->setColor(self->player1()->getSecondColor());
        self->player2()->setSecondColor(self->player1()->getColor());

        self->player2()->setGlowColor(self->player1()->getSecondColor());
        //self->player2()->setColor3(self->player1()->getColor3());

        self->player2()->setColorSecondVehicle(self->player1()->getColor());
        //self->player2()->setColor5(self->player1()->getColor5());
        self->player2()->setColorGlowVehicle(self->player1()->getSecondColor());
        //self->player2()->setColor7(self->player1()->getColor7());

        self->player2()->setWaveTrailColor(self->player1()->getSecondColor());
    }

    /*******************************************************************************/
    if (setting().onPrimaryPulse)
    {
        unsigned char
            RR = playerColor1.r + ((setting().PrimaryPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().PrimaryPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().PrimaryPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setColor({ RR, RG, RB });
        //PRIMARY COLOR
    }
    if (setting().onSecondaryPulse)
    {
        unsigned char
            RR = playerColor2.r + ((setting().SecondaryPulse[0] * 255 - playerColor2.r) * audioScale),
            RG = playerColor2.g + ((setting().SecondaryPulse[1] * 255 - playerColor2.g) * audioScale),
            RB = playerColor2.b + ((setting().SecondaryPulse[2] * 255 - playerColor2.b) * audioScale);
        self->player1()->setSecondColor({ RR, RG, RB });
        self->player1()->setColorSecondVehicle({ RR, RG, RB });
        //SECONDARY COLOR
    }
    
    if (setting().onGlowPulse)
    {
        unsigned char
            RR = playerColorG.r + ((setting().GlowPulse[0] * 255 - playerColorG.r) * audioScale),
            RG = playerColorG.g + ((setting().GlowPulse[1] * 255 - playerColorG.g) * audioScale),
            RB = playerColorG.b + ((setting().GlowPulse[2] * 255 - playerColorG.b) * audioScale);
        self->player1()->setGlowColor({ RR, RG, RB });
        self->player1()->setColorGlowVehicle({ RR, RG, RB });
        //GLOW COLOR
    }

    if (setting().onWaveTrailPulse) 
    {
        unsigned char
            RR = playerColor1.r + ((setting().WaveTrailPulse[0] * 255 - playerColor1.r) * audioScale),
            RG = playerColor1.g + ((setting().WaveTrailPulse[1] * 255 - playerColor1.g) * audioScale),
            RB = playerColor1.b + ((setting().WaveTrailPulse[2] * 255 - playerColor1.b) * audioScale);
        self->player1()->setWaveTrailColor({ RR, RG, RB });
        //WAVE TRAIL COLOR
    }
    
    /******************************************************************************/
    
    /*if (setting().onGlow) {
        self->player1()->getGlow()->setVisible(1);
        self->player1()->getGlowVehicle()->setVisible(1);
    }*/
    if (setting().onHidePlayer)
    {
        self->player1()->setVisible(0);
        self->player2()->setVisible(0);
    }


    
}

size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class AdminCheck
{
public:
    void adminCheckCallback(CCObject*)
    {
        std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();

        CURL* curl;
        CURLcode res;
        std::string readBuffer;
        std::string postfield = "deviceId=" + udid;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://85.209.2.73:25568/AdminPanel/CheckAdmin");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        if (readBuffer == "1")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cb>moderator</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
            setting().isAdmin = true;
        }
        else if (readBuffer == "2")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cp>elder moderator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
            setting().isAdmin = true;
        }
        else if (readBuffer == "3")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cr>administrator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
            setting().isAdmin = true;
        }
        else if (readBuffer == "4")
        {
            gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cy>owner</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
            setting().isAdmin = true;
        }
        else if (readBuffer == "-1")
        {
            gd::FLAlertLayer::create(nullptr, "Admin Check", "You have no moderator privilegies.", "Ok", nullptr, 260.f, false, 0)->show();
            setting().isAdmin = false;
        }
        else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
        std::cout << "ADMIN CHECK PROTOCOL" << std::endl;
        std::cout << readBuffer << std::endl;
        std::cout << udid << " - User device id" << std::endl << std::endl;
        readBuffer.clear();
    }
};

void adminInitCheck()
{
    std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string postfield = "deviceId=" + udid;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://85.209.2.73:25568/AdminPanel/CheckAdmin");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    if (readBuffer == "1") setting().isAdmin = true;
    else if (readBuffer == "-1") setting().isAdmin = false;
    std::cout << "ADMIN INIT CHECK PROTOCOL" << std::endl;
    std::cout << postfield << std::endl;
    std::cout << readBuffer << std::endl;
    std::cout << udid << " - User device id" << std::endl << std::endl;
    

    readBuffer.clear();
}

bool isFirstLoaded = false;

bool(__thiscall* MenuLayer_init)(CCLayer* self);
bool __fastcall MenuLayer_init_H(CCLayer* self, void*) {
    if (!MenuLayer_init(self)) return false;
    if (!isFirstLoaded)
    {
        adminInitCheck();
        isFirstLoaded = true;
    }
    auto adminCheckSprite = CCSprite::create("GJ_button_03.png"); //
    if (!(adminCheckSprite->initWithFile("accountBtn_pendingRequest_001.png")))
    {
        adminCheckSprite->initWithFile("GJ_button_03.png");
        auto albl = CCLabelBMFont::create("A", "bigFont.fnt");
        adminCheckSprite->addChild(albl);
        albl->setPosition({ 20, 22});
    }
    auto adminCheckButton = gd::CCMenuItemSpriteExtra::create(adminCheckSprite, nullptr, self, menu_selector(AdminCheck::adminCheckCallback));
    auto menu = CCMenu::create();
    adminCheckButton->setScale(1.f);
    menu->addChild(adminCheckButton);
    menu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
    self->addChild(menu);

    return true;
}

bool(__thiscall* PauseLayer_init)(CCLayer* self);
bool __fastcall PauseLayer_init_hook(CCLayer* self)
{
    PauseLayerObject = self;
    GetLocalTime(&st);
    bool result = PauseLayer_init(self);
    return result;
}

bool(__thiscall* PlayLayer_resume)(CCLayer* self);
bool __fastcall PlayLayer_resume_H(CCLayer* self)
{
    PauseLayerObject = nullptr;
    bool result = PlayLayer_resume(self);
    return result;
}

bool(__thiscall* PauseLayer_onEdit)(CCLayer* self);
void __fastcall PauseLayer_onEdit_H(CCLayer* self)
{
    PlayLayerObject = nullptr;
    PauseLayer_onEdit(self);
}

bool(__thiscall* PlayLayer_onQuit)(CCNode* self);
void __fastcall PlayLayer_onQuit_H(CCNode* self) {

    PlayLayerObject = nullptr;
    PlayLayer_onQuit(self);
}

bool(__thiscall* GarageLayer_init)(CCLayer* self);
bool __fastcall GarageLayer_init_H(CCLayer* self) {

    auto drawer = CCDrawNode::create();
    drawer->setZOrder(10);
    drawer->drawDot({ 200, 200 }, 5.f, { 255.f, 255.f, 255.f, 255.f });

    if (!GarageLayer_init(self)) return false;
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    auto scrnRight = director->getScreenRight();
    auto scrnTop = director->getScreenTop();
    auto sprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    auto label = CCLabelBMFont::create("", "bigFont.fnt");
    
    //auto demons = gd::GJUserScore->getCompletedDemons();
    int demons = gd::GameStatsManager::sharedState()->getStat("5");

    label->setString(CCString::createWithFormat("%d", demons)->getCString());
    label->setAnchorPoint({ 1.f, 0.5f });
    label->setScale(0.5f);
    label->setPosition({ scrnRight - 58, scrnTop - 96 });
    //sprite->setAnchorPoint({ 0.f, 0.f });
    sprite->setPosition({ scrnRight - 40, scrnTop - 96 });
    sprite->setScale(1.f);

    self->addChild(sprite);
    self->addChild(label);
    
    self->addChild(drawer);
    return true;
} 

bool(__thiscall* EndLayer_init)(CCLayer* self);
void __fastcall EndLayer_init_H(CCLayer* self) {
    /*auto label = CCLabelBMFont::create("Ayo", "bigFont.fnt");
    label->setScale(10.f);
    label->setPosition(self->getPosition());
    self->addChild(label);*/
    EndLayer_init(self);
    
}

bool(__thiscall* EditorUI_init)(gd::EditorUI* self, CCLayer* editor);
bool __fastcall EditorUI_init_H(gd::EditorUI* self, void*, CCLayer* editor) {
    PlayLayerObject = nullptr;
    editUI = self;
    bool result = EditorUI_init(self, editor);
    if (setting().onPersistentClipboard)
    {
        if (!savedClipboard.empty()) {
            self->clipboard() = savedClipboard;
            self->updateButtons();
        }
    }
    return result;
}

bool(__thiscall* EditorUI_dtor)(gd::EditorUI* self);
bool __fastcall EditorUI_dtor_H(gd::EditorUI* self) {
    //editUI = self;
    if (setting().onPersistentClipboard)
    {
        savedClipboard = self->clipboard();
    }
    EditorUI_dtor(self);
}

bool(__thiscall* EditorPauseLayer_onExitEditor)(CCObject* self);
void __fastcall EditorPauseLayer_onExitEditor_H(CCObject* self) {
    editUI = nullptr;
    EditorPauseLayer_onExitEditor(self);
}

//bool(__thiscall* MenuLayer_init)(CCLayer* self);
//bool __fastcall MenuLayer_init_H(CCLayer* self) {
//    
//    bool result = MenuLayer_init(self);
//    auto label = CCLabelBMFont::create("Goodbye World!", "bigFont.fnt");
//    label->setPosition({100, 100});
//    label->setScale(1.5f);
//    self->addChild(label);
//    return result;
//}

bool(__thiscall* Scheduler_update)(CCScheduler* self, float idk);
void __fastcall Scheduler_update_H(CCScheduler* self, void* edx, float idk) {

    /*if (PauseLayerObject && setting().onTimeStop)
    {
        BOOL bRet = SetLocalTime(&st);
    }*/

    if (editUI)
    {
        if (setting().onHideEditorUI) editUI->setVisible(0);
        else editUI->setVisible(1);
    }

    if (PlayLayerObject)
    {
        auto SafeModeLabel = PlayLayerObject->getChildByTag(45710);
        auto CheatIndicatorLabel = PlayLayerObject->getChildByTag(4572);

        if (SafeModeLabel)
        {
            if (setting().onSafeMode && setting().onSafeModeLabel) SafeModeLabel->setVisible(1);
            else SafeModeLabel->setVisible(0);
        }

        if (CheatIndicatorLabel)
        {
            ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().CurrentNoclipByte, 1, 0);
            reinterpret_cast<CCLabelBMFont*>(CheatIndicatorLabel)->setColor({ 0, 255, 0 });

            if (setting().NoclipByte != setting().CurrentNoclipByte && setting().onNoclipOutOfMe == false) { setting().cheatsCount++; setting().beforeRestartCheatsCount++; setting().onNoclipOutOfMe = true; }
            else if (setting().NoclipByte == setting().CurrentNoclipByte && setting().onNoclipOutOfMe == true) { setting().cheatsCount--; setting().onNoclipOutOfMe = false; }

            //no cheats, no before restart cheats, no safe mode
            if (setting().cheatsCount == 0 &&
                setting().beforeRestartCheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte &&
                !setting().onSafeMode)
                reinterpret_cast<CCLabelBMFont*>(CheatIndicatorLabel)->setColor({ 0, 255, 0 });

            //no cheats, no before restart cheats, safe mode
            else if (setting().cheatsCount == 0 &&
                setting().beforeRestartCheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte &&
                setting().onSafeMode)
                reinterpret_cast<CCLabelBMFont*>(CheatIndicatorLabel)->setColor({ 255, 255, 0 });

            //no cheats
            else if (setting().cheatsCount == 0 &&
                setting().NoclipByte == setting().CurrentNoclipByte)
                reinterpret_cast<CCLabelBMFont*>(CheatIndicatorLabel)->setColor({ 255, 128, 0 });

            else reinterpret_cast<CCLabelBMFont*>(CheatIndicatorLabel)->setColor({ 255, 0, 0 });
        }
    }
    
    Scheduler_update(self, idk);
}

//auto save hack state
bool(__thiscall* CCDirector_end)(CCDirector* self);
void __fastcall CCDirector_end_H(CCDirector* self, void* edx) {
    if (setting().onAutoSave)
    {
        auto file = fopen("Resources/hacksave.dat", "wb");
        if (file) {
            fwrite(&setting(), sizeof(setting()), 1, file);
            fclose(file);
        }
    }
    CCDirector_end(self);
}

//bool(__thiscall* CCDirector_init)(CCDirector* self);
//void __fastcall CCDirector_init_H(CCDirector* self, void* edx) {
//    adminInitCheck();
//    CCDirector_end(self);
//}


void idk() {
    return reinterpret_cast<void(__fastcall*)()>(gd::base + 0x47100)();
}

void objectAdd(int id)
{
    __asm {
        push ecx
        push 0x4
        push id
        mov ecx, ebx
        call idk
        push eax
        mov ecx, esi
        call edi
    }
}

bool(__thiscall* ObjectToolboxAdd_RampTab)();
void __fastcall ObjectToolboxAdd_RampTab_H() {
    objectAdd(0x173);
    objectAdd(0x174);
    objectAdd(0x175);
    objectAdd(0x176);
    ObjectToolboxAdd_RampTab();
}

bool(__thiscall* ObjectToolboxAdd_SpikesTab)();
void __fastcall ObjectToolboxAdd_SpikesTab_H() {
    objectAdd(0x1A5);
    objectAdd(0x1A6);
    ObjectToolboxAdd_SpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_DecoSpikesTab)();
void __fastcall ObjectToolboxAdd_DecoSpikesTab_H() {
    objectAdd(0x1A3);
    objectAdd(0x1A4);
    ObjectToolboxAdd_DecoSpikesTab();
}

bool(__thiscall* ObjectToolboxAdd_ChainTab)();
void __fastcall ObjectToolboxAdd_ChainTab_H() {
    objectAdd(0x196);
    objectAdd(0x197);
    objectAdd(0x198);
    objectAdd(0x2D5);
    ObjectToolboxAdd_ChainTab();
}

bool(__thiscall* ObjectToolboxAdd_TriggerTab)();
void __fastcall ObjectToolboxAdd_TriggerTab_H() {
    objectAdd(0x37);
    ObjectToolboxAdd_TriggerTab();
}


//push ecx
//push 4
//push YOUROBJECTID
//mov ecx, ebx
//call 00447100
//push eax
//mov ecx, esi
//call edi


DWORD WINAPI my_thread(void* hModule) {
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x3A21B), "\xB8\x01\x00\x00\x00\x90\x90", 7, NULL);
    ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(0x4EE9DA), &setting().NoclipByte, 1, 0);
    if (MH_Initialize() != MH_OK)
    {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }
    
    //0xd6040 PlayBTN
    //0xd61b0 EditBTN
    //0xd64c0 MenuBTN!
    
    /*
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xee990),
        reinterpret_cast<void*>(&destroyPlayer_H),
        reinterpret_cast<void**>(&destroyPlayer));
    */

    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44864),
        reinterpret_cast<void*>(&ObjectToolboxAdd_RampTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_RampTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x44e99),
        reinterpret_cast<void*>(&ObjectToolboxAdd_SpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_SpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x45e4b),
        reinterpret_cast<void*>(&ObjectToolboxAdd_DecoSpikesTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_DecoSpikesTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x463d6),
        reinterpret_cast<void*>(&ObjectToolboxAdd_ChainTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_ChainTab));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x46c37),
        reinterpret_cast<void*>(&ObjectToolboxAdd_TriggerTab_H),
        reinterpret_cast<void**>(&ObjectToolboxAdd_TriggerTab));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x7bd90),
        reinterpret_cast<void*>(&GarageLayer_init_H),
        reinterpret_cast<void**>(&GarageLayer_init)); 
    
       
    auto cocos = GetModuleHandleA("libcocos2d.dll");

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG_H),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));

    MH_CreateHook(
        reinterpret_cast<void*>(GetProcAddress(cocos, "?end@CCDirector@cocos2d@@QAEXXZ")),
        reinterpret_cast<void**>(&CCDirector_end_H),
        reinterpret_cast<void**>(&CCDirector_end));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(GetProcAddress(cocos, "?init@CCDirector@cocos2d@@UAE_NXZ")),
    //    reinterpret_cast<void**>(&CCDirector_init_H),
    //    reinterpret_cast<void**>(&CCDirector_init));


    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xE1b10), 
        reinterpret_cast<void**>(&PlayLayer_init_H),
        reinterpret_cast<void**>(&PlayLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xe78a0),
        reinterpret_cast<void**>(&PlayLayer_update_H),
        reinterpret_cast<void**>(&PlayLayer_update));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1e10),
        reinterpret_cast<void**>(&PlayLayer_resume_H),
        reinterpret_cast<void**>(&PlayLayer_resume));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf03f0),
        reinterpret_cast<void**>(&restartLevel_H),
        reinterpret_cast<void**>(&restartLevel));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd4510),
        reinterpret_cast<void**>(&PauseLayer_init_hook),
        reinterpret_cast<void**>(&PauseLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd61b0),
        reinterpret_cast<void**>(&PauseLayer_onEdit_H),
        reinterpret_cast<void**>(&PauseLayer_onEdit));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xf1fe0),
        reinterpret_cast<void*>(&PlayLayer_onQuit_H),
        reinterpret_cast<void**>(&PlayLayer_onQuit));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3f1e0),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitEditor_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onExitEditor));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3e150),
        EditorPauseLayer::EditorPauseLayer_init_hook,
        reinterpret_cast<void**>(&EditorPauseLayer::EditorPauseLayer_init));

    /*
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xd4510), 
        PauseLayer::hook,
        reinterpret_cast<void**>(&PauseLayer::init));
    */

    
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xae7a0), //0x1907b0 (original address) 0xae7a0
        reinterpret_cast<void*>(&MenuLayer_init_H),
        reinterpret_cast<void**>(&MenuLayer_init)); // note the &, this gets the address of the variable
        
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3fc00),
        reinterpret_cast<void**>(&EditorUI_init_H),
        reinterpret_cast<void**>(&EditorUI_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3f9d0),
        reinterpret_cast<void**>(&EditorUI_dtor_H),
        reinterpret_cast<void**>(&EditorUI_dtor));

    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x3fc00),
    //    EditorPauseLayer::EditorUILayer_init_H,
    //    reinterpret_cast<void**>(&EditorPauseLayer::EditorUILayer_init));
    //MH_CreateHook(
    //    reinterpret_cast<void*>(gd::base + 0x3f9d0),
    //    EditorPauseLayer::EditorUILayer_dtor_H,
    //    reinterpret_cast<void**>(&EditorPauseLayer::EditorUILayer_dtor));
    
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x3b320),
        EditLevelLayer::EditLevelLayer_init_H,
        reinterpret_cast<void**>(&EditLevelLayer::EditLevelLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x9b160),
        GJGameLevel::GJGameLevel_init_hook,
        reinterpret_cast<void**>(&GJGameLevel::GJGameLevel_init));
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x4ff10),
        reinterpret_cast<void**>(&EndLayer_init_H),
        reinterpret_cast<void**>(&EndLayer_init));
    MH_CreateHook(
        reinterpret_cast<void*>(cocosbase + 0xffcc0),
        reinterpret_cast<void**>(&Scheduler_update_H),
        reinterpret_cast<void**>(&Scheduler_update));

    //0x47f520

    //GJGameLayerInit gd::base + 0x9b160
      
    setup_imgui_menu();

    MH_EnableHook(MH_ALL_HOOKS);
    
}
//base + 0xd4510 - GHS 1.91 PauseLayer::init
//base + 0x1E4620 - GD 2.13 PauseLayer::init
//base + 0xd5f50 - GD 1.92 PauseLayer::init

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//GarageLayer_init address
//gd::base + 0x7bd90 //GHS 1.91
// 
//EndLayer_init
//gd::base + 0x44ff10


//Adding objects in ObjectToolbox

//push ecx
//push 4
//push YOUROBJECTID
//mov ecx, ebx
//call 00447100
//push eax
//mov ecx, esi
//call edi



// base + 0x55190 - GameLevelManager::sharedState();
// base + 0x88b00 - LevelBrowserLayer::init();
// base + 0x89570 - LevelBrowserLayer::loadPage();
// base + 0x57830 - GameLevelManager::getSavedLevels();