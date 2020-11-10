#include <string>
#include <vector>
#include <Windows.h>
#include"vars.h"


template <typename T>
class VarType
{
public:
	VarType(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
public:
	CConfig()
	{
		ints = std::vector<VarType<int>*>();
		bools = std::vector<VarType<bool>*>();
		floats = std::vector<VarType<float>*>();
	}

	void Initialize()
	{ 
		szPath = "C:\\config\\";
		Setup();
	}

	void Save()
	{
		std::string file;

		file = szPath + Vars::Config::configname;

		CreateDirectoryA(szPath.c_str(), NULL);

		for (VarType<int>* pValue : ints)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<float>* pValue : floats)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), std::to_string(*pValue->value).c_str(), file.c_str());

		for (VarType<bool>* pValue : bools)
			WritePrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), *pValue->value ? "1" : "0", file.c_str());
	}

	void Load()
	{
		std::string file;

		file = szPath + Vars::Config::configname;

		CreateDirectoryA(szPath.c_str(), NULL);

		char value_l[32] = { '\0' };

		for (VarType<int>* pValue : ints)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = atoi(value_l);
		}

		for (VarType<float>* pValue : floats)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = (float)atof(value_l);
		}

		for (VarType<bool>* pValue : bools)
		{
			GetPrivateProfileStringA(pValue->category.c_str(), pValue->name.c_str(), "", value_l, 32, file.c_str());
			*pValue->value = !strcmp(value_l, "1");
		}
	}

