#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <gd.h>

namespace gd {

	class CCSpritePlus;

	// jesus fucking christ
	enum GameObjectType {
		kGameObjectTypeSolid = 0,
		kGameObjectTypeHazard = 2,
		kGameObjectTypeInverseGravityPortal = 3,
		kGameObjectTypeNormalGravityPortal = 4,
		kGameObjectTypeShipPortal = 5,
		kGameObjectTypeCubePortal = 6,
		kGameObjectTypeDecoration = 7,
		kGameObjectTypeYellowJumpPad = 8,
		kGameObjectTypePinkJumpPad = 9,
		kGameObjectTypeGravityPad = 10,
		kGameObjectTypeYellowJumpRing = 11,
		kGameObjectTypePinkJumpRing = 12,
		kGameObjectTypeGravityRing = 13,
		kGameObjectTypeInverseMirrorPortal = 14,
		kGameObjectTypeNormalMirrorPortal = 15,
		kGameObjectTypeBallPortal = 16,
		kGameObjectTypeRegularSizePortal = 17,
		kGameObjectTypeMiniSizePortal = 18,
		kGameObjectTypeUfoPortal = 19,
		kGameObjectTypeModifier = 20,
		kGameObjectTypeSecretCoin = 22,
		kGameObjectTypeDualPortal = 23,
		kGameObjectTypeSoloPortal = 24,
		kGameObjectTypeSlope = 25,
		kGameObjectTypeWavePortal = 26,
		kGameObjectTypeRobotPortal = 27,
		kGameObjectTypeTeleportPortal = 28,
		kGameObjectTypeCollectible = 30,
		kGameObjectTypeUserCoin = 31,
		kGameObjectTypeDropRing = 32,
		kGameObjectTypeSpiderPortal = 33,
		kGameObjectTypeRedJumpPad = 34,
		kGameObjectTypeRedJumpRing = 35,
		kGameObjectTypeCustomRing = 36,
		kGameObjectTypeDashRing = 37,
		kGameObjectTypeGravityDashRing = 38,
		kGameObjectTypeCollisionObject = 39,
		kGameObjectTypeSpecial = 40,
	};



	enum GJCustomColorMode {};

	class GJSpriteColor;
	class ColorActionSprite;
	class GJEffectManager;

	#pragma runtime_checks("s", off)
	class GameObject : public CCSpritePlus {
	protected:
		PAD(44);
		bool unk_21C;
		bool unk_21D;
		bool unk_21E;
		PAD(13);
		cocos2d::CCPoint m_obRealPositionOffset; //0x022C
		PAD(4);
		bool unk_238;
		bool m_bIsFlippedX; //0x0239
		bool m_bIsFlippedY; //0x023A
		PAD(9);
		bool m_bIsOriented; //0x0244 // what
		PAD(35);
		float m_fObjectRectWidth; //0x0268
		float m_fObjectRectHeight; //0x026C
		bool unk_270;
		bool unk_271;
		bool unk_272;
		PAD(1);
		cocos2d::CCParticleSystemQuad* m_pParticleSystemQuad;
		std::string m_sEffectPlistName; //0x0278
		PAD(32);
		bool unk_2B0;
		PAD(23);
		bool unk_2C8;
		bool unk_2C9;
		bool m_bHasBeenActivated; //0x02CA
		PAD(25);
		cocos2d::CCSprite* m_pUnk_2E4;
		PAD(8);
		bool m_bIsRotatedSide; //0x02F0 // what
		PAD(3);
		float unk_2F4;
		float unk_2F8;
		unsigned int m_nUniqueID; //0x02FC
		GameObjectType m_nObjectType; //0x0300
		int m_nSection; //0x0304
		PAD(4);
		cocos2d::CCPoint m_obRealPosition; //0x030C
		std::string m_sTextureName; //0x0314
		bool unk_32C;
		bool unk_32D;
		PAD(14);
		float unk_33C;
		float unk_340;
		PAD(16);
		bool m_bHasGlowDisabled; //0x0354
		PAD(7);
		float m_fScale; //0x035C
		int m_nObjectID; //0x0360
		PAD(4);
		bool unk_368;
		bool unk_369;
		bool unk_36A;
		bool m_bDontEnter; //0x036B
		bool m_bDontFade; //0x036C
		PAD(35);
		float unk_390;
		PAD(20);
		GJSpriteColor* m_pBaseColor; //0x03A8
		GJSpriteColor* m_pDetailColor; //0x03AC
		PAD(8);
		int m_nZLayer; //0x03B8
		int m_nZOrder; //0x03BC
		PAD(24);
		bool unk_3D8;
		bool unk_3D9;
		bool m_bIsSelected; //0x03DA
		PAD(1);
		int m_nGlobalClickCounter; //0x03DC // this is rly weird
		PAD(12);
		float unk_3EC;
		bool m_bGroupParent; //0x03F0
		PAD(3);
		short* m_pGroups; //0x03F4
		short m_nGroupCount; //0x03F8
		PAD(18);
		int m_nEditorLayer; //0x040C
		int m_nEditorLayer2; //0x0410
		PAD(16);
		cocos2d::CCPoint m_obStartPos; //0x0424 // maybe
		PAD(28);
		bool m_bHighDetail; //0x0448
		PAD(3);
		ColorActionSprite* unk_44C;
		ColorActionSprite* unk_450;
		GJEffectManager* m_pEffectManager;
		PAD(16);

