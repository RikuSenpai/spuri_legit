#pragma once
/*
#include "../../../dependencies/common_includes.hpp"
#include <deque>

struct incoming_sequence_t {
	incoming_sequence_t(int instate, int outstate, int seqnr, float time) {
		m_in_reliable_state = instate;
		m_out_reliable_state = outstate;
		m_sequence_nr = seqnr;
		m_current_time = time;
	}

	int m_in_reliable_state;
	int m_out_reliable_state;
	int m_sequence_nr;
	float m_current_time;
};

class c_pingmanipulation : public singleton< c_pingmanipulation > {
public:


public:
	std::deque< incoming_sequence_t > sequences;
	int m_last_incoming_sequence_number;
	void add_latency(i_net_channel * net_channel);
};*/