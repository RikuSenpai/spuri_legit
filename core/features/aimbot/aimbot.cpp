#define _USE_MATH_DEFINES
#include <cmath>
#include "aimbot.hpp"
#include "../../../source-sdk/sdk.hpp"
#include "../../../source-sdk/math/vector2d.hpp"
#include "../../../dependencies/common_includes.hpp"
#include "../../features/backtrack/backtrack.hpp"

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10


int c_aimbot::get_nearest_bone(player_t* entity, c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	float best_dist = 360.f;
	int aimbone;

	matrix_t matrix[MAXSTUDIOBONES];

	if (!entity->setup_bones(matrix, 128, BONE_USED_BY_HITBOX, 0.0f))
		return -1;

	studio_hdr_t* studio_model = interfaces::model_info->get_studio_model(entity->model());
	if (!studio_model)
		return -1;

	studio_hitbox_set_t* set = studio_model->hitbox_set(entity->hitbox_set());
	if (!set)
		return -1;

	for (int i = 0; i < set->hitbox_count; i++) {
		if (i >= hitbox_max)
			continue;

		studio_box_t* hitbox = set->hitbox(i);

		if (!hitbox)
			continue;

		auto angle = c_math::get().calculate_angle(local_player->get_eye_pos(), vec3_t(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3]), user_cmd->viewangles);
		auto this_dist = std::hypotf(angle.x, angle.y);

		if (best_dist > this_dist) {
			best_dist = this_dist;
			aimbone = hitbox->bone;
			continue;
		}
	}
	return aimbone;
}

void c_aimbot::weapon_settings(weapon_t* weapon) noexcept {
	if (!weapon)
		return;

	if (weapon->is_pistol(weapon)) {
		switch (c_config::get().aim_bone_pistol) {
		case 0:
			hitbox_id = hitbox_head;
			break;
		case 1:
			hitbox_id = hitbox_neck;
			break;
		case 2:
			hitbox_id = hitbox_chest;
			break;
		case 3:
			hitbox_id = hitbox_stomach;
			break;
		case 4:
			hitbox_id = hitbox_pelvis;
			break;
		}

		aim_smooth = c_config::get().aim_smooth_pistol * 6;
		aim_fov = c_config::get().aim_fov_pistol;

	}
	else if (weapon->is_rifle(weapon)) {
		switch (c_config::get().aim_bone_rifle) {
		case 0:
			hitbox_id = hitbox_head;
			break;
		case 1:
			hitbox_id = hitbox_neck;
			break;
		case 2:
			hitbox_id = hitbox_chest;
			break;
		case 3:
			hitbox_id = hitbox_stomach;
			break;
		case 4:
			hitbox_id = hitbox_pelvis;
			break;
		}

		aim_smooth = c_config::get().aim_smooth_rifle * 6;
		aim_fov = c_config::get().aim_fov_rifle;
		rcs_x = c_config::get().rcs_x_rifle;
		rcs_y = c_config::get().rcs_y_rifle;
	}
	else if (weapon->is_sniper(weapon)) {
		switch (c_config::get().aim_bone_sniper) {
		case 0:
			hitbox_id = hitbox_head;
			break;
		case 1:
			hitbox_id = hitbox_neck;
			break;
		case 2:
			hitbox_id = hitbox_chest;
			break;
		case 3:
			hitbox_id = hitbox_stomach;
			break;
		case 4:
			hitbox_id = hitbox_pelvis;
			break;
		}

		aim_smooth = c_config::get().aim_smooth_sniper * 6;
		aim_fov = c_config::get().aim_fov_sniper;

	}
	else if (weapon->is_heavy(weapon)) {
		switch (c_config::get().aim_bone_heavy) {
		case 0:
			hitbox_id = hitbox_head;
			break;
		case 1:
			hitbox_id = hitbox_neck;
			break;
		case 2:
			hitbox_id = hitbox_chest;
			break;
		case 3:
			hitbox_id = hitbox_stomach;
			break;
		case 4:
			hitbox_id = hitbox_pelvis;
			break;
		}

		aim_smooth = c_config::get().aim_smooth_heavy * 6;
		aim_fov = c_config::get().aim_fov_heavy;
		rcs_x = c_config::get().rcs_x_heavy;
		rcs_y = c_config::get().rcs_y_heavy;
	}
	else if (weapon->is_smg(weapon)) {
		switch (c_config::get().aim_bone_smg) {
		case 0:
			hitbox_id = hitbox_head;
			break;
		case 1:
			hitbox_id = hitbox_neck;
			break;
		case 2:
			hitbox_id = hitbox_chest;
			break;
		case 3:
			hitbox_id = hitbox_stomach;
			break;
		case 4:
			hitbox_id = hitbox_pelvis;
			break;
		}

		aim_smooth = c_config::get().aim_smooth_smg * 6;
		aim_fov = c_config::get().aim_fov_smg;
		rcs_x = c_config::get().rcs_x_smg;
		rcs_y = c_config::get().rcs_y_smg;
	}
}