	public:
		//CCNode vtable
		virtual void setScaleX(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x72070
				)(this, scale);
		}
		virtual void setScaleY(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x720d0
				)(this, scale);
		}
		virtual void setScale(float scale) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x72130
				)(this, scale);
		}
		virtual void setPosition(const cocos2d::CCPoint& pos) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, const cocos2d::CCPoint&)>(
				base + 0x71e60
				)(this, pos);
		}
		virtual void setVisible(bool visible) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, bool)>(
				base + 0x72610
				)(this, visible);
		}
		virtual void setRotation(float rotation) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, float)>(
				base + 0x71f50
				)(this, rotation);
		}
		virtual bool initWithTexture(cocos2d::CCTexture2D* texture) {
			return reinterpret_cast<bool(__thiscall*)(GameObject*, cocos2d::CCTexture2D*)>(
				base + 0xCFA80
				)(this, texture); //not correct
		}
		virtual void setChildColor(const cocos2d::ccColor3B& color) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, const cocos2d::ccColor3B&)>(
				base + 0xEE900
				)(this, color); //not correct
		}

		//CCRGBAProtocol vtable
		virtual void setOpacity(GLubyte opacity) {
			return reinterpret_cast<void(__thiscall*)(GameObject*, GLubyte)>(
				base + 0x72350
				)(this, opacity);
		}

		static GameObject* createWithFrame(const char* frame) {
			return reinterpret_cast<GameObject* (__fastcall*)(const char*)>(
				base + 0xCF8F0 //not correct
				)(frame);
		}
		static GameObject* objectFromString(std::string str, bool unknown) {
			auto pRet = reinterpret_cast<GameObject* (__fastcall*)(std::string, bool)>(
				base + 0xEBE50 //not correct
				)(str, unknown);
			__asm add esp, 0x18
			return pRet;
		}

		void deselectObject() {
			this->m_bIsSelected = false;

			return reinterpret_cast<void(__fastcall*)(GameObject*)>(
				base + 0xeee50 //maybe it is lol
			)(this);
		}

		std::string getSaveString() {
			std::string ret;

			reinterpret_cast<void(__thiscall*)(GameObject*, std::string*)>(
				base + 0xed0c0
			)(
				this, &ret
			);

			return ret;
		}

		void addToGroup(int id) {
			reinterpret_cast<void(__thiscall*)(
				GameObject*, int
			)>(
				base + 0xeb8d0
			)(
				this, id
			);
		}

		auto getColorMode() {
			auto active = from<int>(this, 0x308);
			auto default_color = from<int>(this, 0x30c);
			// TODO: gd checks some boolean
			if (active == 0)
				active = default_color;
			return active;
		}

		bool isOnValidGroup() {
			if (from<int>(this, 0x160) == 842150450) return 0;
			else return 1;
		}

		int getGroup()
		{
			return from<int>(this, 0x324);
		}

		int getType() {
			return from<int>(this, 0x249);
			//return reinterpret_cast<int(__fastcall*)(GameObject*)>(base + 0x52240)(this);
		}
		void setType(int newType) {
			from<int>(this, 0x249) = newType;
		}

		float getObjectRadius() {
			return from<float>(this, 0x254);
		}

		bool getIsFlippedY() {
			return from<bool>(this, 0x1C4);
		}
		bool getIsFlippedX() {
			return from<bool>(this, 0x1C5);
		}

		bool getUseAudioScale() {
			return from<bool>(this, 0x295);
		}

		void updateOrientedBox() {
			return reinterpret_cast<void (__thiscall*)(GameObject*)>(base + 0x75300)(this);
		}

		gd::OBB2D* getOrientedBox() {
			return reinterpret_cast<gd::OBB2D*(__thiscall*)(GameObject*)>(base + 0x75290)(this);
		}

		int getObjectID() {
			return from<int>(this, 0x2c4);
		}


		virtual cocos2d::CCRect* const& getObjectRect(cocos2d::CCRect* rect) {
			return reinterpret_cast<cocos2d::CCRect* (__thiscall*)(GameObject*, cocos2d::CCRect*)>(base + 0x71b90)(this, rect);
		}

		cocos2d::CCRect* getObjectRect(cocos2d::CCRect* rect, float fl1, float fl2) {
			return reinterpret_cast<cocos2d::CCRect * (__thiscall*)(GameObject*, cocos2d::CCRect*, float, float)>(base + 0x71bd0)(this, rect, fl1, fl2);
		}
		//

		unsigned int getUniqueID() { return m_nUniqueID; }
		short getGroupID(int ix) { return m_pGroups[ix]; }
		short getGroupIDCount() { return m_nGroupCount; }
		int getGameZOrder() { return m_nZOrder; }
		void setGameZOrder(int z) { m_nZOrder = z; }
		void setGameObjType(GameObjectType t) { m_nObjectType = t; }
	};
	#pragma runtime_checks("s", restore)
}

#endif