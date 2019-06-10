#pragma once
#include "../../../dependencies/common_includes.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <deque>
using namespace std;

class c_misc : public singleton <c_misc> {
public:
	
	static void normalizeinout(vec3_t &vIn, vec3_t &vOut)
	{
		float flLen = vIn.length();
		if (flLen == 0) {
			vOut.init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}
	bool paste = false;
	void remove_smoke() noexcept;
	void remove_flash() noexcept;
	void left_knife() noexcept;
	void smart_switch(player_t * entity) noexcept;
	void remove_scope() noexcept;
	void spectators() noexcept;
	void watermark() noexcept;
	void clantag_spammer() noexcept;
	void viewmodel_offset() noexcept;
	void disable_post_processing() noexcept;
	void grenade_trajectory() noexcept;
	void recoil_crosshair() noexcept;
	void force_crosshair() noexcept;
	void aspect_ratio() noexcept;
	void rank_reveal() noexcept;
	void keystrokes() noexcept;
	void fake_walk(c_usercmd * user_cmd);
	void velocity_draw() noexcept;
	void third_person() noexcept;
	void free_cam(c_viewsetup * setup) noexcept;
	void chat_spam() noexcept;
	void name_spam() noexcept;
	void walk_bot(vec3_t & OrigAng, c_usercmd * user_cmd) noexcept;
	void ragdoll() noexcept;
	void force_scope_overlay() noexcept;
private:
	
};