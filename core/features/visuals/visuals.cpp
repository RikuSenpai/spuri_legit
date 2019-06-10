#include "visuals.hpp"
#include "../../../dependencies/common_includes.hpp"

void c_visuals::run() noexcept {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!c_config::get().visuals_enabled)
		return;

	if (c_config::get().anti_screenshot && interfaces::engine->is_taking_screenshot())
		return;

	if (!local_player)
		return;

	//player drawing loop
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));


		if (!entity)
			continue;

		if (entity == local_player)
			continue;

		if (entity->health() <= 0)
			continue;

		if (c_config::get().radar) {
			entity->spotted() = true;
		}

		

		if (entity->team() == local_player->team() && !c_config::get().visuals_team_check)
			continue;

		const int fade = (int)((6.66666666667f * interfaces::globals->frame_time) * 255);

		auto new_alpha = alpha[i];
		new_alpha += entity->dormant() ? -fade : fade;

		if (new_alpha > (entity->has_gun_game_immunity() ? 130 : 210))
			new_alpha = (entity->has_gun_game_immunity() ? 130 : 210);
		if (new_alpha < c_config::get().player_dormant ? 50 : 0)
			new_alpha = c_config::get().player_dormant ? 50 : 0;

		alpha[i] = new_alpha;

		player_rendering(entity);
		skeleton(entity);
		backtrack_skeleton(entity);
		last_dormant[i] = entity->dormant();
	}

	//non player drawing loop
	for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (entity && entity != local_player) {
			auto client_class = entity->client_class();
			auto model_name = interfaces::model_info->get_model_name(entity->model());

			if (client_class->class_id == class_ids::cplantedc4) { //this should be fixed in better ways than this - designer
				bomb_esp(entity);
			}

			entity_esp(entity);
			dropped_weapons(entity);
			projectiles(entity);

		}
	}
}

void c_visuals::fov_crosshair() noexcept
{
	if (!c_config::get().fov_crosshair || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto weapon = local_player->active_weapon();

	if (!local_player->is_alive())
		return;

	if (!weapon)
		return;

	int xs;
	int ys;
	float FoV;
	player_t* entity;

	std::vector<int> HitBoxesToScan;

	if (weapon->is_pistol(weapon))
	{
		FoV = c_config::get().aim_fov_pistol + 0.5;
	}
	else if (weapon->is_sniper(weapon))
	{
		FoV = c_config::get().aim_fov_sniper + 0.5;

	}
	else if (weapon->is_rifle(weapon))
	{
		FoV = c_config::get().aim_fov_rifle + 0.5;
	}
	else if (weapon->is_smg(weapon))
	{
		FoV = c_config::get().aim_fov_smg + 0.5;
	}
	else
	{
		FoV = 360;
	}

	interfaces::engine->get_screen_size(xs, ys);
	interfaces::engine->get_screen_size(xs, ys);
	xs /= 2; ys /= 2;

	render::get().draw_circle(xs, ys, FoV * 6.2, FoV * 6.2, color(66, 134, 244, 255));
}

void c_visuals::anti_aim_indicator() noexcept
{
	if (!c_config::get().anti_aim)
		return;

	int W, H, cW, cH;
	interfaces::engine->get_screen_size(W, H);
	cW = W / 2;
	cH = H / 2;

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	static bool left = false;
	static bool right = false;
	static bool back = false;
	static bool free = false;

	if (c_config::get().anti_aim_yaw == 1)
	{

		if (GetAsyncKeyState(VK_XBUTTON1))
		{
			left = true;
			right = false;
			back = false;
			free = false;
		}
		else
		{
		}
		if (GetAsyncKeyState(VK_XBUTTON2))
		{
			left = false;
			right = true;
			back = false;
			free = false;
		}
		else
		{
		}
		if (GetAsyncKeyState(VK_MBUTTON))
		{
			left = false;
			right = false;
			back = true;
			free = false;
		}
		else
		{
		}
		{
		}
		{
			if (left)
			{
				render::get().old_text(cW + 30, cH - 0, L">", render::get().name_font, color(255, 255, 255, 200));
				render::get().old_text(cW - 38, cH + 0, L"<", render::get().name_font, color(177, 0, 0));
				render::get().old_text(cW - 3.5, cH + 30, L"v", render::get().name_font, color(255, 255, 255, 200));
			}
			else if (right)
			{
				render::get().old_text(cW + 30, cH - 0, L">", render::get().name_font, color(177, 0, 0));
				render::get().old_text(cW - 38, cH + 0, L"<", render::get().name_font, color(255, 255, 255, 200));
				render::get().old_text(cW - 3.5, cH + 30, L"v", render::get().name_font, color(255, 255, 255, 200));
			}
			else if (back)
			{
				render::get().old_text(cW + 30, cH - 0, L">", render::get().name_font, color(255, 255, 255, 200));
				render::get().old_text(cW - 38, cH + 0, L"<", render::get().name_font, color(255, 255, 255, 200));
				render::get().old_text(cW - 3.5, cH + 30, L"v", render::get().name_font, color(177, 0, 0));
			}
		}
	}
}

void c_visuals::grenade_pred() noexcept
{
	if (!c_config::get().grenade_prediction || !c_config::get().visuals_enabled)
		return;

	if (!interfaces::engine->is_connected())
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!interfaces::inputsystem->is_button_down(ButtonCode_t::MOUSE_LEFT))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));


	if (!local_player->is_alive())
		return;


	auto weapon = local_player->active_weapon();
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	vec3_t vPos;
	vec3_t vStart;
	vec3_t vThrow;
	vec3_t vThrow2;
	int iCollisions = 0;

	vec3_t vViewAngles;
	interfaces::engine->get_view_angles_alternative(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	c_math::get().angle_vectors2(vThrow, &vThrow2);

	vec3_t vEye = local_player->get_eye_pos();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + local_player->velocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + local_player->velocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + local_player->velocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		ray_t ray;
		trace_t tr;

		trace_filter loc;
		loc.skip = local_player;

		ray.initialize(vStart, vPos);
		interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &loc, &tr);

		if (tr.did_hit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *dot_product(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2 * tr.flFraction*fStep;

			fTime += (fStep * (1 - tr.flFraction));
		} // end of bounce
		auto red = c_config::get().clr_grenadepred[0] * 255;
		auto green = c_config::get().clr_grenadepred[1] * 255;
		auto blue = c_config::get().clr_grenadepred[2] * 255;
		auto alpha = c_config::get().clr_grenadepred[3] * 255;
		vec3_t vOutStart, vOutEnd;
		if (weapon->is_grenade(weapon))
		{
			//if (user_cmd->buttons & in_attack || user_cmd->buttons & in_attack2)
			if (c_math::get().world_to_screen(vStart, vOutStart), c_math::get().world_to_screen(vPos, vOutEnd))
			{
				render::get().draw_line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, color(red, green, blue, alpha));
			}

			vStart = vPos;
			vThrow2.z -= fGravity * tr.flFraction*fStep;
		}
	}
}

