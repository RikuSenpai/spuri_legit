#include "misc.hpp"


void c_misc::remove_smoke() noexcept {
	if (!c_config::get().remove_smoke || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	static auto smoke_count = *reinterpret_cast<uint32_t **>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "A3 ? ? ? ? 57 8B CB") + 1);

	static std::vector<const char*> smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust"
	};

	for (auto material_name : smoke_materials) {
		i_material * smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
		smoke->increment_reference_count();
		smoke->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true);

		*(int*)smoke_count = 0;
	}
}

void c_misc::rank_reveal() noexcept
{
	if (!c_config::get().rank_reveal || !c_config::get().misc_enabled)
		return;
	if (GetKeyState(VK_TAB))
	{
		utilities::ServerRankRevealAll();
	}
}



void c_misc::keystrokes() noexcept
{
	if (!c_config::get().key_strokes || !c_config::get().misc_enabled)
		return;
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!interfaces::engine->is_connected())
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!local_player->is_alive())
		return;

	int w, h;
	int centerW, centerh, topH;
	interfaces::engine->get_screen_size(w, h);
	centerW = w / 2;
	centerh = h / 2;


	if (GetAsyncKeyState(0x57))
	{
		render::get().draw_text(25, (centerh + 20), render::get().name_font, "w", true, color(0, 255, 0));
	}
	else
	{
		render::get().draw_text(25, (centerh + 20), render::get().name_font, "w", true, color(255, 255, 255));
	}

	if (GetAsyncKeyState(0x41))
	{
		render::get().draw_text(15, (centerh + 30), render::get().name_font, "a", true, color(0, 255, 0));
	}
	else
	{
		render::get().draw_text(15, (centerh + 30), render::get().name_font, "a", true, color(255, 255, 255));
	}
	if (GetAsyncKeyState(0x53))
	{
		render::get().draw_text(25, (centerh + 30), render::get().name_font, "s", true, color(0, 255, 0));
	}
	else
	{
		render::get().draw_text(25, (centerh + 30), render::get().name_font, "s", true, color(255, 255, 255));
	}

	if (GetAsyncKeyState(0x44))
	{
		render::get().draw_text(35, (centerh + 30), render::get().name_font, "d", true, color(0, 255, 0));
	}
	else
	{
		render::get().draw_text(35, (centerh + 30), render::get().name_font, "d", true, color(255, 255, 255));
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		render::get().draw_text(25, (centerh + 40), render::get().name_font, "space", true, color(0, 255, 0));
	}
	else
	{
		render::get().draw_text(25, (centerh + 40), render::get().name_font, "space", true, color(255, 255, 255));
	}
	
}

void c_misc::fake_walk(c_usercmd* user_cmd) {

	if (!c_config::get().fake_walk)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player->flags() & fl_onground) {
		int movetype = local_player->move_type();
		if (!(movetype == 8 || movetype == 9)) {
			if (GetAsyncKeyState(c_config::get().fake_walk_key)) {
				static int choked = 0;
				choked = choked > 7 ? 0 : choked + 1;
				user_cmd->forwardmove = choked < 2 || choked > 5 ? 0 : user_cmd->forwardmove;
				user_cmd->sidemove = choked < 2 || choked > 5 ? 0 : user_cmd->sidemove;
				g::send_packet = choked < 1;
			}
		}
	}
}


void c_misc::velocity_draw() noexcept
{
	if (!c_config::get().draw_velocity || !c_config::get().misc_enabled)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!interfaces::engine->is_connected())
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!local_player->is_alive())
		return;


	
	char jew[64];
	float lspeed = local_player->velocity().Length2D();
	int w, h;
	int centerW, centerh, topH;

	interfaces::engine->get_screen_size(w, h);
	centerW = w / 2;
	centerh = h / 2;

	sprintf_s(jew, "velocity: %.1f", lspeed);
	render::get().draw_text(40, (centerh + 55), render::get().name_font, jew, true, color(255, 255, 255));
}

