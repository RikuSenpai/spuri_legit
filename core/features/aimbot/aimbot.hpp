#pragma once
#include "../../../dependencies/common_includes.hpp"
#include "../../../dependencies/utilities/singleton.hpp"

extern bool aimbotting;

class c_aimbot : public singleton< c_aimbot > {
private:
	int find_target(c_usercmd * user_cmd) noexcept;
	bool AimAtPoint(vec3_t point, c_usercmd * pCmd, bool & bSendPacket);
	int get_nearest_bone(player_t * entity, c_usercmd* user_cmd) noexcept;

	bool canshoot();


	float aim_smooth;
	float aim_fov;
	float rcs_x;
	float rcs_y;
	int hitbox_id;
	vec3_t angle;
	float speed;
	int delay = 0;
	bool perfect_silent;
	

public:
	void norecoil(c_usercmd * user_cmd) noexcept;
	void do_pitch(c_usercmd * user_cmd, bool & bSendPacket);
	void run(c_usercmd*) noexcept;
	void weapon_settings(weapon_t * weapon) noexcept;
	void smart_switch(c_usercmd* user_cmd) noexcept;
	void triggerbot(c_usercmd * user_cmd, weapon_t * weapon);
	void rcs_standalone() noexcept;
};