void c_visuals::entity_esp(player_t* entity) noexcept {
	if (!c_config::get().entity_esp)
		return;

	if (!entity)
		return;

	if (entity->dormant())
		return;

	auto model_name = interfaces::model_info->get_model_name(entity->model());
	vec3_t entity_position, entity_origin;
	entity_origin = entity->origin();
	auto client_class = entity->client_class();

	if (!c_math::get().world_to_screen(entity_origin, entity_position))
		return;

	//if (client_class->class_id == class_ids::cchicken) {
	//	render::get().draw_text(entity_position.x, entity_position.y, render::get().name_font, "chicken", true, color(255, 255, 255));
	//}

	else if (strstr(model_name, "dust_soccer_ball001")) {
		render::get().draw_text(entity_position.x, entity_position.y, render::get().name_font, "soccer ball", true, color(255, 255, 255));
	}

	else if (client_class->class_id == class_ids::chostage) {
		render::get().draw_text(entity_position.x, entity_position.y, render::get().name_font, "hostage", true, color(255, 255, 255));
	}
}

void c_visuals::player_rendering(player_t* entity) noexcept {
	if ((entity->dormant() && alpha[entity->index()] == 0) && !c_config::get().player_dormant)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	box bbox;
	if (!get_playerbox(entity, bbox))
		return;

	if (!GetAsyncKeyState(c_config::get().esp_hold_key) && c_config::get().esp_hold)
		return;

	if (c_config::get().dead_esp && local_player->is_alive())
		return;

	if (c_config::get().dormant_check && !local_player->can_see_player_pos(entity, entity->get_eye_pos()))
		return;



	if (c_config::get().player_box) {
		auto red = c_config::get().clr_box[0] * 255;
		auto green = c_config::get().clr_box[1] * 255;
		auto blue = c_config::get().clr_box[2] * 255;

		render::get().draw_outline(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, 255 + alpha[entity->index()]));
		render::get().draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color(red, green, blue, alpha[entity->index()]));
		render::get().draw_outline(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, 255 + alpha[entity->index()]));
	}
	if (c_config::get().player_health) {
		box temp(bbox.x - 5, bbox.y + (bbox.h - bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100.f) / 100.f)), 1, bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
		box temp_bg(bbox.x - 5, bbox.y, 1, bbox.h);

		// change the color depending on the entity health
		auto health_color = color( ( 255 - entity->health() * 2.55 ), ( entity->health() * 2.55 ), 0, alpha[entity->index()] );

		// clamp health (custom maps, danger zone, etc)
		if ( entity->health() > 100 )
		    	health_color = color( 0, 255, 0 );

		//draw actual dynamic hp bar
		render::get().draw_filled_rect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color(0, 0, 0, 25 + alpha[entity->index()]));
		render::get().draw_filled_rect(temp.x, temp.y, temp.w, temp.h, color(health_color));
	}
	if (c_config::get().player_name) {
		auto red = c_config::get().clr_name[0] * 255;
		auto green = c_config::get().clr_name[1] * 255;
		auto blue = c_config::get().clr_name[2] * 255;


		std::string print(info.fakeplayer ? std::string("bot ").append(info.name).c_str() : info.name);
		std::transform(print.begin(), print.end(), print.begin(), ::tolower);

		render::get().draw_text(bbox.x + (bbox.w / 2), bbox.y - 13, render::get().name_font, print, true, color(red, green, blue, alpha[entity->index()]));
	}
	{
		std::vector<std::pair<std::string, color>> flags;
		

		if (c_config::get().player_flags_armor && entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("hk", color(255, 255, 255, alpha[entity->index()])));
		else if (c_config::get().player_flags_armor && !entity->has_helmet() && entity->armor() > 0)
			flags.push_back(std::pair<std::string, color>("k", color(255, 255, 255, alpha[entity->index()])));

		if (c_config::get().player_flags_money && entity->money())
			flags.push_back(std::pair<std::string, color>(std::string("$").append(std::to_string(entity->money())), color(120, 180, 10, alpha[entity->index()])));

		if (c_config::get().player_flags_scoped && entity->is_scoped())
			flags.push_back(std::pair<std::string, color>(std::string("zoom"), color(80, 160, 200, alpha[entity->index()])));

		if (c_config::get().player_flags_c4 && entity->has_c4())
			flags.push_back(std::pair<std::string, color>(std::string("bomb"), color(255, 255, 255, alpha[entity->index()])));

		if (c_config::get().player_flags_flashed && entity->is_flashed())
			flags.push_back(std::pair<std::string, color>(std::string("flashed"), color(255, 255, 255, alpha[entity->index()])));

		if (utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)) && c_config::get().player_flags_behind_smoke)
			flags.push_back(std::pair<std::string, color>(std::string("behind smoke"), color(255, 255, 255, alpha[entity->index()])));

		if (c_config::get().vulnerability_check)
		{
		weapon_t* enemy_wep = entity->active_weapon_enemy();
		if (!enemy_wep) 
			return;
		int ammo = enemy_wep->clip1_count();
		


			if (entity->is_flashed())
				flags.push_back(std::pair<std::string, color>(std::string("flashed"), color(255, 255, 255, alpha[entity->index()])));

			if (entity->health() < 30)
				flags.push_back(std::pair<std::string, color>(std::string("low hp"), color(255, 255, 255, alpha[entity->index()])));

			if (5 > ammo && !enemy_wep->is_knife(enemy_wep))
				flags.push_back(std::pair<std::string, color>(std::string("low ammo"), color(255, 255, 255, alpha[entity->index()])));

			if (0 == ammo && !enemy_wep->is_knife(enemy_wep))
				flags.push_back(std::pair<std::string, color>(std::string("reloading"), color(255, 255, 255, alpha[entity->index()])));

		}
		

			

		auto position = 0;
		for (auto text : flags) {
			render::get().draw_text(bbox.x + bbox.w + 3, bbox.y + position - 2, render::get().name_font, text.first, false, text.second);
			position += 10;
		}
	}
	if (c_config::get().player_weapon) {
		auto red = c_config::get().clr_weapon[0] * 255;
		auto green = c_config::get().clr_weapon[1] * 255;
		auto blue = c_config::get().clr_weapon[2] * 255;
		auto weapon = entity->active_weapon();

		if (!weapon)
			return;

		std::string names;
		names = clean_item_name(weapon->get_weapon_data()->weapon_name);

		render::get().draw_text(bbox.x + (bbox.w / 2), bbox.h + bbox.y + 2, render::get().name_font, names, true, color(red, green, blue, alpha[entity->index()]));


	}
}