void c_misc::third_person() noexcept
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	static bool enabledtp = false, check = false;


	auto GetCorrectDistance = [&local_player](float ideal_distance) -> float
	{
		vec3_t inverse_angles;
		interfaces::engine->get_view_angles_alternative(inverse_angles);

		inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

		vec3_t direction;
		c_math::get().angle_vectors(inverse_angles, direction);

		trace_world_only filter;
		trace_t trace;
		ray_t ray;

		ray.initialize(local_player->origin() + local_player->view_offset(), (local_player->origin() + local_player->view_offset()) + (direction * (ideal_distance + 5.f)));
		interfaces::trace_ray->trace_ray(ray, MASK_ALL, &filter, &trace);

		return ideal_distance * trace.flFraction;
	};

	if (c_config::get().third_person && in_thirdperson)
	{
		if (local_player->health() <= 0)
			local_player->observer_mode() = 5;

		if (!interfaces::input->m_fCameraInThirdPerson)
		{
			interfaces::input->m_fCameraInThirdPerson = true;
			interfaces::input->m_vecCameraOffset = vec3_t(g::real_angles.x, g::real_angles.y, GetCorrectDistance(100));

			vec3_t camForward;
			c_math::get().angle_vectors_alternative(vec3_t(interfaces::input->m_vecCameraOffset.x, interfaces::input->m_vecCameraOffset.y, 0), &camForward);
		}
	}
	else
	{
		interfaces::input->m_fCameraInThirdPerson = false;
		interfaces::input->m_vecCameraOffset = vec3_t(g::real_angles.x, g::real_angles.y, 0);
	}
}

void c_misc::free_cam(c_viewsetup* setup) noexcept
{
	if (!c_config::get().free_cam || !c_config::get().misc_enabled)
		return;

	static vec3_t newOrigin;
	int freecamkey = c_config::get().free_cam_key;

	float freecamspeed = c_config::get().free_cam_speed;

	if (!GetAsyncKeyState(freecamkey))
	{
		newOrigin = setup->origin;
	}
	if (GetAsyncKeyState(freecamkey))
	{

		unsigned int fSpeed = freecamspeed; //5.f;
		if (GetAsyncKeyState(VK_CONTROL))
			fSpeed = fSpeed * 0.45;
		if (GetAsyncKeyState(VK_SHIFT))
			fSpeed = fSpeed * 1.65;
		if (GetAsyncKeyState(0x57)) // W		
		{
			newOrigin += setup->angles.forward() * fSpeed;
		}
		if (GetAsyncKeyState(0x41)) // A		
		{
			newOrigin += setup->angles.right() * fSpeed;
		}
		if (GetAsyncKeyState(0x44)) // D		
		{
			newOrigin -= setup->angles.right() * fSpeed;
		}
		if (GetAsyncKeyState(0x53)) // S		
		{
			newOrigin -= setup->angles.forward() * fSpeed;

		}
		if (GetAsyncKeyState(VK_SPACE)) // Space		
		{
			newOrigin += setup->angles.up() * fSpeed;
		}
		setup->origin = newOrigin;
	}

}

void c_misc::chat_spam() noexcept
{
	static DWORD lastspammed = 0;

	if (!c_config::get().chat_spam || !c_config::get().misc_enabled)
		return;

	std::vector<std::string> chatspam =
	{
		"1 tapped",
		"easy own",
		"consider buying a monitor",
		"why i see only bots in the enemy team?",
		"spin on my dick like a beyblade",
		"even your mom was better last night"
	};


	if (GetTickCount() - lastspammed > 1000)
	{
		lastspammed = GetTickCount();
		auto say = "say ";
		std::string p = say + chatspam[rand() % chatspam.size()];
		interfaces::engine->execute_cmd(p.c_str());
	}
}

