void WeaponPatch()
{
	USHORT Hash = 0;
	WeaponData* Weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
	if (Weapon) Hash = Weapon->GetNameHash();

	switch (Hash)
	{
		//nullptr
	case 0: return;

		//Weapon
	case 0x5A9F:
		Weapon->SuperBow();
		break;

	case 0x6B1:
	case 0x74F5:
	case 0x67DA:
	case 0x3146:
	case 0x79C4:
	case 0x573C:
	case 0xB32A:
	case 0xC196:
		Weapon->SetAutomatic();

	case 0x7983:
	case 0xE5EA:
	case 0x77E9:
	case 0xE97D:
	case 0xC2DD:
	case 0x52F7:
	case 0xFDC3:
	case 0x62E5:
	case 0x6A09:
	case 0x3511:
	case 0xCFA1:
	case 0xF87E:
		Weapon->AntiSpread();
		Weapon->NoRecoil();
		Weapon->FatBullet();


	case 0xB0A0:
	case 0x435E:
	{
		Weapon->FastReload();
		if (Hash == 0x435E)
		{
			//Weapon->SetAutomatic();
			Weapon->SuperEoka();
		}
	} break;

	//Melee
	case 0xF3B9:
		//Weapon->BuildAsUWant();
		break;
	case 0xCBD8:
	case 0xAB85:
	case 0xFB2B:
	case 0x6104:
	case 0xC859:
	case 0x96D3:
	case 0x7340:
	case 0xC68B:
	case 0x2F12:
	case 0x65B4:
	case 0x1F2D:
	case 0x2658:
	case 0xAC02:
	case 0x282A:
	case 0x8151:
	case 0xFACE:
	case 0x7296:
	case 0x4B0A:
	case 0xCD:
	case 0x2333:
		if (Vars::Misc::LongHand) {
			Weapon->FatHand();
			Weapon->LongHand();
			Weapon->RunOnHit();
		} break;
	}
}

float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!LocalPlayer.WorldToScreen(Entity->GetBoneByID(Bone), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(Vars::Global::ScreenWidth / 2, Vars::Global::ScreenHigh / 2), ScreenPos);
}

float GetBulletSpeed()
{
	switch (LocalPlayer.BasePlayer->GetActiveWeapon()->GetNameHash())
	{
	case 0:
		return 0.f;

	case 0x77E9:
		return 420.25f;
		break;

	case 0xCFA1:
		return 656.25f;
		break;

	case 0x573C:
		return 420.f;
		break;

	case 0x6A09:
		return 960.f;
		break;

	case 0x7983:
	case 0xE5EA:
	case 0x79C4:
		return 340.f;
		break;

	case 0xE97D:
		return 192.f;
		break;

	case 0x3511:
		return 190.f;
		break;

	case 0x67DA:
		return 260.f;
		break;

	case 0x62E5:
		return 260.f;
		break;

	case 0x3146:
		return 250.f;
		break;

	case 0x6B1:
		return 260.f;
		break;

	case 0x5A9F:
		return 60.f;
		break;

	case 0xC196:
		return 60.f;
		break;

	case 0xB0A0:
		return 95.f;
		break;

	case 0x746A:
		return 125.f;
		break;

	default:
		return 250.f;
	}
}

auto Prediction(const Vector3& LP_Pos, BasePlayer* Player, BoneList Bone)
{
	Vector3 BonePos = Player->GetBoneByID(Bone);
	float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);

	if (Dist > 0.001f) {
		float BulletTime = Dist / GetBulletSpeed();
		Vector3 vel = Player->GetVelocity();
		Vector3 PredictVel = vel * BulletTime * 0.75f;
		BonePos += PredictVel; BonePos.y += (4.905f * BulletTime * BulletTime);
	} return BonePos;
}

void Normalize(float& Yaw, float& Pitch) {
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}

void GoToTarget(BasePlayer* player)
{
	Vector3 Local = LocalPlayer.BasePlayer->GetBoneByID(neck);
	Vector3 PlayerPos = Prediction(Local, player, BoneList(Vars::Global::BoneToAim));

	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - LocalPlayer.BasePlayer->GetVA();
	Vector2 RecoilAng = LocalPlayer.BasePlayer->GetRA();

	Normalize(Offset.y, Offset.x);

	Vector2 AngleToAim = LocalPlayer.BasePlayer->GetVA() + Offset;
	if (Vars::AimBot::RCS) {
		AngleToAim = AngleToAim - RecoilAng;
	}

	Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer.BasePlayer->SetVA(AngleToAim);
}

void Aim(BasePlayer* AimEntity)
{
	if (Vars::AimBot::Activate)
	{
		if (AimEntity && !LocalPlayer.BasePlayer->IsMenu())
		{
			if (FC(user32,
				GetAsyncKeyState,
				VK_RBUTTON))
				GoToTarget(AimEntity);
		}
	}
}