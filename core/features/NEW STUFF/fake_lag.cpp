#include "fake_lag.hpp"

int CFakeLag::lag_comp_break()
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player || local_player->health() <= 0)
		return 1;

	auto velocity = local_player->velocity();
	velocity.z = 0.f;
	auto speed = velocity.length();
	auto distance_per_tick = speed * interfaces::globals->interval_per_tick;
	int choked_ticks = std::ceilf(64.f / distance_per_tick);
	return std::min<int>(choked_ticks, 14.f);
}

void CFakeLag::do_fakelag(c_usercmd* user_cmd)
{
	g::send_packet = true;
	int choke_amount;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player || local_player->health() <= 0)
		return;

	auto net_channel = interfaces::engine->get_net_channel();
	if (!net_channel)
		return;


	if (c_config::get().fake_lag_enable)
	{
		choke_amount = 1;
		if (!(local_player->flags() & fl_onground))
		{
			switch (c_config::get().fake_lag_type % 2)
			{
			case 0: choke_amount = c_config::get().fake_lag_amount; break;
			case 1: choke_amount = lag_comp_break(); break;
			}
		}
		else if (local_player->velocity().Length2D() > 0.1)
		{
			switch (c_config::get().fake_lag_type % 2)
			{
			case 0: choke_amount = c_config::get().fake_lag_amount; break;
			case 1: choke_amount = lag_comp_break(); break;
			}
		}
		else if (local_player->velocity().Length2D() < 0.1)
		{
			switch (c_config::get().fake_lag_type % 2)
			{
			case 0: choke_amount = c_config::get().fake_lag_amount; break;
			case 1: choke_amount = lag_comp_break(); break;
			}
		}
	}
	else
		choke_amount = 1;

	if (net_channel->m_nChokedPackets >= min(14, choke_amount))
		g::send_packet = true;
	else
		g::send_packet = false;
}

CFakeLag* fakelag = new CFakeLag();