void c_misc::name_spam() noexcept
{
	static DWORD delay = 0;

	if (c_config::get().name_spam == 0 || !c_config::get().misc_enabled)
		return;

	if (c_config::get().name_spam == 1)
	{
		utilities::change_name("\n\xAD\xAD\xAD");
	}

	if (c_config::get().name_spam == 2)
	{
		convar* name_change = interfaces::console->get_convar("name");
		utilities::change_name("\nspuri_framework\n");
		utilities::change_name("\n|spuri_framework\n");
		utilities::change_name("\n|spuri_framework|\n");
		utilities::change_name("\n$|spuri_framework|\n");
		utilities::change_name("\n$|spuri_framework|$\n");
	}

	if (c_config::get().name_spam == 3)
	{
		utilities::change_name("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\You will be kicked for inactivity. Press F1 to avoid getting kicked!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                                                                                                                        \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

		if (GetTickCount() - delay > 4000)
		{
			delay = GetTickCount();
			utilities::change_name("\n\xAD\xAD\xAD");
		}
	}

	if (c_config::get().name_spam == 4)
	{
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < .5)
			return;
		const char* result;
		std::vector <std::string> names;
		if (interfaces::engine->is_in_game() && interfaces::engine->is_in_game()) {
			for (int i = 1; i < interfaces::globals->max_clients; i++)
			{
				auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));


				player_info_t pInfo;

				if (entity && local_player->team() == entity->team())
				{
					c_client_class* cClass = (c_client_class*)entity->client_class();

					if (cClass->class_id == (int)ccsplayer)
					{
						if (interfaces::engine->get_player_info_alternative(i, &pInfo))
						{
							if (!strstr(pInfo.name, "GOTV"))
								names.push_back(pInfo.name);
						}
					}
				}
			}
		}

		utilities::change_name("\n\xAD\xAD\xAD");
		int randomIndex = rand() % names.size();
		char buffer[128];
		sprintf_s(buffer, "%s ", names[randomIndex].c_str());
		result = buffer;



		utilities::change_name(result);
		start_t = clock();
	}

	if (c_config::get().name_spam == 5)
	{
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		static clock_t start_t = clock();
		double time_so_far = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (time_so_far < .5)
			return;
		const char* result;
		std::vector <std::string> names;
		if (interfaces::engine->is_in_game() && interfaces::engine->is_in_game()) {
			for (int i = 1; i < interfaces::globals->max_clients; i++)
			{
				auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
				player_info_t pInfo;

				if (entity && local_player->team() == entity->team())
				{
					c_client_class* c_class = (c_client_class*)entity->client_class();

					if (c_class->class_id == (int)ccsplayer)
					{
						if (interfaces::engine->get_player_info_alternative(i, &pInfo))
						{
							if (!strstr(pInfo.name, "GOTV"))
								names.push_back(pInfo.name);
						}
					}
				}
			}
		}

		//set_name("\n\xAD\xAD\xAD");
		int random_index = rand() % names.size();
		char buffer[128];
		sprintf_s(buffer, "%s ", names[random_index].c_str());
		result = buffer;



		utilities::change_name(result);
		start_t = clock();

		if (GetTickCount() - delay > 4000)
		{
			delay = GetTickCount();
			utilities::change_name("\n\xAD\xAD\xAD");
		}
	}

}