void c_visuals::dropped_weapons(player_t* entity) noexcept {
	auto class_id = entity->client_class()->class_id;
	auto model_name = interfaces::model_info->get_model_name(entity->model());
	auto weapon = entity;

	vec3_t dropped_weapon_position, dropped_weapon_origin;

	dropped_weapon_origin = weapon->origin();

	if (!c_math::get().world_to_screen(dropped_weapon_origin, dropped_weapon_position))
		return;

	auto red = c_config::get().clr_dropped_weapon[0] * 255;
	auto green = c_config::get().clr_dropped_weapon[1] * 255;
	auto blue = c_config::get().clr_dropped_weapon[2] * 255;
	auto alpha = c_config::get().clr_dropped_weapon[3] * 255;

	if (!(entity->origin().x == 0 && entity->origin().y == 0 && entity->origin().z == 0)) { //ghetto fix sorry - designer
		if (c_config::get().dropped_weapons) {
			if (strstr(entity->client_class()->network_name, ("CWeapon"))) {
				std::string data = strstr(entity->client_class()->network_name, ("CWeapon"));
				std::transform(data.begin(), data.end(), data.begin(), ::tolower); //convert dropped weapons names to lowercase, looks cleaner - designer
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, clean_item_name(data), true, color(red, green, blue, alpha));
			}

			if (class_id == class_ids::cak47)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "ak47", true, color(red, green, blue, alpha));

			if (class_id == class_ids::cc4)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "bomb", true, color(red, green, blue, alpha));

			if (class_id == class_ids::cdeagle)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "deagle", true, color(red, green, blue, alpha));

			if (strstr(model_name, "w_defuser"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "defuse kit", true, color(red, green, blue, alpha));
		}

		if (c_config::get().danger_zone_dropped) { 	//no need to create separate func for danger zone shit - designer (also use switch instead of else if)
			if (strstr(model_name, "case_pistol"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "pistol case", true, color(255, 152, 56));

			else if (strstr(model_name, "case_light_weapon"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "light case", true, color(255, 152, 56));

			else if (strstr(model_name, "case_heavy_weapon"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "heavy case", true, color(255, 152, 56));

			else if (strstr(model_name, "case_explosive"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "explosive case", true, color(255, 152, 56));

			else if (strstr(model_name, "case_tools"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "tools case", true, color(255, 152, 56));

			else if (strstr(model_name, "random"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "airdrop", true, color(255, 255, 255));

			else if (strstr(model_name, "dz_armor_helmet"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "armor, helmet", true, color(66, 89, 244));

			else if (strstr(model_name, "dz_helmet"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "helmet", true, color(66, 89, 244));

			else if (strstr(model_name, "dz_armor"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "armor", true, color(66, 89, 244));

			else if (strstr(model_name, "upgrade_tablet"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "tablet upgrade", true, color(255, 255, 255));

			else if (strstr(model_name, "briefcase"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "briefcase", true, color(255, 255, 255));

			else if (strstr(model_name, "parachutepack"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "parachutepack", true, color(255, 255, 255));

			else if (strstr(model_name, "dufflebag"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "cash bag", true, color(120, 180, 10));

			else if (strstr(model_name, "ammobox"))
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "ammobox", true, color(158, 48, 255));

			else if (class_id == class_ids::cdrone)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "drone", true, color(255, 255, 255));

			else if (class_id == class_ids::cphyspropradarjammer)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "radar jammer", true, color(255, 255, 255));

			else if (class_id == class_ids::cdronegun)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "sentry turret", true, color(255, 30, 30));

			else if (class_id == class_ids::cknife)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "knife", true, color(255, 255, 255));

			else if (class_id == class_ids::cmelee)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "melee", true, color(255, 255, 255));

			else if (class_id == class_ids::citemcash)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "cash", true, color(120, 180, 10));

			else if (class_id == class_ids::citem_healthshot)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "health shot", true, color(66, 89, 244));

			else if (class_id == class_ids::ctablet)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "tablet", true, color(255, 255, 255));

			else if (class_id == class_ids::chostage)
				render::get().draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::get().name_font, "hostage", true, color(255, 255, 255));
		}
	}
}

void c_visuals::projectiles(player_t* entity) noexcept {
	if (!c_config::get().projectiles)
		return;

	if (!entity)
		return;

	auto client_class = entity->client_class();
	auto model = entity->model();

	if (!model)
		return;

	if (model) {
		vec3_t grenade_position, grenade_origin;

		auto model = interfaces::model_info->get_studio_model(entity->model());

		if (!model || !strstr(model->name_char_array, "thrown") && !strstr(model->name_char_array, "dropped"))
			return;

		std::string name = model->name_char_array;
		grenade_origin = entity->origin();

		if (!c_math::get().world_to_screen(grenade_origin, grenade_position))
			return;

		if (name.find("flashbang") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "flashbang", true, color(255, 255, 255));
		}

		else if (name.find("smokegrenade") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "smoke", true, color(255, 255, 255));

			auto time = interfaces::globals->interval_per_tick * (interfaces::globals->tick_count - entity->smoke_grenade_tick_begin());

			if (!(18 - time < 0)) {
				render::get().draw_filled_rect(grenade_position.x - 18, grenade_position.y + 13, 36, 3, color(10, 10, 10, 180));
				render::get().draw_filled_rect(grenade_position.x - 18, grenade_position.y + 13, time * 2, 3, color(167, 24, 71, 255));
			}
		}

		else if (name.find("incendiarygrenade") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "incendiary", true, color(255, 255, 255));
		}

		else if (name.find("molotov") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "molotov", true, color(255, 255, 255));
		}

		else if (name.find("fraggrenade") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "grenade", true, color(255, 255, 255));
		}

		else if (name.find("decoy") != std::string::npos) {
			render::get().draw_text(grenade_position.x, grenade_position.y, render::get().name_font, "decoy", true, color(255, 255, 255));
		}
	}
}

