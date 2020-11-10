static D2D1::ColorF misc_flags(0.23f, 1.0f, 1.0f, 1.0f);

#include <d3d9.h>
D3DCOLOR FLOAT4TOD3DCOLOR(float Col[])
{
	ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)b, (int)g, (int)r);
}



void Skeleton(BasePlayer* BasePlayer)
{
	BoneList Bones[15] = {
		/*LF*/l_foot, l_knee, l_hip,
		/*RF*/r_foot, r_knee, r_hip,
		/*BD*/spine1, neck, head,
		/*LH*/l_upperarm, l_forearm, l_hand,
		/*RH*/r_upperarm, r_forearm, r_hand
	}; Vector2 BonesPos[15];

	//get bones screen pos
	for (int j = 0; j < 15; j++) {
		if (!LocalPlayer.WorldToScreen(BasePlayer->GetBoneByID(Bones[j]), BonesPos[j]))
			return;
	}

	//draw main lines
	for (int j = 0; j < 15; j += 3) {
		GUI::Render.Line(Vector2{ BonesPos[j].x, BonesPos[j].y }, Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, D2D1::ColorF::Red, 3.f);
		GUI::Render.Line(Vector2{ BonesPos[j].x, BonesPos[j].y }, Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, D2D1::ColorF::Red);
		GUI::Render.Line(Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, Vector2{ BonesPos[j + 2].x, BonesPos[j + 2].y }, D2D1::ColorF::Red, 3.f);
		GUI::Render.Line(Vector2{ BonesPos[j + 1].x, BonesPos[j + 1].y }, Vector2{ BonesPos[j + 2].x, BonesPos[j + 2].y }, D2D1::ColorF::Red);
	}

	//draw add lines
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ BonesPos[5].x, BonesPos[5].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[5].x, BonesPos[5].y }, Vector2{ BonesPos[6].x, BonesPos[6].y }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ BonesPos[9].x, BonesPos[9].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[9].x, BonesPos[9].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ BonesPos[12].x, BonesPos[12].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[12].x, BonesPos[12].y }, Vector2{ BonesPos[7].x, BonesPos[7].y }, D2D1::ColorF::Red);
}

void MemeBox(const D2D1::ColorF& PlayerClr, BasePlayer* BasePlayer)
{
	BoneList Bones[4] = {
		/*UP*/l_upperarm, r_upperarm,
		/*DOWN*/r_foot, l_foot
	}; Vector2 BonesPos[4];

	//get bones screen pos
	for (int j = 0; j < 4; j++) {
		if (!LocalPlayer.WorldToScreen(BasePlayer->GetBoneByID(Bones[j]), BonesPos[j]))
			return;
	}

	//draw main lines
	GUI::Render.Line(Vector2{ BonesPos[0].x, BonesPos[0].y }, Vector2{ BonesPos[1].x, BonesPos[1].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[0].x, BonesPos[0].y }, Vector2{ BonesPos[1].x, BonesPos[1].y }, PlayerClr);
	GUI::Render.Line(Vector2{ BonesPos[0].x, BonesPos[0].y }, Vector2{ BonesPos[3].x, BonesPos[3].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[0].x, BonesPos[0].y }, Vector2{ BonesPos[3].x, BonesPos[3].y }, PlayerClr);
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[1].x, BonesPos[1].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[1].x, BonesPos[1].y }, PlayerClr);
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[3].x, BonesPos[3].y }, D2D1::ColorF::Red, 3.f);
	GUI::Render.Line(Vector2{ BonesPos[2].x, BonesPos[2].y }, Vector2{ BonesPos[3].x, BonesPos[3].y }, PlayerClr);
}

void Crosshair()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;
	GUI::Render.Line(Vector2{ xs, ys - 10 }, Vector2{ xs , ys + 10 }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ xs - 10, ys }, Vector2{ xs + 10, ys }, D2D1::ColorF::Red);
}

void DrawFOV()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;

	GUI::Render.FillCircle(Vector2{ xs, ys }, D2D1::ColorF(0, 0, 0, 0.7f), Vars::AimBot::Fov / Vars::AimBot::FillFov);
	GUI::Render.Ñircle(Vector2{ xs, ys }, D2D1::ColorF(255, 0, 0), Vars::AimBot::Fov, 0.5f);
}

void Radar(BasePlayer* BP)
{



	bool PlayerSleeping = BP->HasFlags(16);
	if (Vars::PlayerEsp::IgnoreSleepers && PlayerSleeping)
		return;

	Vector3 localPos = LocalPlayer.BasePlayer->GetBoneByID(head);
	Vector3 entityPos = BP->GetBoneByID(head);

	Vector2 offset = Vector2(entityPos.x - localPos.x, entityPos.z - localPos.z) / 2.0f;
	Vector2 center = Vector2(Vars::Global::MenuPos.x + 95.0f, Vars::Global::MenuPos.y + 95.0f);

	if (offset.x > 35.0f)
		offset.x = 35.0f;
	if (offset.y > 35.0f)
		offset.y = 35.0f;

	if (offset.x < -35.0f)
		offset.x = -35.0f;
	if (offset.y < -35.0f)
		offset.y = -35.0f;

	if (!PlayerSleeping)
	{
		bool PlayerWounded = BP->HasFlags(64);
		GUI::Render.Ñircle({ center.x + offset.x, center.y + offset.y }, D2D1::ColorF::BlueViolet, 1.0f);
	}
	else
		GUI::Render.Ñircle({ center.x + offset.x, center.y + offset.y }, D2D1::ColorF::White, 1.0f);

}


