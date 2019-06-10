#include "third_person.hpp"
/*

c_thirdperson g_thirdperson;
void c_thirdperson::set_up() {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;

	if (c_config::get().third_person_key != 0)
	{
		static bool is_clicked = false;
		static bool is_down = false;

		bool key_pressed = GetAsyncKeyState(c_config::get().third_person_key);
		is_clicked = !key_pressed && is_down;
		is_down = key_pressed;

		if (is_clicked)
			c_config::get().third_person = !c_config::get().third_person;
	}


	static vec3_t vecAngles;
	interfaces::engine->get_view_angles_alternative(vecAngles);

	if (c_config::get().third_person && local_player->is_alive()) {
		if (!interfaces::input->m_fCameraInThirdPerson) {
			auto get_correct_distance = [&](float ideal_distance) -> float {
				vec3_t inverseAngles;
				interfaces::engine->get_view_angles_alternative(inverseAngles);

				inverseAngles.x *= -1.f, inverseAngles.y += 180.f;

				vec3_t direction;
				c_math::get().angle_to_vector_alt(inverseAngles, direction);

				ray_t ray;
				trace_t trace;
				trace_filter_skip_one_entity filter(local_player);

				ray.initialize(local_player->eye_pos(), local_player->eye_pos() + (direction * ideal_distance));

				interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &trace);

				return (ideal_distance * trace.flFraction) - 10.f;
			};

			vecAngles.z = get_correct_distance(60.f);

			interfaces::input->m_fCameraInThirdPerson = true;

			interfaces::input->m_vecCameraOffset = vec3_t(vecAngles.x, vecAngles.y, vecAngles.z);
		}
	}
	else {
		interfaces::input->m_fCameraInThirdPerson = false;

		interfaces::input->m_vecCameraOffset = vec3_t(vecAngles.x, vecAngles.y, 0);
	}

}
void c_thirdperson::set_up(c_viewsetup * setup) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	static bool once = false;
	static bool once2 = false;

	if (c_config::get().thirdperson_spectators) {
		once = true;

		if (local_player->is_alive()) {
			once2 = false;
			return;
		}

		auto spechandle = local_player->observer_handle();

		auto spec = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(spechandle));
		if (!spec)
			return;

		static bool once2 = false;

		if (once2)
			local_player->observer_mode() = 5;

		if (local_player->observer_mode() == 4)
			once2 = true;

		static vec3_t angles;
		interfaces::engine->get_view_angles_alternative(angles);
		setup->angles = angles;

		trace_t tr;
		ray_t ray;

		vec3_t forward, right, up;
		c_math::get().angle_to_vectors_alt(angles, &forward, &right, &up);

		vec3_t cam_offset = spec->eye_pos() + (forward * -120.f) + (right)+(up);

		ray.initialize(spec->eye_pos(), cam_offset);
		trace_world_only trace_filter;
		trace_filter.pSkip1 = spec;
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &trace_filter, &tr);

		setup->origin = tr.end;
	}
	else if (once) {
		local_player->observer_mode() = local_player->is_alive() ? 0 : 4;
		once = false;
	}
}
void c_thirdperson::OverrideView(c_viewsetup* setup) {
	set_up(setup);
	if (interfaces::engine->is_in_game()) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		if (local_player && local_player->is_alive()) {
			//pSetup->m_fov = config.visuals.effects.camera_fov;
			setup->fov = 120.f;
		}
	}
	set_up();
}
void c_thirdperson::FrameStageNotify(int frame_stage) {
	switch (frame_stage) {
	case FRAME_RENDER_START: {
		const auto in_thirdperson = interfaces::input->m_fCameraInThirdPerson;
		if (in_thirdperson) {
			interfaces::prediction->set_local_viewangles(globals::real_angles);
			//g_csgo.m_prediction->set_local_viewangles(g_client.m_last_angle);
		}
	}
	}
}*/