void c_visuals::bomb_esp(player_t* entity) noexcept {
	if (!c_config::get().bomb_planted)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	auto class_id = entity->client_class()->class_id;
	auto explode_time = entity->c4_blow_time();

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	vec3_t bomb_origin, bomb_position;

	bomb_origin = entity->origin();

	explode_time -= interfaces::globals->interval_per_tick * local_player->get_tick_base();
	if (explode_time <= 0)
		explode_time = 0;

	char buffer[64];
	sprintf_s(buffer, "explodes in: %.1f", explode_time);

	auto c4_timer = interfaces::console->get_convar("mp_c4timer")->get_int();
	auto value = (explode_time * height) / c4_timer;

	//bomb damage indicator calculations, credits casual_hacker
	float damage;
	float damage2;
	auto distance = local_player->get_eye_pos().distance_to(entity->get_eye_pos());
	auto a = 450.7f;
	auto b = 75.68f;
	auto c = 789.2f;
	auto d = ((distance - b) / c);
	auto fl_damage = a * exp(-d * d);
	damage = float((std::max)((int)ceilf(utilities::csgo_armor(fl_damage, local_player->armor())), 0));
	damage2 = float((std::max)((int)ceilf(utilities::csgo_armor(fl_damage, local_player->health())), 0));


	//convert damage to string
	std::string damage_text;
	damage_text += "damage: -";
	damage_text += std::to_string((int)(damage));
	damage_text += "hp";

	std::string damage_text2;
	damage_text2 += "remain: ";
	damage_text2 += std::to_string((int)((damage2 - local_player->health() + 1)*(-1)));
	damage_text2 += "hp";

	std::string defusing;
	defusing += "being defused";




	//render on screen bomb bar
	if (explode_time >= 10) {
		render::get().gradient_horizontal(0, 0, 7, (value / 3), color(10, 10, 10, 255), color(26, 130, 228, 255));
	}
	
	else if (explode_time <= 10) {
	render::get().gradient_horizontal(0, 0, 7, (value / 3), color(10, 10, 10, 255), color(240, 20, 20, 255));
	}

	else if (!explode_time >= 0.01) {
		return;
	}

		

	//render bomb timer
	if (explode_time >= 0.01)
	{
		render::get().draw_text(3, 500, render::get().name_font, buffer, false, color(255, 255, 255));
	}

	//render bomb damage
	//if (local_player->is_alive() && explode_time >= 0.01)
	//{
	//	render::get().draw_text(11, 320, render::get().name_font, damage_text, false, color(255, 255, 255));
	//}

	//render enemy defusing

	//render health remaining
	if (local_player->is_alive() && damage2 <= 100 && explode_time >= 0.01)
	{
		render::get().draw_text(3, 510, render::get().name_font, damage_text2, false, color(255, 255, 255));
	}
	else
	{
		if (local_player->is_alive() && explode_time >= 0.01)
		render::get().draw_text(3, 510, render::get().name_font, "remain: 0hp", false, color(255, 255, 255));
	}

	if (!c_math::get().world_to_screen(bomb_origin, bomb_position))
		return;

	//render classic world timer + bar

	render::get().draw_text(bomb_position.x, bomb_position.y, render::get().name_font, buffer, true, color(255, 255, 255));
	render::get().draw_filled_rect(bomb_position.x - c4_timer / 2, bomb_position.y + 13, c4_timer, 3, color(10, 10, 10, 180)); //c4_timer / 2 so it always will be centered
	render::get().draw_filled_rect(bomb_position.x - c4_timer / 2, bomb_position.y + 13, explode_time, 3, color(167, 24, 71, 255));

}

