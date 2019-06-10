#include "menu_new.hpp"
#include "cfgsaving.hpp"
#include "../../../dependencies/common_includes.hpp"
// credits to team gamer food

void CConfig::Setup()
{
	/* AIMBOT */
	SetupValue(g_menu.config.aimbot.Aimbot, false, "aimbot");
	SetupValue(g_menu.config.aimbot.AutoFire, false, "autofire");
	SetupValue(g_menu.config.aimbot.RCS, false, "rcs");
	SetupValue(g_menu.config.aimbot.Hitchance, 0, "hitchance");
	SetupValue(g_menu.config.aimbot.hitboxes, false, 5, "hitboxes");

	SetupValue(g_menu.config.aimbot.HitchanceRifle, 0, "hitchance rifle");
	SetupValue(g_menu.config.aimbot.HitchanceSniper, 0, "hitchance sniper");
	SetupValue(g_menu.config.aimbot.HitchancePistol, 0, "hitchance pistol");
	SetupValue(g_menu.config.aimbot.HitchanceAutosniper, 0, "hitchance autosniper");
	SetupValue(g_menu.config.aimbot.HitchanceShotgun, 0, "hitchance shotgun");
	SetupValue(g_menu.config.aimbot.HitchanceSMG, 0, "hitchance smg");
	SetupValue(g_menu.config.aimbot.HitchanceHeavy, 0, "hitchance heavy");

	SetupValue(g_menu.config.aimbot.MinimumDMGRifle, 0, "minimumdmg rifle");
	SetupValue(g_menu.config.aimbot.MinimumDMGSniper, 0, "minimumdmg sniper");
	SetupValue(g_menu.config.aimbot.MinimumDMGPistol, 0, "minimumdmg pistol");
	SetupValue(g_menu.config.aimbot.MinimumDMGAutosniper, 0, "minimumdmg autosniper");
	SetupValue(g_menu.config.aimbot.MinimumDMGShotgun, 0, "minimumdmg shotgun");
	SetupValue(g_menu.config.aimbot.MinimumDMGSMG, 0, "minimumdmg smg");
	SetupValue(g_menu.config.aimbot.MinimumDMGHeavy, 0, "minimumdmg heavy");


	SetupValue(g_menu.config.aimbot.Resolver, false, "resolver");
	SetupValue(g_menu.config.aimbot.BodyScale, 0, "bodyscale");
	SetupValue(g_menu.config.aimbot.HeadScale, 0, "headscale");
	SetupValue(g_menu.config.aimbot.MultiPoint, false, "multipoint");
	SetupValue(g_menu.config.aimbot.DelayShot, false, "delayshot");
	SetupValue(g_menu.config.aimbot.Autostop, false, "autostop");
	SetupValue(g_menu.config.aimbot.PosBacktrack, false, "posbacktrk");
	SetupValue(g_menu.config.aimbot.baim_lethal, false, "baimlethal");
	SetupValue(g_menu.config.aimbot.baim_air, false, "baiminair");
	SetupValue(g_menu.config.aimbot.Antiaim, false, "antiaimd");
	SetupValue(g_menu.config.aimbot.DesyncAngle, false, "desyncang");
	SetupValue(g_menu.config.aimbot.RandJitterInRange, false, "randjitterrng");
	SetupValue(g_menu.config.aimbot.JitterRange, 0, "jitterrange");
	SetupValue(g_menu.config.aimbot.yaw_stand, 0, "yaw stand");
	SetupValue(g_menu.config.aimbot.yaw_move, 0, "yaw move");
	SetupValue(g_menu.config.aimbot.pitch_stand, 0, "pitch stand");
	SetupValue(g_menu.config.aimbot.pitch_move, 0, "pitch move");
	SetupValue(g_menu.config.aimbot.stand_velocity_threshold, 0, "stand velocity threshold");
	/* AIMBOT */

	/* VISUALS */
	SetupValue(g_menu.config.visuals.Esp, false, "esp");

	SetupValue(g_menu.config.visuals.nightmode, 100, "nightmode");
	SetupValue(g_menu.config.visuals.propalpha, 100, "prop alpha");
	SetupValue(g_menu.config.visuals.worldalpha, 100, "world alpha");
	SetupValue(g_menu.config.visuals.Name, 0, "name");

	SetupValue(g_menu.config.visuals.glow, 0, "glow");
	SetupValue(g_menu.config.visuals.glow_alpha, 0, "glow alpha");
	SetupValue(g_menu.config.visuals.glow_color, 255.f, "glow color");

	SetupValue(g_menu.config.visuals.AutowallCross, false, "crosshair");
	SetupValue(g_menu.config.visuals.pene_dot, false, "penetration dot");
	SetupValue(g_menu.config.visuals.HealthVal, false, 2, "healthval");
	SetupValue(g_menu.config.visuals.Weapon, 0, "weapon");
	SetupValue(g_menu.config.visuals.ammo, false, 2, "ammo");

	SetupValue(g_menu.config.visuals.player_flags, 0, "player flags");
	SetupValue(g_menu.config.visuals.flags, false, 4, "flags");

	SetupValue(g_menu.config.visuals.Box, false, "box");
	SetupValue(g_menu.config.visuals.BoxColor, 255.f, "boxclr");
	SetupValue(g_menu.config.visuals.HealthBar, false, "healthbar");
	SetupValue(g_menu.config.visuals.Skeleton, false, "skeleton");
	SetupValue(g_menu.config.visuals.SkeletonColor, 255.f, "skeleton color");
	SetupValue(g_menu.config.visuals.Chams, false, "chams");
	SetupValue(g_menu.config.visuals.chammat, 0, "chams mat");
	SetupValue(g_menu.config.visuals.chams_opacity, 0, "chams opacity");

	SetupValue(g_menu.config.visuals.local_chams, false, "local chams");
	SetupValue(g_menu.config.visuals.localchammat, 0, "local chams material");
	SetupValue(g_menu.config.visuals.local_chams_opacity, 0, "local chams opacity");
	SetupValue(g_menu.config.visuals.localchamscolor, 255.f, "local chams color");
	SetupValue(g_menu.config.visuals.localpulse, false, "local pulse");

	SetupValue(g_menu.config.visuals.desynccolor, 255.f, "desync cham color");
	SetupValue(g_menu.config.visuals.desync_chams, false, "desync chams");

	SetupValue(g_menu.config.visuals.fovarrow, false, "out of fov arrow");
	SetupValue(g_menu.config.visuals.fov_arrows_color, 255.f, "out of fov arrow color");
	SetupValue(g_menu.config.visuals.fov_arrows_dia, 0, "out of fov arrow diameter");
	SetupValue(g_menu.config.visuals.fov_arrows_size, 0, "out of fov arrow size");
	SetupValue(g_menu.config.visuals.ChamsXQZ, false, "chams xqz");
	SetupValue(g_menu.config.visuals.ChamsColor, 255.f, "chamsclr");
	SetupValue(g_menu.config.visuals.ChamsColorXQZ, 255.f, "chamsclr xqz");
	SetupValue(g_menu.config.visuals.Fov, 90, "fov");

	SetupValue(g_menu.config.visuals.swing, 0, "viewmodel swing");
	SetupValue(g_menu.config.visuals.tilt, 0, "viewmodel tilt");
	SetupValue(g_menu.config.visuals.roll, 0, "viewmodel roll");
	SetupValue(g_menu.config.visuals.ViewmodelX, 0, "viewmodel x");
	SetupValue(g_menu.config.visuals.ViewmodelY, 0, "viewmodel y");
	SetupValue(g_menu.config.visuals.ViewmodelZ, 0, "viewmodel z");

	SetupValue(g_menu.config.visuals.nViewmodelX, 0, "neg viewmodel x");
	SetupValue(g_menu.config.visuals.nViewmodelY, 0, "neg viewmodel y");
	SetupValue(g_menu.config.visuals.nViewmodelZ, 0, "neg viewmodel z");




	SetupValue(g_menu.config.visuals.fakelagchams, false, "fakelag chams");
	SetupValue(g_menu.config.visuals.fakelagcolor, 255.f, "fakelag chams color");
	SetupValue(g_menu.config.visuals.fakelagchams_opacity, 255.f, "fakelag chams opacity");
	/* VISUALS */

	/* MISC */
	SetupValue(g_menu.config.misc.bunnyhop, 0, "bhop");
	SetupValue(g_menu.config.misc.bunnyhop_hitchance, 0, "bhop hitchance");
	SetupValue(g_menu.config.misc.bunnyhop_min, 0, "bhop min hops");
	SetupValue(g_menu.config.misc.bunnyhop_max, 0, "bhop max hops");
	SetupValue(g_menu.config.misc.autostrafe, 0, "autostrafe");
	SetupValue(g_menu.config.misc.edgejump, false, "edge jump");
	SetupValue(g_menu.config.misc.infiniteducks, false, "infinite duck");

	/* MISC */

}

