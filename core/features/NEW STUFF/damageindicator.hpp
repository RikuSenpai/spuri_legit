#pragma once

#include "../../../dependencies/common_includes.hpp"


struct damage_indicator_t {
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	player_t * Player;
	vec3_t Position;
};

class damage_indicator {
public:
	std::vector<damage_indicator_t> data;
	void paint() noexcept;
};

extern damage_indicator damage_indicators;