void c_misc::walk_bot(vec3_t& OrigAng, c_usercmd* user_cmd) noexcept
{
	if (!c_config::get().walk_bot || !c_config::get().misc_enabled)
		return;

	bool walkbotBefore = false;
	
	vec3_t viewangles;
	interfaces::engine->get_view_angles_alternative(viewangles);

	static int OldMouseX = OrigAng.y;
	int mousedx = OldMouseX - OrigAng.y;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	auto fDistanceToWall = [&](vec3_t diff = vec3_t(0, 0, 0))->float {
		auto tmpviewangles = viewangles + diff;
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = local_player;
		vec3_t begin = local_player->get_eye_pos(), t, end;
		c_math::get().angle_vectors(tmpviewangles, t);
		normalizeinout(t, end);
		end *= 8192.0f;
		end += begin;
		ray.initialize(begin, end);
		interfaces::trace_ray->trace_ray(ray, 0x4600400B, &filter, &tr);
		return (begin - tr.end).size();
	};

	static float old1, old2, old3;
	if (local_player->velocity().length() < 3 && !user_cmd->buttons & in_attack)
	{
		viewangles.y += 2.0f;
	}
	if (local_player->velocity().length() < 40 && !user_cmd->buttons & in_attack)
	{
		viewangles.y += 15.0f;
	}
	float Distances = c_config::get().walk_bot_distance;
	if (fDistanceToWall() < Distances) // we are near to some wall
	{
		int turn = 1 + (rand() % 90);
		float negativeDist = fDistanceToWall(vec3_t(0, -1, 0)), positiveDist = fDistanceToWall(vec3_t(0, 1, 0));
		if (abs(negativeDist - positiveDist) < 1.0f)
		{
			viewangles.y += turn;
		}
		else
		{
			viewangles.y += positiveDist < negativeDist ? -1 : 1;
		}
		
	}

	if (abs(viewangles.x) > 0)
		viewangles.x = 1;

	while (viewangles.y > 180.0f)
		viewangles.y -= 360.0f;
	while (viewangles.y < -180.0f)
		viewangles.y += 360.0f;

	interfaces::engine->set_view_angles(viewangles);

	if (!walkbotBefore)
	{
		if (user_cmd->buttons & in_attack)
			user_cmd->forwardmove = 0.f;
		else
		user_cmd->forwardmove = 450.f;
		walkbotBefore = true;
	}

	else if (walkbotBefore)
	{
		walkbotBefore = false;
		if (user_cmd->buttons & in_attack)
			user_cmd->forwardmove = 0.f;
		else

		user_cmd ->forwardmove = 450.f;

	}
}