void CConfig::SetupValue(int& value, int def, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(name, &value));
}

void CConfig::SetupValue(ColorV2& value, float def, std::string name)
{
	value.red = def;
	value.green = def;
	value.blue = def;
	value.alpha = def;

	floats.push_back(new ConfigValue< float >(name + "red", &value.red));
	floats.push_back(new ConfigValue< float >(name + "green", &value.green));
	floats.push_back(new ConfigValue< float >(name + "blue", &value.blue));
	floats.push_back(new ConfigValue< float >(name + "alpha", &value.alpha));
}

void CConfig::SetupValue(bool* value, bool def, int size, std::string name) // for multiboxes
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(name, &value[c]));
	}
}

static char name[] = " ";

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\happyhack\\";
		switch (g_menu.config.misc.ConfigList) {
		case 0: file = std::string(path) + "\\happyhack\\legit"; break; //legit
		case 1: file = std::string(path) + "\\happyhack\\rage"; break; //rage
		case 2: file = std::string(path) + "\\happyhack\\custom"; break; //custom
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString("happyhack_config", value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString("happyhack_config", value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString("happyhack_config", value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\happyhack\\";
		switch (g_menu.config.misc.ConfigList) {
		case 0: file = std::string(path) + "\\happyhack\\legit"; break; //legit
		case 1: file = std::string(path) + "\\happyhack\\rage"; break; //rage
		case 2: file = std::string(path) + "\\happyhack\\custom"; break; //custom
		}
	}

	if (g_menu.config.misc.ResetCFG) {
		file = std::string(path) + "\\happyhack\\blank";
	}
	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString("happyhack_config", value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString("happyhack_config", value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString("happyhack_config", value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* g_Config = new CConfig();