int c_aimbot::find_target(c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto best_fov = aim_fov;
	auto best_target = 0;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		auto entity_bone_pos = entity->get_bone_position(get_nearest_bone(entity, user_cmd));
		auto local_eye_pos = local_player->get_eye_pos();
		auto distance = local_eye_pos.distance_to(entity_bone_pos);

		if (!entity || entity == local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity())
			continue;

		//find nearest target to crosshair using nearest bone (this should fix nearest bone selection)
		angle = c_math::get().calculate_angle(local_eye_pos, entity_bone_pos, user_cmd->viewangles);
		auto fov = c_config::get().aim_distance_based_fov ? c_math::get().distance_based_fov(distance, c_math::get().calculate_angle_alternative(local_eye_pos, entity_bone_pos), user_cmd) : std::hypotf(angle.x, angle.y);
		if (fov < best_fov) {
			best_fov = fov;
			best_target = i;
		}
	}
	return best_target;
}

void c_aimbot::rcs_standalone() noexcept
{
	if (!c_config::get().rcs_standalone)
	{
		int rcsamount = 2;
		return;
	}
		
	int rcsamount = 2 - (c_config::get().rcs_standalone_x);

	auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
	recoil_scale->set_value(rcsamount);
}

void c_aimbot::smart_switch(c_usercmd* user_cmd) noexcept
{
	if (!c_config::get().smart_switch || !c_config::get().misc_enabled)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto weapon = local_player->active_weapon();
	if (!interfaces::engine->is_connected())
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!local_player->is_alive())
		return;

	vec3_t position;
	if (auto target = find_target(user_cmd)) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(target));
		if (weapon->is_rifle(weapon) && !weapon->clip1_count() || weapon->is_smg(weapon) && !weapon->clip1_count() || weapon->is_heavy(weapon) && !weapon->clip1_count())
		{
			interfaces::engine->execute_cmd("slot2");
		}
	}
	else
		return;
}