void c_misc::ragdoll() noexcept
{
	if (!c_config::get().ragdoll_gravity || !c_config::get().misc_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	bool applied = false;
	if (!applied) {
		auto ragdoll = interfaces::console->get_convar("cl_ragdoll_gravity");
		ragdoll->set_value(-1000);
		applied = true;
	}

}


void c_misc::remove_flash() noexcept {
	if (!c_config::get().reduce_flash || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player->flash_duration() > 0.0f)
		local_player->flash_duration() = 0.0f;
}

void c_misc::left_knife() noexcept
{
	if (!c_config::get().left_knife || !c_config::get().misc_enabled)
		return;

	static auto left_knife = interfaces::console->get_convar("cl_righthand");
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto weapon = local_player->active_weapon();

	if (!local_player || !local_player->is_alive())
	{
		left_knife->set_value(1);
		return;
	}

	if (weapon->is_knife(weapon))
	{
		left_knife->set_value(0);
	}
	else
	{
		left_knife->set_value(1);
	}
}

void c_misc::remove_scope() noexcept {
	if (!c_config::get().remove_scope || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (local_player && !local_player->is_scoped())
		return;

	int w, h;
	interfaces::engine->get_screen_size(w, h);
	interfaces::surface->set_drawing_color(0, 0, 0, 255);
	interfaces::surface->draw_line(0, h / 2, w, h / 2);
	interfaces::surface->draw_line(w / 2, 0, w / 2, h);
}

void c_misc::force_scope_overlay() noexcept
{
	if (!c_config::get().force_scope || !c_config::get().misc_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (GetAsyncKeyState(c_config::get().force_scope_key))
	{
		int w, h;
		interfaces::engine->get_screen_size(w, h);
		interfaces::surface->set_drawing_color(0, 0, 0, 255);
		interfaces::surface->draw_line(0, h / 2, w, h / 2);
		interfaces::surface->draw_line(w / 2, 0, w / 2, h);
	}
}

void c_misc::spectators() noexcept {
	if (!c_config::get().spectators_list || !c_config::get().misc_enabled)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	int spectator_index = 0;
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	render::get().draw_text(width - 80, height / 2 - 10, render::get().name_font, "spectators:", true, color(255, 255, 255));
	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		player_info_t info;

		if (entity && entity != local_player) {
			interfaces::engine->get_player_info(i, &info);
			if (!entity->is_alive() && !entity->dormant()) {
				auto target = entity->observer_target();
				if (target) {
					auto spectator_target = interfaces::entity_list->get_client_entity_handle(target);
					if (spectator_target == local_player) {
						
						std::string player_name = info.name;
						std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);
						player_info_t spectator_info;
						interfaces::engine->get_player_info(i, &spectator_info);
						render::get().draw_text(width - 80, height / 2 + (10 * spectator_index), render::get().name_font, player_name.c_str(), true, color(255, 255, 255));
						spectator_index++;
					}
				}
			}
		}
	}
}

void c_misc::watermark() noexcept 
{
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	static int fps, old_tick_count;

	if ((interfaces::globals->tick_count - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / interfaces::globals->frame_time);
		old_tick_count = interfaces::globals->tick_count;
	}
	std::stringstream ss;

	auto net_channel = interfaces::engine->get_net_channel_info();
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	std::string incoming = local_player ? std::to_string((int)(net_channel->get_latency(FLOW_INCOMING) * 1000)) : "0";
	std::string outgoing = local_player ? std::to_string((int)(net_channel->get_latency(FLOW_OUTGOING) * 1000)) : "0";

	ss << "spuri_framework | fps: " << fps << " | ping: " << outgoing.c_str() << "";

	render::get().draw_filled_rect(width - 200, 4, 195, 20, color(33, 35, 47, 255));
	render::get().draw_outline(width - 200, 4, 195, 20, color(30, 30, 41, 255));
	render::get().draw_text(width - 195, 7, render::get().name_font, ss.str().c_str(), false, color(255, 255, 255, 255));
}

void c_misc::clantag_spammer() noexcept {
	if (!c_config::get().clan_tag || !c_config::get().misc_enabled)
		return;

	static std::string tag = "spuri_framework ";
	static float last_time = 0;

	if (interfaces::globals->cur_time > last_time) {
		std::rotate(std::begin(tag), std::next(std::begin(tag)), std::end(tag));
		utilities::apply_clan_tag(tag.c_str());

		last_time = interfaces::globals->cur_time + 0.9f;
	}

	if (fabs(last_time - interfaces::globals->cur_time) > 1.f)
		last_time = interfaces::globals->cur_time;
}

void c_misc::viewmodel_offset() noexcept {
	if (!c_config::get().viewmodel_offset || !c_config::get().misc_enabled)
		return;

	interfaces::console->get_convar("viewmodel_offset_x")->set_value(c_config::get().viewmodel_x);
	interfaces::console->get_convar("viewmodel_offset_y")->set_value(c_config::get().viewmodel_y);
	interfaces::console->get_convar("viewmodel_offset_z")->set_value(c_config::get().viewmodel_z);
}

void c_misc::disable_post_processing() noexcept {
	if (!c_config::get().misc_enabled)
		return;

	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(c_config::get().disable_post_processing ? 0 : 1);
}

void c_misc::grenade_trajectory() noexcept {

	if (!c_config::get().nade_pred)
	{
		interfaces::console->get_convar("sv_grenade_trajectory")->set_value(0);
	}
		

	float value = 0.1;
	if (c_config::get().nade_pred)
	{
		interfaces::console->get_convar("sv_grenade_trajectory")->set_value(1);
		interfaces::console->get_convar("sv_grenade_trajectory_time")->set_value(1);
		interfaces::console->get_convar("sv_grenade_trajectory_thickness")->set_value(value);
	}

}

void c_misc::recoil_crosshair() noexcept {
	if (!c_config::get().misc_enabled)
		return;

	static auto cl_crosshair_recoil = interfaces::console->get_convar("cl_crosshair_recoil");
	cl_crosshair_recoil->set_value(c_config::get().recoil_crosshair ? 1 : 0);
}

void c_misc::force_crosshair() noexcept {
	if (!c_config::get().misc_enabled)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");

	if (local_player && local_player->health() > 0) {
		weapon_debug_spread_show->set_value(local_player->is_scoped() || !c_config::get().force_crosshair ? 0 : 3);
	}
}

void c_misc::aspect_ratio() noexcept {
	if (!c_config::get().aspect_ratio || !c_config::get().misc_enabled)
		return;

	float ratio = (c_config::get().aspect_ratio_value * 0.1) / 2;
	if (ratio > 0.001)
		interfaces::console->get_convar("r_aspectratio")->set_value(ratio);
	else
		interfaces::console->get_convar("r_aspectratio")->set_value((35 * 0.1f) / 2);
}