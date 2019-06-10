#include "../../../dependencies/common_includes.hpp"
#include "movement.hpp"
auto flags_backup = 0;
vec3_t view_angles;


void c_movement::bunnyhop(c_usercmd* user_cmd) noexcept {
	if (c_config::get().bunny_hop == 0)
		return;
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (c_config::get().bunny_hop == 1)
	{

		if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
			return;
		if (user_cmd->buttons & in_jump && !(local_player->flags() & fl_onground))
		{
			user_cmd->buttons &= ~in_jump;
		}
	}
	if (c_config::get().bunny_hop == 2)
	{
		const int hitchance = c_config::get().bunny_hop_hitchance;
		const int restrictlimit = 12;
		const int hoplimit = c_config::get().bunny_hop_maximum_value;
		const int minhop = c_config::get().bunny_hop_minimum_value;


		static int hops_restricted = 0;
		static int hops_hit = 0;
		if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
			return;

		if (user_cmd->buttons & in_jump && !(local_player->flags() & fl_onground))
		{
			user_cmd->buttons &= ~in_jump;
			hops_restricted = 0;
		}

		else if ((rand() % 100 > hitchance && hops_restricted < restrictlimit) || (hoplimit > 0 && hops_hit > hoplimit && minhop > 0 && hops_hit > minhop))
		{
			user_cmd->buttons &= ~in_jump;
			hops_restricted++;
			hops_hit = 0;
		}
		else
		{
			hops_hit++;
		}
	}
}

void c_movement::auto_strafe(c_usercmd* user_cmd) noexcept
{
	if (c_config::get().auto_strafers == 0)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));


	if (c_config::get().auto_strafers == 1)
	{
		bool bKeysPressed = true;
		if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x57) || GetAsyncKeyState(0x53) || GetAsyncKeyState(0x44))
			bKeysPressed = false;

		if (!(local_player->flags() & fl_onground) && bKeysPressed)
		{
			user_cmd->forwardmove = (1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (user_cmd->forwardmove > 450.f)
				user_cmd->forwardmove = 450.f;
			interfaces::engine->get_view_angles_alternative(view_angles);

		}
	}
	if (c_config::get().auto_strafers == 2)
	{
		if (!(local_player->flags() & fl_onground))
		{
			if (GetAsyncKeyState(0X57)) //w
			{

				user_cmd->forwardmove = (1550.f * 5) / local_player->velocity().Length2D();
				user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (user_cmd->forwardmove > 450.f)
					user_cmd->forwardmove = 450.f;

			}
			else if (GetAsyncKeyState(0X41)) //a
			{

				user_cmd->sidemove = (-1550.f * 5) / local_player->velocity().Length2D();
				user_cmd->forwardmove = (user_cmd->command_number % 2) == 0 ? 450.f : -450.f;
				if (user_cmd->sidemove < -450.f)
					user_cmd->sidemove = -450.f;
			}
			else if (GetAsyncKeyState(0x44)) //d
			{
				user_cmd->sidemove = (1550.f * 5) / local_player->velocity().Length2D();
				user_cmd->forwardmove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (user_cmd->sidemove > 450.f)
					user_cmd->sidemove = 450.f;
			}
			else if (GetAsyncKeyState(0x53)) //s
			{
				user_cmd->forwardmove = (-1550.f * 5) / local_player->velocity().Length2D();
				user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? 450.f : -450.f;
				if (user_cmd->forwardmove < -450.f)
					user_cmd->forwardmove = -450.f;
			}
			else //default
			{
				user_cmd->forwardmove = (1550.f * 5) / local_player->velocity().Length2D();
				user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (user_cmd->forwardmove > 450.f)
					user_cmd->forwardmove = 450.f;
			}
		}
	}

}

void c_movement::edge_jump_pre_prediction(c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!c_config::get().edge_jump)
		return;

	if (!GetAsyncKeyState(0x45))
		return;

	if (!local_player)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	flags_backup = local_player->flags();
}

void c_movement::edge_jump_post_prediction(c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!c_config::get().edge_jump)
		return;

	if (!GetAsyncKeyState(0x45))
		return;

	if (!local_player)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	if (flags_backup & fl_onground && !(local_player->flags() & fl_onground))
		user_cmd->buttons |= in_jump;

	if (!(local_player->flags() & fl_onground) && c_config::get().edge_jump_duck_in_air)
		user_cmd->buttons |= in_duck;
}

void c_movement::infinite_duck(c_usercmd* user_cmd) noexcept
{
	if (!c_config::get().infinite_duck)
		return;

	user_cmd->buttons |= in_bullrush;
}