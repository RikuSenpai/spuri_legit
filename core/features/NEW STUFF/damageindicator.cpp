#include "damageindicator.hpp"


damage_indicator damage_indicators;

void damage_indicator::paint() noexcept
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	

	if (interfaces::engine->is_connected() && interfaces::engine->is_in_game() && local_player->is_alive())
	{
		float current_time = local_player->get_tick_base() * interfaces::globals->interval_per_tick;

		for (int i = 0; i < data.size(); i++)
		{
			if (data[i].flEraseTime < current_time) {
				data.erase(data.begin() + i);
				continue;
			}

			if (!data[i].bInitialized) {
				data[i].Position = data[i].Player->get_head_pos();
				data[i].bInitialized = true;
			}

			if (current_time - data[i].flLastUpdate > 0.0001f) {
				data[i].Position.z -= (0.1f * (current_time - data[i].flEraseTime));
				data[i].flLastUpdate = current_time;
			}

			vec3_t screen_pos;
			auto red = c_config::get().clr_damage_indicator[0] * 255;
			auto green = c_config::get().clr_damage_indicator[1] * 255;
			auto blue = c_config::get().clr_damage_indicator[2] * 255;
			auto alpha = c_config::get().clr_damage_indicator[3] * 255;
			if (c_math::get().world_to_screen(data[i].Position, screen_pos))
			{
				//render::get().draw_text(screen_pos.x, screen_pos.y, std::to_string(data[i].iDamage).c_str(), render::get().name_font, Color(26, 130, 228, 255));
				render::get().draw_text(screen_pos.x, screen_pos.y, render::get().damage_font, std::to_string(data[i].iDamage).c_str(), false, color(red, green, blue, alpha));
			}
		}
	}
	else
	{
		return;
	}

}