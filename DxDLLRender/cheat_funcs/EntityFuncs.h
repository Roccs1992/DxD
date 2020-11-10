namespace DischanSDK
{
	namespace Silent
	{
		void Enable();
		void Disable();
		void SetTarget(DWORD64 pEntity);
	}
}

bool mfound = false;
void FindMatrix() {
	static DWORD64 dwGameObjectManager = 0;
	if (!dwGameObjectManager)
		dwGameObjectManager = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", "xxx????xx", L"UnityPlayer.dll"), 7);

	DWORD64 ObjMgr = safe_read(dwGameObjectManager, DWORD64);
	if (!ObjMgr) return;
	UINT64 end = safe_read(ObjMgr, UINT64);
	for (UINT64 Obj = safe_read(ObjMgr + 0x8, UINT64); (Obj && (Obj != end)); Obj = safe_read(Obj + 0x8, UINT64))
	{
		UINT64 GameObject = safe_read(Obj + 0x10, UINT64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);
		if (Tag == 5)
		{
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			LocalPlayer.pViewMatrix = (Matrix4x4*)(Entity + 0xDC);
			printf(StrA("Found matrix!\n"));
			mfound = true;
			return;
		}
		else if (Tag == 20011 && Vars::Misc::AlwaysDay)
		{
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
			DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
			safe_write(TodCycle + 0x10, 13.37f, float);
		}


	}
}

