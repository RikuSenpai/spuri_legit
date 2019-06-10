#include "../../../dependencies/common_includes.hpp"
class cbullet_tracer_info
{
public:
	cbullet_tracer_info(vec3_t src, vec3_t dst, float time, color color)
	{
		this->src = src;
		this->dst = dst;
		this->time = time;
		this->color = color;

	}
	vec3_t src, dst;
	float time;
	color color;
	player_t* pPlayer = nullptr;
};


class c_bullet_beams : public singleton<c_bullet_beams> {
public:
	void bullet_beams(i_game_event* event) noexcept;
	void render_beams() noexcept;
};