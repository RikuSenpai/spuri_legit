/*
#include "fakeping.hpp"

void c_pingmanipulation::add_latency(i_net_channel * net_channel) {
	for (auto& seq : sequences) {
		if (interfaces::globals->realtime - seq.m_current_time >= c_config::get().fake_ping_value / 1000.f) {
			net_channel->in_reliable_state_count = seq.m_in_reliable_state;
			net_channel->in_sequence_nr = seq.m_sequence_nr;

			break;
		}
	}
}*/