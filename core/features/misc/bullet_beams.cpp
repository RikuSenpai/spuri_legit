#include "bullet_beams.hpp"
std::vector<cbullet_tracer_info> logs;

void c_bullet_beams::bullet_beams(i_game_event* event) noexcept
{
	if (!interfaces::engine->is_in_game() && interfaces::engine->is_connected())
		return;
	if (!c_config::get().bullet_beams)
		return;

		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

		if (local_player)
		{
			auto index = interfaces::engine->get_player_for_user_id(event->get_int("userid"));

			if (index != interfaces::engine->get_local_player())
				return;


			auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(index));

			vec3_t position(event->get_float("x"), event->get_float("y"), event->get_float("z"));

			ray_t ray;

			ray.initialize(local->get_eye_pos(), position);

			trace_filter filter;

			filter.skip = local;

			trace_t tr;
			interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

			logs.push_back(cbullet_tracer_info(local->get_eye_pos(), position, interfaces::globals->cur_time, color(255, 255, 255, 255)));

			int red = 255;
			int green = 255;
			int blue = 255;

			if (!local)
				return;

			for (size_t i = 0; i < logs.size(); i++)
			{
				auto current = logs.at(i);

				current.color = color(int(c_config::get().clr_bullet_beams[0] * 255.f),
					int(c_config::get().clr_bullet_beams[1] * 255.f),
					int(c_config::get().clr_bullet_beams[2] * 255.f)); //color of local player's tracers
				BeamInfo_t beam_info;
				beam_info.m_nType = TE_BEAMPOINTS;
				beam_info.m_pszModelName = "sprites/physbeam.vmt";
				beam_info.m_nModelIndex = -1;
				beam_info.m_flHaloScale = 0.0f;
				beam_info.m_flLife = c_config::get().bullet_beams_duration;
				beam_info.m_flWidth = c_config::get().bullet_beams_width;
				beam_info.m_flEndWidth = c_config::get().bullet_beams_width;
				beam_info.m_flFadeLength = 0.0f;
				beam_info.m_flAmplitude = 2.0f;
				beam_info.m_flBrightness = 255.f;
				beam_info.m_flSpeed = 0.2f;
				beam_info.m_nStartFrame = 0;
				beam_info.m_flFrameRate = 0.f;
				beam_info.m_flRed = red;
				beam_info.m_flGreen = green;
				beam_info.m_flBlue = blue;
				beam_info.m_nSegments = 2;
				beam_info.m_bRenderable = true;
				beam_info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

				beam_info.m_vecStart = local_player->get_eye_pos();
				beam_info.m_vecEnd = current.dst;
				if (c_config::get().bullet_beams)
				{

					auto beam = interfaces::render_beams->create_beam_point(beam_info);
					if (beam)
						interfaces::render_beams->draw_beam(beam);
				}

				logs.erase(logs.begin() + i);
			}
		}
	
}

void c_bullet_beams::render_beams() noexcept
{
	if (!c_config::get().bullet_beams)
		return;

	int red = 255;
	int green = 255;
	int blue = 255;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	for (size_t i = 0; i < logs.size(); i++)
	{
		auto current = logs.at(i);
		current.color = color(int(c_config::get().clr_bullet_beams[0] * 255.f),
			int(c_config::get().clr_bullet_beams[1] * 255.f),
			int(c_config::get().clr_bullet_beams[2] * 255.f));

		if (c_config::get().bullet_beams)
			interfaces::debug_overlay->addlineoverlay(current.src, current.dst, red, green, blue, true, -1.f);

		interfaces::debug_overlay->addboxoverlay(current.dst, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), vec3_t(0, 0, 0), red, green, blue, 127, -1.f);

		if (fabs(interfaces::globals->cur_time - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}