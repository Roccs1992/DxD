void Misc()
{
	if (Vars::Misc::HighJump) {
		if (FC(user32, GetAsyncKeyState, VK_MBUTTON))
			LocalPlayer.BasePlayer->SetGravity(Vars::Misc::JumpValue);
		else LocalPlayer.BasePlayer->SetGravity(3.0f);
	}

	//air stuck
	if (Vars::Misc::AirStuck) {
		if (FC(user32, GetAsyncKeyState, VK_CAPITAL))
			LocalPlayer.BasePlayer->AirStuck(true);
		else LocalPlayer.BasePlayer->AirStuck(false);
	}

	//swiminair
	if (Vars::Misc::swiming && GetAsyncKeyState(0x38))
		LocalPlayer.BasePlayer->Swim();

	//dickahrd
	if (Vars::Misc::AimInJump)
		LocalPlayer.BasePlayer->NoBlockAiming();

	//boost speed on heavy armor
	if (Vars::Misc::NoHeavyReduct)
		LocalPlayer.BasePlayer->NoHeavyReduct();

	//spider man
	if (Vars::Misc::SpiderMan)
		LocalPlayer.BasePlayer->SpiderMan();

	//remove weapon sway
	if (Vars::Misc::NoSway)
		LocalPlayer.BasePlayer->NoSway();


	if (Vars::Misc::FakeAdmin)
		LocalPlayer.BasePlayer->FakeAdmin(4);

	if (Vars::Misc::WalkOnWater && GetAsyncKeyState(0x56))
		LocalPlayer.BasePlayer->WalkWater();


}

