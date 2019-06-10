#pragma once
#include "../../../dependencies/common_includes.hpp"
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

extern float local_update;
class c_antiaim
{
public:
	void fake_duck(c_usercmd* user_cmd) noexcept;
	void do_antiaim(c_usercmd* user_cmd) noexcept;
	void fix_movement(c_usercmd * user_cmd, vec3_t wish_angle) const noexcept;
	vec3_t fix_movement(c_usercmd* user_cmd, c_usercmd original) noexcept;
private:

};

extern c_antiaim* antiaim;

float get_max_desync_delta(SDK::CBaseEntity * local);

