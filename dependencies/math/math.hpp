#pragma once
#include <cmath>
#include "../../source-sdk/math/vector3d.hpp"
#include "../../dependencies/utilities/singleton.hpp"
#include <array>
constexpr double pi = 3.14159265358979323846;

#ifndef deg_to_rad
#define deg_to_rad(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

#define rad_to_deg( x )  ( (float)(x) * (float)( 180.f / pi ) )

class c_math : public singleton< c_math > {
public:

	template< typename t = float >
	void clamp(t &val, const t &min, const t &max) {
		// check type.
		static_assert(std::is_arithmetic< t >::value, "math::clamp only supports integral types.");
		val = (t)_mm_cvtss_f32(
			_mm_min_ss(_mm_max_ss(_mm_set_ss((float)val), _mm_set_ss((float)min)),
				_mm_set_ss((float)max))
		);
	}

	float distance_based_fov(float distance, vec3_t angle, c_usercmd * cmd);
	vec3_t calculate_angle(const vec3_t & source, const vec3_t & destination, const vec3_t & viewAngles);
	void angle_to_vectors(const vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up);
	bool clamp_angles_alt_2(vec3_t & angles);
	void correct_movement(vec3_t vOldAngles, c_usercmd* user_cmd, float fOldForward, float fOldSidemove);
	void angle_vectors_alternative(const vec3_t & angles, vec3_t * forward);
	void sin_cos(float r, float* s, float* c);
	float normalize_angle(float angle);
	vec3_t normalize_angle_alternative(vec3_t angle);
	vec3_t angle_vector(vec3_t angle);
	float point_to_line(vec3_t point, vec3_t line_origin, vec3_t dir);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t&);
	void vector_angles2(const vec3_t & forward, vec3_t & angles);
	vec3_t calculate_angle3(vec3_t src, vec3_t dst);
	void angle_vectors2(vec3_t& angles, vec3_t* forward = nullptr, vec3_t* right = nullptr, vec3_t* up = nullptr);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	void angle_to_vector_alt(const vec3_t & angles, vec3_t & forward);
	void angle_to_vectors_alt(const vec3_t & angles, vec3_t * forward);
	void angle_to_vectors_alt(const vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up);
	vec3_t calculate_angle_alternative(vec3_t & a, vec3_t & b);
	vec3_t calculate_angle2(const vec3_t & vecSource, const vec3_t & vecDestination);
	float degrees_to_radians(const float deg);
	float radians_to_degrees(const float rad);
	void rotate_triangle(std::array<vec2_t, 3>& points, float rotation);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	void normalize_vector(vec3_t & vec);
	bool clamp_angles_alt(vec3_t & angles);
	bool normalize_angles(vec3_t & angles);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t &origin, vec3_t &screen);
	float distance_to_ray(vec3_t &pos, vec3_t &ray_start, vec3_t &ray_end, float *along = nullptr, vec3_t *point_on_ray = nullptr);

};
extern vec3_t extrapolate_tick(vec3_t p0, vec3_t v0);