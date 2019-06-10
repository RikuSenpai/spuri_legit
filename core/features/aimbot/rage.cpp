#include "rage.hpp"

void c_ragebot::rcs_standalone() noexcept
{
	if (!c_config::get().no_recoil)
	{
		int rcsamount = 2;
		return;
	}
	auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
	recoil_scale->set_value(0);
}

void c_ragebot::run(c_usercmd* user_cmd) noexcept
{
	if (c_config::get().aim_auto_stop)
	{
		user_cmd->forwardmove = 0;
		user_cmd->sidemove = 0;
		user_cmd->upmove = 0;
	}

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto weapon = local_player->active_weapon();
	//run misc aimbot stuff
	//weapon_settings(weapon);


	//run aimbot
	if (c_config::get().rage_enable)
		//we dont wanna aim without ammo in clip
		if (!weapon || !weapon->clip1_count())
			return;

	//if (!c_config::get().aim_team_check && entity->is_in_local_team())
	//	return;


	//weapon check
	if (weapon->is_knife(weapon) || weapon->is_grenade(weapon) || weapon->is_bomb(weapon))
		return;

	//scope check
	if (weapon->is_sniper(weapon) && !local_player->is_scoped() && !c_config::get().scope_aim)
		return;

	rcs_standalone();
}