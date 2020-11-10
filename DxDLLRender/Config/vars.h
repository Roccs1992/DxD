namespace Vars
{
	HANDLE handle;

	namespace Global
	{
		ULONG_PTR gameObjectManager;
		ULONG_PTR gameObjectOffset = 0x14C1790;
		INT16 tag;
		int closestPlayer;
		BYTE state;
		Vector3 position;
		Vector2 MenuPos = { 30.0f, 30.0f };
		Vector2 ShowPlayerBox = { 150, 150 };
		RECT rGameWindow;
		bool DrawCircle = false;
		int ScreenHigh = 0;
		int ScreenWidth = 0;
		int GameWindow;
		HWND hWindow = nullptr;
		bool MenuVisible = false;
		WNDPROC oWindproc = nullptr;

		Vector3 LastPos = { 0.f, 0.f, 0.f };
		ULONGLONG LastTime = 0;
		DWORD64 LastSteamID = 0;
		const wchar_t* LastName;
		Vector3 PlayerVeloc = { 0.f, 0.f, 0.f };

		float BoneToAim = 0.5;

		float BulletSpeed = 250.f;
		DWORD64 PresentDelta = 0;
		float CorX = 0.f;
		float CorY = 0.f;

		int CurPage = 0;
	}

	namespace AimBot
	{
		bool Activate = false;
		bool BodyAim = false;
		bool Prediction = false;
		bool Distance = false;
		bool IgnoreTeam = false;
		bool Standalone = false;
		bool BoneToAim = false;
		bool RightMouseAiming = false;
		bool RCS = false;
		bool VisibleCheck = false;
		bool FatBullet = false;
		bool DrawFov = false;
		bool Crosshair = false;
		float Range = 0.f;
		bool FillFov = false;
		float Fov = 90.f;
		int aimkey = 0;
		static int keystatus = 0;
		bool salobydischanwork_work = false;
	}

	namespace Weapon
	{
		bool Weapon1 = false;
		bool NoRecoil = false;
		bool AntiSpread = false;
		bool FastReload = false;
		bool NoSway = false;
		bool SetAutomatic = false;
		bool noSpread = false;
		bool jumpAim = false;
		bool sprintAim = false;
		bool RapidFire = false;
		bool SuperBow = false;
		bool SuperEoka = false;
		bool SkinChanger = false;
		bool CustomRCS = false;
		bool ProjectileFired = false;
		float RCSs = 0.f;
	}

	namespace PlayerEsp
	{
		bool PlayerESP = false;
		bool ShowPlayerBox = false;
		bool SkeletonPlayer = false;
		bool ShowPlayerName = false;
		bool ShowPlayerHealth = false;
		bool ShowPlayerWeapon = false;
		bool ShowPlayerDist = false;
		bool IgnoreSleepers = false;
		bool SnapLines = false;
		bool CSnapLines = false;
		bool SkeletonBots = false;
		bool ShowHPBox = false;
		bool FillBox = false;
		bool BotsESP = false;
	}


	namespace BotsEsp
	{
		bool botsESP = false;
		bool BotsBoxes = false;
		bool SkeletonBots = false;
		bool ShowBotsHealth = false;
		bool BotsName = false;
		bool ShowBotsDist = false;
		bool ShowBotsWeapon = false;
		bool IgnoreNpc = false;
	}



	namespace TransportEsp
	{
		bool Activate = false;
		bool hellicopter = false;
		bool MineCopter = false;
		bool ScrapCopter = false;
		bool rowboat = false;
		bool horse = false;
	}

	namespace CrateEsp
	{
		bool Activate = false;
		bool heli_crate = false;
		bool bradley_crate = false;
		bool CrateElite = false;
		bool CrateBasic = false;
		bool CrateMine = false;
	}

	namespace WorckbeanchEsp
	{
		bool Activate = false;
		bool workbench1 = false;
		bool workbench2 = false;
		bool workbench3 = false;
	}


	namespace Drop
	{
		bool Activate = false;
		bool supply_drop = false;
		bool WoodBox_bacpack = false;
		bool closestPlayer = false;
		bool inventoryESP = false;
	}

	namespace PicUpEsp
	{
		bool Activate = false;
		bool StonePicUp = false;
		bool MetalPicUp = false;
		bool SulfurPicUp = false;
		bool WoodPicUp = false;
		bool hempEsp = false;
	}

	namespace OtherEsp
	{
		bool Radar = false;
		float RadarSize = 90.f;
		bool Stash = false;
		bool guntrap = false;
		bool campfire = false;
		bool WoodBox = false;
	}


	namespace Misc
	{
		bool Misk = false;
		bool FatBullet = false;
		bool RGB = false;
		float pred = 0.f;
		bool SpiderMan = false;
		bool AimInJump = false;
		bool SuperBow = false;
		bool speedHack = false;
		bool LongHand = false;
		bool Up = false;
		bool NoClip = false;
		bool WalkOnWater = false;
		bool FakeAdmin = false;
		float SetSwim = 0.f;
		bool NoSway = false;
		bool swiming = false;
		bool CustomTime = false;
		float Time = 1.0f;
		bool FlyHack = false;
		bool WorkBench = false;
		bool skyDome = false;
		float ForceCaveLight = 0.f;
		bool NoGreenZone = false;
		bool FatHand = false;
		bool inGame = false;
		bool handle = false;
		bool RunOnHit = false;
		bool HighJump = false;
		bool AlwaysDay = false;
		float JumpValue = 1.2f;
		bool WaterBoost = false;
		bool AirStuck = false;
		bool AntiKastrulya = false;
		bool NoHeavyReduct = false;
	}

	namespace Other
	{
		bool bUnloading = false;
	}

	namespace HvH
	{
		float flCrosshairR = 0.f;
		float flCrosshairG = 0.f;
		float flCrosshairB = 0.f;
		float flCrosshairA = 255.f;
	}
	namespace Visuals
	{
		bool Resource = false;
		bool Stone = false;
		bool Sulphur = false;
		bool Metal = false;
		bool Stash = false;
		bool Hemp = false;
		bool Minicopter = false;
		bool Traps = false;
		bool Corpse = false;
		bool Mushrooms = false;
		bool bagloot = false;
		bool Drop = false;
		bool Airdrop = false;
		bool ScrapHeli = false;
		bool Crate = false;
		bool dropbox = false;



	}

	namespace Color {

		float CSnapColors[] = { 255.f,255.f,255.f,255.f };
		float SnapColors[] = { 255.f,255.f,255.f,255.f };
		bool SnapColor = false;
		bool CSnapColor = false;
		bool FovColor = false;
		bool boxs = false;
		//float box[] = { R / 255.f, G / 255.f, B / 255.f, A / 255.f };
		float Fov[] = { 255.f, 255.f, 255.f };
		float SkeletonColors[] = { 255.f, 255.f, 255.f };
		float BoxColors[] = { 255.f, 255.f, 255.f };
		float NameColors[] = { 255.f, 255.f, 255.f };
		float DistanceColors[] = { 255.f, 255.f, 255.f };
		float WeaponColors[] = { 255.f, 255.f, 255.f };
		float CrosshairColors[] = { 255.f, 255.f, 255.f };
		float Chams[] = { 0.45f, 0.55f, 0.60f, 1.00f };
		float VisibleEsp[] = { 0.45f, 0.55f, 0.60f, 1.00f };
		float noVisibleEsp[] = { 255.f, 0.55f, 0.60f, 1.00f };
		float InventoryESP[] = { 0.f, 0.f, 0.f, 0.f };
		float TabColors[] = { 0.f, 0.f, 0.f, 0.f };
		float BotColors[] = { 0.f, 0.f, 0.f, 0.f };
		float FillFovColors[] = { 0.f, 0.f, 0.f, 0.f };
	}
	namespace Config {
		static char configname[128] = "My";
		bool LoadConfig = false;
		bool SaveConfig = false;
		bool InitConfig = false;
	}

}