#pragma region Player ESP

void ESP(BasePlayer* BP, BasePlayer* LP, bool npc = false)
{
	bool PlayerSleeping = BP->HasFlags(16);
	if (Vars::PlayerEsp::IgnoreSleepers && PlayerSleeping)
		return;

	if (Vars::PlayerEsp::SkeletonPlayer && BP->GetSteamID() > 1000000000)
	{
		Skeleton(BP);
	}

	Vector2 tempFeetR, tempFeetL;
	Vector3 ro = BP->GetBoneByID(r_foot);
	if (ro.x == 0 && ro.y == 0 && ro.z == 0) return;
	if (LocalPlayer.WorldToScreen(ro, tempFeetR) && LocalPlayer.WorldToScreen(BP->GetBoneByID(l_foot), tempFeetL))
		Vector2 tempFeetR, tempFeetL;

	if (LocalPlayer.WorldToScreen(BP->GetBoneByID(r_foot), tempFeetR) && LocalPlayer.WorldToScreen(BP->GetBoneByID(l_foot), tempFeetL))
	{
		Vector2 tempHead;
		if (LocalPlayer.WorldToScreen(BP->GetBoneByID(jaw) + Vector3(0.f, 0.16f, 0.f), tempHead))
		{
			Vector2 tempFeet = (tempFeetR + tempFeetL) / 2.f;
			float Entity_h = tempHead.y - tempFeet.y;
			float w = Entity_h / 4;
			float Entity_x = tempFeet.x - w;
			float Entity_y = tempFeet.y;
			float Entity_w = Entity_h / 2;

			bool PlayerWounded = BP->HasFlags(64);
			D2D1::ColorF::Enum PlayerClr = PlayerSleeping ? D2D1::ColorF::Orange : PlayerWounded ? D2D1::ColorF::Orange : D2D1::ColorF::Red;

			int CurPos = 0;


			if (Vars::PlayerEsp::PlayerESP)
			{
				GUI::Render.String(Vector2{ (Entity_x + 7), (Entity_y + Entity_h - 10.f) }, BP->GetName(), PlayerClr);
				CurPos += 15;

				if (Vars::PlayerEsp::ShowPlayerBox)
				{
					if (!PlayerWounded && !PlayerSleeping)
					{
						//GUI::Render.FillRectangle(Vector2{ Entity_x, Entity_y }, Vector2{ Entity_w, Entity_h - 10.f }, D2D1::ColorF(0, 0, 0, 0.3f));

						GUI::Render.Rectangle(Vector2{ Entity_x, Entity_y }, Vector2{ Entity_w, Entity_h - 10.f }, PlayerClr);
					}
					else MemeBox(PlayerClr, BP);

					if (Vars::PlayerEsp::ShowPlayerHealth) {

						int health = (int)BP->GetHealth();
						float maxheal = (npc ? 300.f : 100.f);
						float minheal = (npc ? 300.f : 100.f);

						GUI::Render.FillRectangle(Vector2{ Entity_x + Entity_w - 10.f, Entity_y }, Vector2{ 20, Entity_h }, D2D1::ColorF::Red);
						GUI::Render.FillRectangle(Vector2{ Entity_x + Entity_w - 10.f, Entity_y }, Vector2{ 20, Entity_h * (health / minheal) }, D2D1::ColorF::Magenta);
						GUI::Render.FillRectangle(Vector2{ Entity_x + Entity_w - 10.f, Entity_y }, Vector2{ 20, Entity_h * (health / maxheal) }, D2D1::ColorF::LightGreen);
						GUI::Render.Rectangle(Vector2{ Entity_x + Entity_w - 10.f, Entity_y }, Vector2{ 20, Entity_h }, D2D1::ColorF::Black, 0.5f);
						CurPos += 15;
					}

					if (Vars::PlayerEsp::SnapLines)
					{
						static float screenX = GetSystemMetrics(SM_CXSCREEN);
						static float screenY = GetSystemMetrics(SM_CYSCREEN);
						static Vector2 startPos = Vector2(screenX / 1.5f, screenY);

						GUI::Render.Line(startPos, Vector2{ Entity_x + Entity_w / 1.5f, Entity_y }, D2D1::ColorF::Red);
					}


					if (Vars::PlayerEsp::ShowPlayerWeapon)
					{
						const wchar_t* ActiveWeaponName;
						WeaponData* ActWeapon = BP->GetActiveWeapon();
						ActiveWeaponName = ActWeapon->GetName();
						if (!ActWeapon)
						{
							ActiveWeaponName = L"-";

						}
						else
						{
							ActiveWeaponName = ActWeapon->GetName();
						}

						GUI::Render.String(Vector2{ (Entity_x + 7), (Entity_y + Entity_h + CurPos) }, ActiveWeaponName, D2D1::ColorF::White);
						CurPos += 15;
					}
					if (Vars::PlayerEsp::ShowPlayerName || Vars::PlayerEsp::ShowPlayerDist)
					{
						wchar_t res[64];
						if (wcslen(BP->GetName()) > 0)
						{
							if (Vars::PlayerEsp::ShowPlayerDist)
							{
								_swprintf(res, L"%dm", (int)Math::Calc3D_Dist(LP->GetBoneByID(head), ro));
							}
							else
							{
								_swprintf(res, L"%s", BP->GetName());
							}
							GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 40.f }, res, D2D1::ColorF::White);
						}
					}
				}
			}
		}
	}
}

#pragma endregion


