#pragma once
#include "../../../dependencies/common_includes.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class c_hooked_events : public i_game_event_listener2 {
public:
	void fire_game_event(i_game_event * event) noexcept;
	void setup() noexcept;
	void release() noexcept;
	int get_event_debug_id(void) noexcept;
};