private:
	void SetupValue(int &value, int def, std::string category, std::string name)
	{
		value = def;
		ints.push_back(new VarType<int>(category, name, &value));
	}

	void SetupValue(bool &value, bool def, std::string category, std::string name)
	{
		value = def;
		bools.push_back(new VarType<bool>(category, name, &value));
	}

	void SetupValue(float &value, float def, std::string category, std::string name)
	{
		value = def;
		floats.push_back(new VarType<float>(category, name, &value));
	}

	void Setup()
	{
		//aim
		SetupValue(Vars::AimBot::Activate, false, "AimBot", "Activate");
		SetupValue(Vars::AimBot::BodyAim, false, "AimBot", "BodyAim");

		SetupValue(Vars::AimBot::RCS, false, "AimBot", "RecoilControlSystem");
		SetupValue(Vars::AimBot::Standalone, false, "AimBot", "Standalone");
		SetupValue(Vars::AimBot::RightMouseAiming, false, "AimBot", "RightMouseAiming");
		SetupValue(Vars::AimBot::VisibleCheck, false, "AimBot", "VisibleCheck");
		SetupValue(Vars::AimBot::DrawFov, false, "AimBot", "DrawFov");
		SetupValue(Vars::AimBot::Distance, 250.f, "AimBot", "Distance");
		SetupValue(Vars::AimBot::Fov, 30.f, "AimBot", "Fov");
		//SetupValue(Vars::AimBot::silentAim, false, "AimBot", "SilentAim");
		SetupValue(Vars::AimBot::Prediction, false, "AimBot", "Prediction");
		SetupValue(Vars::AimBot::FillFov, false, "AimBot", "FillFov");


		// weapons
		SetupValue(Vars::Weapon::NoRecoil, false, "Weapons", "NoRecoil");
		SetupValue(Vars::Weapon::RapidFire, false, "Weapons", "RapidFire");

		//visuals
		SetupValue(Vars::PlayerEsp::ShowPlayerBox, false, "Visuals", "ShowPlayerBox");
		SetupValue(Vars::PlayerEsp::ShowPlayerName, false, "Visuals", "ShowPlayerName");
		SetupValue(Vars::PlayerEsp::ShowPlayerHealth, false, "Visuals", "ShowPlayerHealth");
		SetupValue(Vars::PlayerEsp::ShowPlayerWeapon, false, "Visuals", "ShowPlayerWeapon");
		SetupValue(Vars::PlayerEsp::ShowPlayerDist, false, "Visuals", "ShowPlayerDist");
		SetupValue(Vars::Misc::AlwaysDay, false, "Visuals", "AlwaysDay");
		SetupValue(Vars::PlayerEsp::IgnoreSleepers, false, "Visuals", "IgnoreSleepers");
		SetupValue(Vars::PlayerEsp::SkeletonPlayer, false, "Visuals", "Skeleton");
		SetupValue(Vars::AimBot::Crosshair, false, "Visuals", "Crosshair");
		SetupValue(Vars::PlayerEsp::PlayerESP, false, "Visuals", "PlayerESP");
		SetupValue(Vars::PlayerEsp::FillBox, false, "Visuals", "FillBoxes");
		SetupValue(Vars::PlayerEsp::SkeletonBots, false, "Visuals", "SkeletonBots");
		SetupValue(Vars::PlayerEsp::SkeletonBots, false, "Visuals", "SkeletonBots");
		SetupValue(Vars::PlayerEsp::SnapLines, false, "Visuals", "SnapLines");
		//SetupValue(Vars::PlayerEsp::corpse, false, "Visuals", "Corpse");
		SetupValue(Vars::PlayerEsp::ShowHPBox, false, "Visuals", "PlayerHeart");
		//SetupValue(Vars::PlayerEsp::VisibleEsp, false, "Visuals", "VisibleEsp");
		//misc
		SetupValue(Vars::Misc::SpiderMan, false, "Misc", "SpiderMan");
		SetupValue(Vars::Weapon::AntiSpread, false, "Misc", "AntiSpread");
		SetupValue(Vars::Weapon::FastReload, false, "Misc", "FastReload");
		SetupValue(Vars::Weapon::SetAutomatic, false, "Misc", "SetAutomatic");
		SetupValue(Vars::Weapon::SuperEoka, false, "Misc", "SuperEoka");
		SetupValue(Vars::Weapon::SkinChanger, false, "Misc", "SkinChanger");
		SetupValue(Vars::Weapon::NoSway, false, "Misc", "NoSway");
		SetupValue(Vars::Misc::SuperBow, false, "Misc", "SuperBow");
		SetupValue(Vars::Misc::LongHand, false, "Misc", "LongHand");
		SetupValue(Vars::Misc::FatHand, false, "Misc", "FatHand");
		SetupValue(Vars::Misc::RunOnHit, false, "Misc", "RunOnHit");
		SetupValue(Vars::Misc::FakeAdmin, false, "Misc", "FakeAdmin");
		SetupValue(Vars::Misc::HighJump, false, "Misc", "HighJump");
		SetupValue(Vars::Misc::JumpValue, 1.2f, "Misc", "Jump Multipler");
		SetupValue(Vars::Misc::WaterBoost, false, "Misc", "Fast Swim");
		SetupValue(Vars::Misc::NoHeavyReduct, false, "Misc", "Fast Run");
		SetupValue(Vars::Misc::AirStuck, false, "Misc", "AirStuck");
		SetupValue(Vars::Misc::AntiKastrulya, false, "Misc", "AntiKastrulya");
		//SetupValue(Vars::Misc::Fly, false, "Misc", "NoClip");
		//SetupValue(Vars::Misc::SwimOnGround, false, "Misc", "SwimOnGround");
		SetupValue(Vars::Misc::CustomTime, false, "Misc", "CustomTime");
		SetupValue(Vars::Misc::Time, 0.f, "Misc", "Time");
		SetupValue(Vars::Misc::skyDome, false, "Misc", "SkyDemo");
		SetupValue(Vars::Misc::swiming, false, "Misc", "Swim");
		SetupValue(Vars::Misc::SetSwim, 0.f, "Misc", "Swiming");
		SetupValue(Vars::Misc::WalkOnWater, false, "Misc", "WalkOnWater");
		SetupValue(Vars::BotsEsp::botsESP, false, "BotsEsp", "botsESP");
		SetupValue(Vars::BotsEsp::BotsBoxes, false, "BotsEsp", "BotsBoxes");
		SetupValue(Vars::BotsEsp::SkeletonBots, false, "BotsEsp", "SkeletonBots");
		SetupValue(Vars::BotsEsp::ShowBotsHealth, false, "BotsEsp", "ShowBotsHealth");
		SetupValue(Vars::BotsEsp::BotsName, false, "BotsEsp", "BotsName");
		SetupValue(Vars::BotsEsp::ShowBotsDist, false, "BotsEsp", "ShowBotsDist");
		SetupValue(Vars::BotsEsp::ShowBotsWeapon, false, "BotsEsp", "ShowBotsWeapon");
		//radar
		SetupValue(Vars::OtherEsp::Radar, false, "Radar", "Radar");

		//colorpicker
#pragma region Fov
		SetupValue(Vars::Color::FillFovColors[0], 255.f, "Color", "FovR");
		SetupValue(Vars::Color::FillFovColors[1], 255.f, "Color", "FovG");
		SetupValue(Vars::Color::FillFovColors[2], 255.f, "Color", "FovB");
#pragma endregion
#pragma region Box
		SetupValue(Vars::Color::BoxColors[0], 255.f, "Color", "BoxR");
		SetupValue(Vars::Color::BoxColors[1], 255.f, "Color", "BoxG");
		SetupValue(Vars::Color::BoxColors[2], 255.f, "Color", "BoxB");
#pragma endregion
#pragma region Skeleton
		SetupValue(Vars::Color::SkeletonColors[0], 255.f, "Color", "SkeletonR");
		SetupValue(Vars::Color::SkeletonColors[1], 255.f, "Color", "SkeletonG");
		SetupValue(Vars::Color::SkeletonColors[2], 255.f, "Color", "SkeletonB");
#pragma endregion
#pragma region Name
		SetupValue(Vars::Color::NameColors[0], 255.f, "Color", "NameR");
		SetupValue(Vars::Color::NameColors[1], 255.f, "Color", "NameG");
		SetupValue(Vars::Color::NameColors[2], 255.f, "Color", "NameB");
#pragma endregion
#pragma region Dist
		SetupValue(Vars::Color::Fov[0], 255.f, "Color", "DistR");
		SetupValue(Vars::Color::Fov[1], 255.f, "Color", "DistG");
		SetupValue(Vars::Color::Fov[2], 255.f, "Color", "DistB");
#pragma endregion
#pragma region Weapon
		SetupValue(Vars::Color::WeaponColors[0], 255.f, "Color", "WeaponR");
		SetupValue(Vars::Color::WeaponColors[1], 255.f, "Color", "WeaponG");
		SetupValue(Vars::Color::WeaponColors[2], 255.f, "Color", "WeaponB");
#pragma endregion
#pragma region Crosshair
		SetupValue(Vars::Color::CrosshairColors[0], 255.f, "Color", "CrossR");
		SetupValue(Vars::Color::CrosshairColors[1], 255.f, "Color", "CrossG");
		SetupValue(Vars::Color::CrosshairColors[2], 255.f, "Color", "CrossB");
#pragma endregion
#pragma region Visible
		SetupValue(Vars::Color::VisibleEsp[0], 255.f, "Color", "VisibleR");
		SetupValue(Vars::Color::VisibleEsp[1], 255.f, "Color", "VisibleG");
		SetupValue(Vars::Color::VisibleEsp[2], 255.f, "Color", "VisibleB");
#pragma endregion
#pragma region noVisible
		SetupValue(Vars::Color::noVisibleEsp[0], 255.f, "Color", "noVisibleR");
		SetupValue(Vars::Color::noVisibleEsp[1], 255.f, "Color", "noVisibleG");
		SetupValue(Vars::Color::noVisibleEsp[2], 255.f, "Color", "noVisibleB");
#pragma endregion
#pragma region Chams
		SetupValue(Vars::Color::Chams[0], 255.f, "Color", "cR");
		SetupValue(Vars::Color::Chams[1], 255.f, "Color", "cG");
		SetupValue(Vars::Color::Chams[2], 255.f, "Color", "cB");
#pragma endregion
#pragma region SnapLines
		SetupValue(Vars::Color::SnapColors[0], 255.f, "Color", "SnapLinesR");
		SetupValue(Vars::Color::SnapColors[1], 255.f, "Color", "SnapLinesG");
		SetupValue(Vars::Color::SnapColors[2], 255.f, "Color", "SnapLinesB");
#pragma endregion
#pragma region BotColors
		SetupValue(Vars::Color::CSnapColors[0], 255.f, "Color", "BotColors");
		SetupValue(Vars::Color::CSnapColors[1], 255.f, "Color", "BotColors");
		SetupValue(Vars::Color::CSnapColors[2], 255.f, "Color", "BotColors");
#pragma endregion

	}

	std::string szPath = "";

protected:
	std::vector<VarType<int>*> ints;
	std::vector<VarType<bool>*> bools;
	std::vector<VarType<float>*> floats;
};

CConfig config;