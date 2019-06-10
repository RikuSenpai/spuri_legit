#pragma once
#include "../../../dependencies/utilities/singleton.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class c_movement : public singleton< c_movement > {
public:
	void bunnyhop(c_usercmd* user_cmd) noexcept;
	void auto_strafe(c_usercmd * user_cmd) noexcept;
	void edge_jump_pre_prediction(c_usercmd * user_cmd) noexcept;
	void edge_jump_post_prediction(c_usercmd * user_cmd) noexcept;
	void infinite_duck(c_usercmd * user_cmd) noexcept;
};