void c_visuals::chams() noexcept
{
	if (!c_config::get().visuals_enabled || (!c_config::get().vis_chams_vis && !c_config::get().vis_chams_invis))
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;
		if (local_player && entity == local_player)
			continue;
		if (entity->health() <= 0)
			continue;
		if (entity->team() == local_player->team())
			continue;

		static i_material* mat = nullptr;
		auto textured = interfaces::material_system->find_material("aristois_material", TEXTURE_GROUP_MODEL, true, nullptr);
		auto crystal = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL, true, nullptr);
		auto dogtag = interfaces::material_system->find_material("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);
		auto gold = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL);
		auto crystal_blue = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_MODEL);
		auto metallic = interfaces::material_system->find_material("aristois_reflective", TEXTURE_GROUP_MODEL, true, nullptr);
		static i_material* matgloss = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); //you can create your own material to really make it shiny, but i like this subtle one
	

		textured->increment_reference_count();  //we need increment_reference_count cuz without it our materialsystem.dll will crash after  map change - designer
		crystal->increment_reference_count();
		dogtag->increment_reference_count();
		gold->increment_reference_count();
		crystal_blue->increment_reference_count();
		matgloss->increment_reference_count();
		metallic->increment_reference_count();

		switch (c_config::get().vis_chams_type) 
		{
		case 0:
			mat = textured;
			break;
		case 1:
			mat = crystal_blue;
			break;
		case 2:
			mat = dogtag;
			break;
		case 3:
			mat = gold;
			break;
		case 4:
			mat = crystal;
			break;
		case 5:
			mat = metallic;
			break;
		}

		if (c_config::get().vis_chams_invis) 
		{

			if (utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)) && c_config::get().vis_chams_smoke_check)
				return;

			interfaces::render_view->modulate_color(c_config::get().clr_chams_invis);
			interfaces::render_view->set_blend(c_config::get().clr_chams_invis[3]);
			mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, true); //crash without increment_reference_count - designer
			interfaces::model_render->override_material(mat);
			if (c_config::get().chams_shiny)
			{
				auto red = c_config::get().clr_chams_shiny[0] * 255;
				auto green = c_config::get().clr_chams_shiny[1] * 255;
				auto blue = c_config::get().clr_chams_shiny[2] * 255;

				bool bFound = false;
				auto pVar = matgloss->find_var("$envmaptint", &bFound);
				if (bFound)
					(*(void(__thiscall **)(int, float, float, float))(*(DWORD *)pVar + 44))((uintptr_t)pVar, red / 255.f, green / 255.f, blue / 255.f); //tfw no IMaterialVar class

				interfaces::model_render->override_material(matgloss);
			}
			entity->draw_model(1, 255);
		}
		if (c_config::get().vis_chams_vis) 
		{
			if (utilities::is_behind_smoke(local_player->get_eye_pos(), entity->get_hitbox_position(entity, hitbox_head)) && c_config::get().vis_chams_smoke_check)
				return;

			interfaces::render_view->modulate_color(c_config::get().clr_chams_vis);
			interfaces::render_view->set_blend(c_config::get().clr_chams_vis[3]);
			mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);
			interfaces::model_render->override_material(mat);
			if (c_config::get().chams_shiny)
			{
				auto red = c_config::get().clr_chams_shiny[0] * 255;
				auto green = c_config::get().clr_chams_shiny[1] * 255;
				auto blue = c_config::get().clr_chams_shiny[2] * 255;

				bool bFound = false;
				auto pVar = matgloss->find_var("$envmaptint", &bFound);
				if (bFound)
					(*(void(__thiscall **)(int, float, float, float))(*(DWORD *)pVar + 44))((uintptr_t)pVar, red / 255.f, green / 255.f, blue / 255.f); //tfw no IMaterialVar class

				interfaces::model_render->override_material(matgloss);
			}
			entity->draw_model(1, 255);

		}



		interfaces::model_render->override_material(nullptr);
	}
}

