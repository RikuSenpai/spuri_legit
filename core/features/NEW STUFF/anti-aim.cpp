#include "anti-aim.hpp"


#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define TICK_INTERVAL			( interfaces::globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}


static bool left, right, back, frees = true;
void manual(c_usercmd* user_cmd)
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	if (GetAsyncKeyState(VK_XBUTTON1))
	{
		left = true;
		right = false;
		back = false;
	}
	else if (GetAsyncKeyState(VK_XBUTTON2))
	{
		left = false;
		right = true;
		back = false;
	}
	else if (GetAsyncKeyState(VK_MBUTTON))
	{
		left = false;
		right = false;
		back = true;
	}

	if (left)
		user_cmd->viewangles.y += 90;
	else if (right)
		user_cmd->viewangles.y -= 90;
	else if (back)
		user_cmd->viewangles.y -= 180;
}

void c_antiaim::fake_duck(c_usercmd* user_cmd) noexcept {
	static int cnt = 0;
	static bool do_ = false;
	if (c_config::get().fake_duck && GetAsyncKeyState(VK_CAPITAL)) {
		g::send_packet = false;
		g::disable_fakelag = false;

		if (cnt % 14 == 0)
			do_ = true;
		else if (cnt % 14 == 6)
			g::send_packet = true;
		else if (cnt % 14 == 7)
			do_ = false;

		if (do_)
			user_cmd->buttons |= in_duck;
		else
			user_cmd->buttons &= ~in_duck;

		cnt++;
	}
	else {
		do_ = false;
		cnt = 0;
		g::disable_fakelag = true;
	}
}

void c_antiaim::do_antiaim(c_usercmd* user_cmd) noexcept
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player) return;
	if (local_player->health() <= 0) return;
	auto weapon = local_player->active_weapon();
	if (!weapon) return; auto grenade = weapon->is_grenade(weapon);
	if (local_player->move_type() == movetype_ladder) return;
	if (user_cmd->buttons & in_use) return;
	if (weapon->is_grenade(weapon)) return;

	if (user_cmd->buttons & in_attack) return;
	if (!c_config::get().anti_aim) return;

	static float last_real; int local_flags = local_player->flags();

	switch (c_config::get().anti_aim_pitch)
	{
	case 0: 
		break;
	case 1: user_cmd->viewangles.x = 89.f; 
		break;
	}

	static float RealAng = 0;

	switch (c_config::get().anti_aim_yaw)
	{
	case 0: 
		break;
	case 1: manual(user_cmd); 
		break;
	}

	RealAng = user_cmd->viewangles.y;

	static bool jitter = false;
	if (c_config::get().desync)
	{
		if (g::send_packet)
		{
			float server_time = local_player->get_tick_base() * interfaces::globals->interval_per_tick;
			float time = TIME_TO_TICKS(server_time);

			while (time >= server_time)
				time = 0.f;

			float idk = rand() % 100;

			jitter = !jitter;
			if (time >= server_time / 2)
			{
				if (idk < 70)
				{
					if (!jitter)
						user_cmd->viewangles.y = RealAng + 55;

				}
				else
				{
					if (!jitter)
						user_cmd->viewangles.y = RealAng - 55;

				}
			}
			else
			{
				if (idk < 70)
				{
					if (jitter)
						user_cmd->viewangles.y = RealAng - c_config::get().desync_range;
				}
				else
				{
					if (jitter)
						user_cmd->viewangles.y = RealAng + c_config::get().desync_range;

				}
			}
		}
	}
}

void c_antiaim::fix_movement(c_usercmd *user_cmd, vec3_t wish_angle)  const noexcept {
	vec3_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	vec3_t viewangles = user_cmd->viewangles;

	c_math::get().angle_to_vectors(wish_angle, &view_fwd, &view_right, &view_up);
	c_math::get().angle_to_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

	const float v8 = std::sqrtf(view_fwd.x * view_fwd.x + view_fwd.y * view_fwd.y);
	const float v10 = std::sqrtf(view_right.x * view_right.x + view_right.y * view_right.y);
	const float v12 = std::sqrtf(view_up.z * view_up.z);

	const vec3_t norm_view_fwd(1.f / v8 * view_fwd.x, 1.f / v8 * view_fwd.y, 0.f);
	const vec3_t norm_view_right(1.f / v10 * view_right.x, 1.f / v10 * view_right.y, 0.f);
	const vec3_t norm_view_up(0.f, 0.f, 1.f / v12 * view_up.z);

	const float v14 = std::sqrtf(cmd_fwd.x * cmd_fwd.x + cmd_fwd.y * cmd_fwd.y);
	const float v16 = std::sqrtf(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y);
	const float v18 = std::sqrtf(cmd_up.z * cmd_up.z);

	const vec3_t norm_cmd_fwd(1.f / v14 * cmd_fwd.x, 1.f / v14 * cmd_fwd.y, 0.f);
	const vec3_t norm_cmd_right(1.f / v16 * cmd_right.x, 1.f / v16 * cmd_right.y, 0.f);
	const vec3_t norm_cmd_up(0.f, 0.f, 1.f / v18 * cmd_up.z);

	const float v22 = norm_view_fwd.x * user_cmd->forwardmove;
	const float v26 = norm_view_fwd.y * user_cmd->forwardmove;
	const float v28 = norm_view_fwd.z * user_cmd->forwardmove;
	const float v24 = norm_view_right.x * user_cmd->sidemove;
	const float v23 = norm_view_right.y * user_cmd->sidemove;
	const float v25 = norm_view_right.z * user_cmd->sidemove;
	const float v30 = norm_view_up.x * user_cmd->upmove;
	const float v27 = norm_view_up.z * user_cmd->upmove;
	const float v29 = norm_view_up.y * user_cmd->upmove;

	user_cmd->forwardmove = norm_cmd_fwd.x * v24 + norm_cmd_fwd.y * v23 + norm_cmd_fwd.z * v25
		+ (norm_cmd_fwd.x * v22 + norm_cmd_fwd.y * v26 + norm_cmd_fwd.z * v28)
		+ (norm_cmd_fwd.y * v30 + norm_cmd_fwd.x * v29 + norm_cmd_fwd.z * v27);

	user_cmd->sidemove = norm_cmd_right.x * v24 + norm_cmd_right.y * v23 + norm_cmd_right.z * v25
		+ (norm_cmd_right.x * v22 + norm_cmd_right.y * v26 + norm_cmd_right.z * v28)
		+ (norm_cmd_right.x * v29 + norm_cmd_right.y * v30 + norm_cmd_right.z * v27);

	user_cmd->upmove = norm_cmd_up.x * v23 + norm_cmd_up.y * v24 + norm_cmd_up.z * v25
		+ (norm_cmd_up.x * v26 + norm_cmd_up.y * v22 + norm_cmd_up.z * v28)
		+ (norm_cmd_up.x * v30 + norm_cmd_up.y * v29 + norm_cmd_up.z * v27);

	user_cmd->forwardmove = clamp(user_cmd->forwardmove, -450.f, 450.f);
	user_cmd->sidemove = clamp(user_cmd->sidemove, -450.f, 450.f);
	user_cmd->upmove = clamp(user_cmd->upmove, -320.f, 320.f);
}
c_antiaim* antiaim = new c_antiaim();