void c_aimbot::triggerbot(c_usercmd* user_cmd, weapon_t* weapon)
{
	if (!c_config::get().trigger_enable)
		return;

	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	//we dont wanna aim without ammo in clip
	if (!weapon || !weapon->clip1_count())
		return;

	if (!local_player && !local_player->is_alive())
		return;

	if (weapon->is_bomb(weapon) || weapon->is_knife(weapon) || weapon->is_grenade(weapon))
		return;

	if (!GetAsyncKeyState(VK_XBUTTON1))
		return;

	vec3_t src, dst, forward;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	c_math::get().angle_vectors2(user_cmd->viewangles, &forward);

	//auto weapon = local_player->active_weapon();
	//weapon_info_t* weapon_data = weapon->get_weapon_data();

	forward *= weapon->get_weapon_data()->range;
	filter.skip = local_player;
	src = local_player->get_eye_pos();
	dst = src + forward;
	ray.initialize(src, dst);
	interfaces::trace_ray->trace_ray(ray, 0x46004003, &filter, &tr);

	vec3_t viewangles = user_cmd->viewangles;
	if (c_config::get().trigger_recoil)
		viewangles += local_player->aim_punch_angle() * 2.0f;

	vec3_t crosshair_forward;
	c_math::get().angle_vectors2(viewangles, &crosshair_forward);
	crosshair_forward *= 8000.f;

	if (!(tr.hit_entity))
		return;

	if (local_player->team() == tr.entity->team())
		return;

	int hitgroup = tr.hitGroup;
	bool didhit = false;


	if (c_config::get().trigger_hitbox_head)
	{
		if (hitgroup == HITGROUP_HEAD) didhit = true;
	}
	if (c_config::get().trigger_hitbox_body)
	{
		if (hitgroup == HITGROUP_CHEST || hitgroup == HITGROUP_STOMACH) didhit = true;
	}
	if (c_config::get().trigger_hitbox_arms)
	{
		if (hitgroup == HITGROUP_LEFTARM || hitgroup == HITGROUP_RIGHTARM) didhit = true;
	}
	if (c_config::get().trigger_hitbox_legs)
	{
		if (hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG) didhit = true;
	}

	if (delay >= 2 + c_config::get().trigger_delay && didhit)
	{
		delay = 0;
		user_cmd->buttons |= in_attack;
	}
	delay++;
}


void c_aimbot::run(c_usercmd* user_cmd) noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto weapon = local_player->active_weapon();
	//run misc aimbot stuff
	weapon_settings(weapon);


	//run aimbot
	if (c_config::get().aim_enabled && user_cmd->buttons & in_attack || c_config::get().aim_enabled && c_config::get().aim_auto_fire) {
		if (auto target = find_target(user_cmd)) {
			auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(target));

			//we dont wanna aim without ammo in clip
			if (!weapon || !weapon->clip1_count())
				return;

			//visible check
			if (!local_player->can_see_player_pos(entity, entity->wtf_am_i_doing()))
				return;


			if ( !c_config::get ( ).aim_team_check && entity->is_in_local_team ( ) )
				return;

			//smoke check
			if (!c_config::get().smoke_check && utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)))
				return;

			//weapon check
			if (weapon->is_knife(weapon) || weapon->is_grenade(weapon) || weapon->is_bomb(weapon))
				return;

			//scope check
			if (weapon->is_sniper(weapon) && !local_player->is_scoped() && !c_config::get().scope_aim)
				return;

			//basic rcs system
			auto recoil_scale = interfaces::console->get_convar("weapon_recoil_scale");
			auto aim_punch = local_player->aim_punch_angle() * recoil_scale->get_float();
			aim_punch.x *= rcs_x;
			aim_punch.y *= rcs_y;


			if (c_config::get().aim_auto_fire && !(user_cmd->buttons & in_attack))
			{
				user_cmd->buttons |= in_attack;

				if (user_cmd->buttons & in_attack)
				{
					static bool WasFiring = false;
					WasFiring = !WasFiring;

					if (WasFiring)
					{
						user_cmd->buttons &= ~in_attack;
					}
				}
			}

			if (c_config::get().aim_at_backtrack) {
				auto record = &records[entity->index()];
				if (record && record->size() && c_backtrack::get().valid_tick(record->front().simulation_time))
				{
					angle = c_math::get().calculate_angle(local_player->get_eye_pos(), record->back().head, user_cmd->viewangles + aim_punch);
				}

			}
			else {
				switch (c_config::get().aim_mode)
				{
				case 0:
					//hitbox
					angle = c_math::get().calculate_angle(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_id), user_cmd->viewangles + aim_punch);
					break;
				case 1:
					//nearest
					angle = c_math::get().calculate_angle(local_player->get_eye_pos(), entity->get_bone_position(get_nearest_bone(entity, user_cmd)), user_cmd->viewangles + aim_punch);
					break;
				}
			}

			angle /= aim_smooth;
			user_cmd->viewangles += angle;

			if (!c_config::get().aim_silent) {
				interfaces::engine->set_view_angles(user_cmd->viewangles);
			}
			
		}
	}
}
