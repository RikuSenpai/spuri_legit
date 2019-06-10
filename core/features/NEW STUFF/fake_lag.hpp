#pragma once

#include "../../../dependencies/common_includes.hpp"


#pragma once
#pragma once

namespace SDK
{
	class CUserCmd;
}

class CFakeLag
{
public:
	int lag_comp_break();
	void do_fakelag(c_usercmd* user_cmd);
};

extern CFakeLag* fakelag;