void c_visuals::epilypsy() noexcept
{
	float flColor[3] = { 0.f };
	flColor[0] = rand() % 255 / 255.f;
	flColor[1] = rand() % 255 / 255.f;
	flColor[2] = rand() % 255 / 255.f;
	interfaces::render_view->modulate_color(flColor);
}

void c_visuals::misc_chams(const model_render_info_t &info) noexcept
{
	if (!c_config::get().visuals_enabled)
		return;
	float flColor[3] = { 0.f };
	const char* model_name = interfaces::model_info->get_model_name((model_t*)info.model);
	static i_material* mat = nullptr;
	auto textured = interfaces::material_system->find_material("aristois_material", TEXTURE_GROUP_MODEL, true, nullptr);
	auto crystal_blue = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL, true, nullptr);
	auto dogtag = interfaces::material_system->find_material("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);
	auto gold = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL);
	auto crystal = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_MODEL);
	auto glass = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_MODEL);
	auto metallic = interfaces::material_system->find_material("aristois_reflective", TEXTURE_GROUP_MODEL, true, nullptr);
	static i_material* matgloss = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER); //you can create your own material to really make it shiny, but i like this subtle one
	

	textured->increment_reference_count();  //we need increment_reference_count cuz without it our materialsystem.dll will crash after  map change - designer
	crystal_blue->increment_reference_count();
	dogtag->increment_reference_count();
	gold->increment_reference_count();
	crystal->increment_reference_count();
	glass->increment_reference_count();
	metallic->increment_reference_count();

	switch (c_config::get().vis_chams_misc_type)
	{
	case 0:
		mat = textured;
		break;
	case 1:
		mat = crystal_blue;
		break;
	case 2:
		mat = dogtag;
		break;
	case 3:
		mat = gold;
		break;
	case 4:
		mat = crystal;
		break;
	case 5:
		mat = glass;
		break;
	case 6:
		mat = metallic;
		break;

	}

	if (c_config::get().vis_chams_misc_wireframe)
	{
		mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true); //crash without increment_reference_count - designer
	}
	if (!c_config::get().vis_chams_misc_wireframe)
	{
		mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, false); //crash without increment_reference_count - designer
	}






	if (c_config::get().weapon_chams && strstr(model_name, "models/weapons/v_"))
	{
		
		interfaces::render_view->set_blend(c_config::get().clr_weapon_chams[3]);
		if (c_config::get().chams_epilepsy)
			epilypsy();
		else
		interfaces::render_view->modulate_color(c_config::get().clr_weapon_chams);
		interfaces::model_render->override_material(mat);
		mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);



	}
	/////////////////

	if (c_config::get().hand_chams && strstr(model_name, "arms"))
	{
		interfaces::model_render->override_material(mat);
		interfaces::render_view->set_blend(c_config::get().clr_hand_chams[3]);
		if (c_config::get().chams_epilepsy)
			epilypsy();
		else
		interfaces::render_view->modulate_color(c_config::get().clr_hand_chams);
		mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);
	}
	/////////////////
	if (c_config::get().sleeve_chams && strstr(model_name, "sleeve"))
	{
		
		interfaces::render_view->set_blend(c_config::get().clr_sleeve_chams[3]);
		if (c_config::get().chams_epilepsy)
			epilypsy();
		else
		interfaces::render_view->modulate_color(c_config::get().clr_sleeve_chams);
		interfaces::model_render->override_material(mat);
		mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, false);
	}



}