void EntityZaLoop()
{
	bool LP_isValid = false;
	//float FOV = 1000000.f;
	//BasePlayer* AimEntity = 0;
	BasePlayer* AimEntity = nullptr;
	float FOV = Vars::AimBot::Fov, CurFOV;
	float flSilentSmallestFOV = 999.0f;
	BasePlayer* pNearestEntity = nullptr;
	if (!LocalPlayer.pViewMatrix || !mfound)
	{
		FindMatrix();
	}

	class BaseProjectile
	{
	public:
		char pad_0x0[0x158]; //0x0
		bool isDeployed; //0x158
		char pad_0x159[0x3]; //0x159
		float hostileScore; //0x15c
		char pad_0x164[0x4]; //0x16
		float deployDelay; //0x170
		float repeatDelay; //0x174
		float nextAttackTime; //0x178
		float lastTickTime; //0x17c
		float nextTickTime; //0x180
		float timeSinceDeploy; //0x184
		bool catchup; //0x188
		char pad_0x189[0x7]; //0x189
		float damageScale; //0x1c8
		float distanceScale; //0x1cc
		float projectileVelocityScale; //0x1d0
		bool automatic; //0x1d4
		char pad_0x1d5[0x3]; //0x1d5
		float reloadTime; //0x1d8
		bool canUnloadAmmo; //0x1dc
		char pad_0x1dd[0x3]; //0x1dd
		float aimSway; //0x1e0
		float aimSwaySpeed; //0x1e4
		float aimCone; //0x1e8
		float hipAimCone; //0x1ec
		bool hasADS; //0x1f0
		bool noAimingWhileCycling; //0x1f1
		bool manualCycle; //0x1f2
		bool needsCycle; //0x1f3
		bool isCycling; //0x1f4
		bool aiming; //0x1f5
		char pad_0x1f6[0x2]; //0x1f6
		float nextReloadTime; //0x1f8
		bool isReloading; //0x1fc
		char pad_0x1fd[0x3]; //0x1fd
		float reloadPressTime; //0x200
	};

	DWORD64 BaseNetworkable;
	BaseNetworkable = safe_read(GetModBase(StrW(L"GameAssembly.dll")) + 0x292AEA0, DWORD64);
	DWORD64 EntityRealm = safe_read(BaseNetworkable + 0xB8, DWORD64);
	DWORD64 ClientEntities = safe_read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = safe_read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, DWORD64);
	if (!ClientEntities_values) return;
	int EntityCount = safe_read(ClientEntities_values + 0x10, int);
	DWORD64 EntityBuffer = safe_read(ClientEntities_values + 0x18, DWORD64);

	for (int i = 0; i <= EntityCount; i++)
	{
		DWORD64 Entity = safe_read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);
		if (Entity <= 100000) continue;
		DWORD64 Object = safe_read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = safe_read(Object + 0x30, DWORD64);
		if (ObjectClass <= 100000) continue;
		pUncStr name = safe_read(ObjectClass + 0x60, pUncStr);
		if (!name) continue;
		char* buff = name->stub;
		if (strstr(buff, StrA("Local"))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x4A8, DWORD64)) continue;
			if (Player != LocalPlayer.BasePlayer /*lp changed*/) {
				printf("LocalPlayer %lld\n", Player->GetSteamID());
				mfound = false;
			}
			//printf("lp pos %f %f %f\n", pos.x, pos.y, pos.z);
			LocalPlayer.BasePlayer = Player;
			LP_isValid = true;

		}
		else if (strstr(buff, StrA("player.prefab")) && (!strstr(buff, StrA("prop")) && !strstr(buff, StrA("corpse")))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x4A8, DWORD64)) continue;
			if (!Player->IsValid()) continue;
			bool ex = false;
			//printf("Esp on %lld\n", Player->GetSteamID());
			ESP(Player, LocalPlayer.BasePlayer);
			if (Vars::PlayerEsp::IgnoreSleepers && Player->HasFlags(16)) continue;

			CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim));
			if (flSilentSmallestFOV > CurFOV)
			{
				pNearestEntity = Player;
				flSilentSmallestFOV = CurFOV;
			}

			//find target
			if (ex) continue;
			//cut distance
			if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
				continue;

			//calc visible & low fov
			float CurFOV;
			if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim))))) {
				if (CurFOV <= Vars::AimBot::Fov) {
					FOV = CurFOV; AimEntity = Player;
				}
			}

		}

		else if (Vars::Visuals::Stone && strstr(buff, "stone-ore.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);                         ///public Vector3 PointEnd
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Stone ore", D2D1::ColorF::White);
			}
		}

		else if (Vars::Visuals::Sulphur && strstr(buff, "sulfur-ore.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Sulfur ore", D2D1::ColorF::Gold);
			}
		}

		else if (Vars::Visuals::Metal && strstr(buff, "metal-ore.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Metal ore", D2D1::ColorF::Red);
			}
		}

		else if (Vars::Visuals::Stash && strstr(buff, "small_stash_deployed.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Stash", D2D1::ColorF::Violet);
			}
		}

		else if (Vars::Visuals::Hemp && strstr(buff, "hemp-collectable.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Hemp", D2D1::ColorF::Lime);
			}


		}
		else if (Vars::Visuals::Minicopter && strstr(buff, "minicopter.entity.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"minicopter", D2D1::ColorF::Red);
			}
		}
		else if (Vars::Visuals::Corpse && strstr(buff, "player_corpse.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Corpse", D2D1::ColorF::Yellow);
			}
		}
		else if (Vars::Visuals::Corpse && strstr(buff, "item_drop_backpack.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Backpack", D2D1::ColorF::Black);
			}
		}
		else if (Vars::Visuals::Traps && strstr(buff, "guntrap.deployed.prefab")) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"ShGun Trap", D2D1::ColorF::White);
			}
		}

		else if (strstr(buff, "mushroom-cluster-5.prefab") && Vars::Visuals::Mushrooms) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Mushroom", D2D1::ColorF::Yellow);
			}
		}

		else if (strstr(buff, "item_drop.prefab") && Vars::Visuals::bagloot) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Bag.loot", D2D1::ColorF::Red);
			}
		}

		else if (strstr(buff, "event_airdrop.prefab") && Vars::Visuals::Airdrop) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				GUI::Render.String(Pos, L"Airdrop", D2D1::ColorF::Red);
			}
		}


	}

	if (LP_isValid)
	{
		WeaponPatch();
		Aim(AimEntity);
		Misc();

		if (Vars::AimBot::salobydischanwork_work)
			DischanSDK::Silent::SetTarget((DWORD64)pNearestEntity);
	}

	//LP not valid
	else LocalPlayer.BasePlayer = nullptr;




	static DWORD64 dwGameObjectManager = 0;
	if (!dwGameObjectManager)
		dwGameObjectManager = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", "xxx????xx", L"UnityPlayer.dll"), 7);

	DWORD64 ObjMgr = safe_read(dwGameObjectManager, DWORD64); if (!ObjMgr) return;




	for (DWORD64 Obj = safe_read(ObjMgr + 0x8, DWORD64); (Obj && (Obj != safe_read(ObjMgr, DWORD64))); Obj = safe_read(Obj + 0x8, DWORD64))
	{
		DWORD64 GameObject = safe_read(Obj + 0x10, DWORD64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);

		if (Tag == 6 || Tag == 5 || Tag == 20011)
		{
			DWORD64 ObjClass = safe_read(GameObject + 0x30, DWORD64);
			DWORD64	Entity = safe_read(ObjClass + 0x18, DWORD64);

			//entity
			if (Tag == 6)
			{
				BasePlayer* Player = (BasePlayer*)safe_read(Entity + 0x28, DWORD64);
				if (!Player->IsValid()) continue;

				//entity
				if (safe_read(safe_read(GameObject + 0x60, DWORD64), DWORD64) != 0x616C506C61636F4C)
				{
					//exec esp
					ESP(Player, LocalPlayer.BasePlayer);

					//find target
					if (Vars::AimBot::IgnoreTeam) {
						DWORD64 EntitySID = Player->GetSteamID();
						for (int j = 0; j < LocalPlayer.BasePlayer->GetTeamSize(); j++) {
							DWORD64 SlotSID = LocalPlayer.BasePlayer->IsTeamMate(j);
							if (SlotSID == EntitySID) goto NextEnt;
						}
					}
					CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim));
					if (flSilentSmallestFOV > CurFOV)
					{
						pNearestEntity = Player;
						flSilentSmallestFOV = CurFOV;
					}

					//cut distance
					if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
						goto NextEnt;
					float FOV = Vars::AimBot::Fov, CurFOV;
					//calc visible & low fov
					if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(Vars::Global::BoneToAim))))) {
						FOV = CurFOV; AimEntity = Player;

					}
				}

				//LP
				else {
					LP_isValid = true;
					LocalPlayer.BasePlayer = Player;
				}
			}

			//camera
			else if (Tag == 5)
				LocalPlayer.pViewMatrix = (Matrix4x4*)(Entity + 0xDC);

			//sky
			else if (Tag == 20011 && Vars::Misc::AlwaysDay) {
				DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
				DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
				safe_write(TodCycle + 0x10, 13.37f, float);
			}
		}

		//goto next entity
	NextEnt: continue;
	}

	//GET MEMES
	if (LP_isValid)
	{
		WeaponPatch();
		Aim(AimEntity);
		Misc();

		if (Vars::AimBot::salobydischanwork_work)
			DischanSDK::Silent::SetTarget((DWORD64)pNearestEntity);
	}

	//LP not valid
	else LocalPlayer.BasePlayer = nullptr;
}