#pragma once

#include "../../../dependencies/common_includes.hpp"
#include "../../../dependencies/utilities/singleton.hpp"

class c_ragebot : public singleton< c_ragebot > {
private:



public:

	void rcs_standalone() noexcept;
	void run(c_usercmd * user_cmd) noexcept;

};