float arrowsh = 0.f;
int arrow_alpha[65];
template <typename t> t clamp_value(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}

studio_box_t* get_hitbox(player_t* entity, int hitbox_index)
{
	if (entity->dormant() || entity->health() <= 0)
		return NULL;

	const auto pModel = entity->model();
	if (!pModel)
		return NULL;

	auto pStudioHdr = interfaces::model_info->get_studio_model(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->hitbox_set(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->hitbox_count || hitbox_index < 0)
		return NULL;

	return pSet->hitbox(hitbox_index);
}

vec3_t get_hitbox_position(player_t* entity, int hitbox_id)
{
	auto hitbox = get_hitbox(entity, hitbox_id);
	if (!hitbox)
		return vec3_t(0, 0, 0);

	auto bone_matrix = entity->get_bone_matrix(hitbox->bone);

	vec3_t bbmin, bbmax;
	c_math::get().transform_vector(hitbox->mins, bone_matrix, bbmin);
	c_math::get().transform_vector(hitbox->maxs, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

void c_visuals::draw_arrows() noexcept {

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	const float arrow_fade = 1.11111111111f * interfaces::globals->frame_time;

	arrowsh += (arrowsh_direction ? arrow_fade : -arrow_fade);

	if (arrowsh > 1.f) {
		arrowsh_direction = false;
		arrowsh = 1.f;
	}
	else if (arrowsh <= 0.f) {
		arrowsh_direction = true;
		arrowsh = 0.f;
	}

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!player)
			continue;

		if (player == local_player)
			continue;

		if (player->health() <= 0)
			continue;

		if (player->team() == local_player->team())
			continue;

		// another case of unnecessary float divison. gotta watch out for that guys! ~chance
		//      const int fade = ( int )( ( ( 1 / 0.15f ) * interfaces::globals->frame_time ) * 255 );
		const int fade = (int)((6.66666666667f * interfaces::globals->frame_time) * 255);

		int new_alpha = alpha[i];
		new_alpha += player->dormant() ? -fade : fade;

		if (new_alpha > (player->has_gun_game_immunity() ? 130 : 210))
			new_alpha = (player->has_gun_game_immunity() ? 130 : 210);
		if (new_alpha < c_config::get().vis_dormant ? 50 : 0)
			new_alpha = c_config::get().vis_dormant ? 50 : 0;

		alpha[i] = new_alpha;
		arrows(player);
		player_rendering(player);
		last_dormant[i] = player->dormant();
	}

}

void c_visuals::rotate_arrow(std::array< vec2_t, 3 >& points, float rotation) noexcept
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = deg_to_rad(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}
//credits to c6l for the offscreen esp
void c_visuals::arrows(player_t* entity) noexcept {

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	const int fade = (int)((4 * interfaces::globals->frame_time) * 255);

	int new_alpha = arrow_alpha[entity->index()];
	new_alpha += entity->dormant() ? -fade : fade;
	new_alpha = utilities::math::clamp_value< int >(new_alpha, 0, 200);
	arrow_alpha[entity->index()] = new_alpha;

	if (!c_config::get().vis_arrows || !c_config::get().visuals_enabled)
		return;

	if (!local_player)
		return;

	if (entity->dormant() && new_alpha == 0)
		return;

	vec3_t position;
	if (render::get().on_screen(entity->get_hitbox(2), position))
		return;

	vec3_t viewangles;
	int width, height;

	viewangles = interfaces::engine->get_view_angles();
	interfaces::engine->get_screen_size(width, height);

	const auto screen_center = vec2_t(width * .5f, height * .5f);

	const auto angle_yaw_rad = deg_to_rad(viewangles.y - c_math::get().calculate_angle2(local_player->eye_position(), entity->get_hitbox(2)).y - 90);

	int radius = max(10, c_config::get().vis_arrows_dist);
	int size = max(5, c_config::get().vis_arrows_px);

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	auto red = c_config::get().clr_arrows[0] * 255;
	auto green = c_config::get().clr_arrows[1] * 255;
	auto blue = c_config::get().clr_arrows[2] * 255;

	color clr(red, green, blue, (float)new_alpha * arrowsh);

	std::array< vec2_t, 3 >points{ vec2_t(new_point_x - size, new_point_y - size),
		vec2_t(new_point_x + size, new_point_y),
		vec2_t(new_point_x - size, new_point_y + size) };

	rotate_arrow(points, viewangles.y - c_math::get().calculate_angle2(local_player->eye_position(), entity->get_hitbox(2)).y - 90);
	render::get().draw_filled_triangle(points, clr);
}

void c_visuals::glow() noexcept
{
	if (!c_config::get().visuals_enabled || !c_config::get().visuals_glow)
		return;


	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
		auto &glow = interfaces::glow_manager->objects[i];


		if (glow.unused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.entity);
		auto client_class = glow_entity->client_class();

		if (!glow_entity || glow_entity->dormant())
			continue;

		auto is_enemy = glow_entity->team() != local_player->team();
		auto is_teammate = glow_entity->team() == local_player->team();

		switch (client_class->class_id) 
		{
		case class_ids::ccsplayer:
			if (is_enemy && c_config::get().visuals_glow_enemy) {

				//glow.m_nGlowStyle = 0;
				glow.set(c_config::get().clr_glow[0], c_config::get().clr_glow[1], c_config::get().clr_glow[2], c_config::get().clr_glow[3]);
			}

			else if (is_teammate && c_config::get().visuals_glow_team) {
				glow.set(c_config::get().clr_glow_team[0], c_config::get().clr_glow_team[1], c_config::get().clr_glow_team[2], c_config::get().clr_glow_team[3]);
			}
			break;
		case class_ids::cplantedc4:
			if (c_config::get().visuals_glow_planted) {
				glow.set(c_config::get().clr_glow_planted[0], c_config::get().clr_glow_planted[1], c_config::get().clr_glow_planted[2], c_config::get().clr_glow_planted[3]);
			}
			break;
		}

		if (strstr(client_class->network_name, ("CWeapon")) && c_config::get().visuals_glow_weapons) {
			glow.set(c_config::get().clr_glow_dropped[0], c_config::get().clr_glow_dropped[1], c_config::get().clr_glow_dropped[2], c_config::get().clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cak47 && c_config::get().visuals_glow_weapons) {
			glow.set(c_config::get().clr_glow_dropped[0], c_config::get().clr_glow_dropped[1], c_config::get().clr_glow_dropped[2], c_config::get().clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cc4 && c_config::get().visuals_glow_weapons) {
			glow.set(c_config::get().clr_glow_dropped[0], c_config::get().clr_glow_dropped[1], c_config::get().clr_glow_dropped[2], c_config::get().clr_glow_dropped[3]);
		}

		else if (client_class->class_id == class_ids::cdeagle && c_config::get().visuals_glow_weapons) {
			glow.set(c_config::get().clr_glow_dropped[0], c_config::get().clr_glow_dropped[1], c_config::get().clr_glow_dropped[2], c_config::get().clr_glow_dropped[3]);
		}

	}
}

void c_visuals::skeleton(player_t* entity) noexcept {
	if (!c_config::get().skeleton)
		return;

	auto red = c_config::get().clr_skeleton[0] * 255;
	auto green = c_config::get().clr_skeleton[1] * 255;
	auto blue = c_config::get().clr_skeleton[2] * 255;

	auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());

	if (!p_studio_hdr)
		return;

	vec3_t v_parent, v_child, s_parent, s_child;

	for (int i = 0; i < p_studio_hdr->bones_count; i++) {
		studio_bone_t* bone = p_studio_hdr->bone(i);

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			v_child = entity->get_bone_position(i);
			v_parent = entity->get_bone_position(bone->parent);

			if (c_math::get().world_to_screen(v_parent, s_parent) && c_math::get().world_to_screen(v_child, s_child))
				render::get().draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1], color(red, green, blue, alpha[entity->index()]));
		}
	}
}

void c_visuals::backtrack_skeleton(player_t* entity) noexcept {
	if (!c_config::get().backtrack_skeleton)
		return;

}

void c_visuals::wolrd_color() noexcept
{
	if (!c_config::get().visuals_enabled)
		return;

	if (c_config::get().world_color)
	{
		static auto red = interfaces::console->get_convar("mat_ambient_light_r");
		static auto green = interfaces::console->get_convar("mat_ambient_light_g");
		static auto blue = interfaces::console->get_convar("mat_ambient_light_b");

		red->set_value(c_config::get().clr_world_color[0]);
		green->set_value(c_config::get().clr_world_color[1]);
		blue->set_value(c_config::get().clr_world_color[2]);
	}
}