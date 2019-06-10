#include "../common_includes.hpp"

float c_math::distance_based_fov(float distance, vec3_t angle, c_usercmd* cmd) {
	vec3_t aiming_at;
	c_math::get().angle_vectors(cmd->viewangles, aiming_at);
	aiming_at *= distance / 10;

	vec3_t aim_at;
	c_math::get().angle_vectors(angle, aim_at);
	aim_at *= distance / 10;

	return aiming_at.distance_to(aim_at);
}

vec3_t c_math::calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) {
	vec3_t delta = source - destination;
	auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
	vec3_t angles;
	angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
	angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - viewAngles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	angles.normalize_aimbot();
	return angles;
}



void c_math::correct_movement(vec3_t vOldAngles, c_usercmd* user_cmd, float fOldForward, float fOldSidemove)
{
	// side/forward move correction
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (user_cmd->viewangles.y < 0.0f)
		f2 = 360.0f + user_cmd->viewangles.y;
	else
		f2 = user_cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	user_cmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	user_cmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

void inline sincos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);

}

void c_math::angle_vectors_alternative(const vec3_t &angles, vec3_t *forward)
{
	float	sp, sy, cp, cy;

	sincos(DEG2RAD(angles[1]), &sy, &cy);
	sincos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}


void c_math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

float c_math::normalize_angle(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));
		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}
	return angle;
}

void c_math::angle_to_vectors(const vec3_t &angles, vec3_t *forward, vec3_t *right, vec3_t *up) {
	float sp, sy, sr, cp, cy, cr;

	sincos(deg_to_rad(angles.x), &sp, &cp);
	sincos(deg_to_rad(angles.y), &sy, &cy);
	sincos(deg_to_rad(angles.z), &sr, &cr);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;

	right->x = -1 * sr * sp * cy + -1 * cr * -sy;
	right->y = -1 * sr * sp * sy + -1 * cr * cy;
	right->z = -1 * sr * cp;

	up->x = cr * sp * cy + -sr * -sy;
	up->y = cr * sp * sy + -sr * cy;
	up->z = cr * cp;
}

bool c_math::clamp_angles_alt(vec3_t &angles) {
	if (std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)) {
		clamp(angles.x, -89.f, 89.f);
		clamp(angles.y, -180.f, 180.f);
		angles.z = 0.f;
		return true;
	}

	return false;
}

bool c_math::normalize_angles(vec3_t &angles) {
	if (std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)) {
		angles.x = std::remainder(angles.x, 360.f);
		angles.y = std::remainder(angles.y, 360.f);
		return true;
	}

	return false;
}

vec3_t c_math::normalize_angle_alternative(vec3_t angle)
{
	while (angle.x > 89.f)
	{
		angle.x -= 180.f;
	}
	while (angle.x < -89.f)
	{
		angle.x += 180.f;
	}
	if (angle.y > 180)
	{
		angle.y -= (round(angle.y / 360) * 360.f);
	}
	else if (angle.y < -180)
	{
		angle.y += (round(angle.y / 360) * -360.f);
	}
	if ((angle.z > 50) || (angle.z < 50))
	{
		angle.z = 0;
	}
	return angle;
}

vec3_t c_math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f* static_cast<float>(M_PI));

	return vec3_t(cp*cy, cp*sy, -sp);
}




float c_math::point_to_line(vec3_t point, vec3_t line_origin, vec3_t dir) {
	auto point_dir = point - line_origin;

	auto temp_offset = point_dir.dot(dir) / (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

	if (temp_offset < 0.000001f)
		return FLT_MAX;

	auto perpendicular_point = line_origin + (dir * temp_offset);

	return (point - perpendicular_point).length();
}

void c_math::transform_vector(vec3_t& a, matrix_t& b, vec3_t& out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

inline vec3_t extrapolate_tick(vec3_t p0, vec3_t v0)
{
	return p0 + (v0 * interfaces::globals->interval_per_tick);
}


void c_math::vector_angles(vec3_t& forward, vec3_t& angles) {
	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles.y = 0.0f; //yaw left/right
	}
	else {
		angles.x = atan2(-forward.z, vec2_t(forward).length()) * -180 / static_cast<float>(pi);
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(pi);

		if (angles.y > 90)
			angles.y -= 180;
		else if (angles.y < 90)
			angles.y += 180;
		else if (angles.y == 90)
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void c_math::angle_vectors(vec3_t& angles, vec3_t& forward) {
	float sp, sy, cp, cy;

	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.x), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void c_math::vector_angles2(const vec3_t& forward, vec3_t& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f; //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90)
			angles[1] -= 180;
		else if (angles[1] < 90)
			angles[1] += 180;
		else if (angles[1] == 90)
			angles[1] = 0;
	}

	angles[2] = 0.0f;
}

vec3_t c_math::calculate_angle3(vec3_t src, vec3_t dst)
{
	vec3_t angles;
	vec3_t delta = src - dst;
	c_math::get().vector_angles2(delta, angles);
	delta.normalize();
	return angles;
}

void c_math::angle_vectors2(vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(DEG2RAD(angles[1]), &sy, &cy);
	sin_cos(DEG2RAD(angles[0]), &sp, &cp);
	sin_cos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}
	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}
	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}



void c_math::angle_vectors(vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(deg_to_rad(angles.x), &sp, &cp);
	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void c_math::angle_to_vector_alt(const vec3_t & angles, vec3_t & forward) {
	float sp, sy, cp, cy;

	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.x), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}



void c_math::angle_to_vectors_alt(const vec3_t& angles, vec3_t* forward) {
	float sp, sy, cp, cy;

	sin_cos(deg_to_rad(angles.x), &sp, &cp);
	sin_cos(deg_to_rad(angles.y), &sy, &cy);


	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;


}
void c_math::angle_to_vectors_alt(const vec3_t & angles, vec3_t * forward, vec3_t * right, vec3_t * up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(deg_to_rad(angles.x), &sp, &cp);
	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.z), &sr, &cr);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;

	right->x = -1 * sr * sp * cy + -1 * cr * -sy;
	right->y = -1 * sr * sp * sy + -1 * cr * cy;
	right->z = -1 * sr * cp;

	up->x = cr * sp * cy + -sr * -sy;
	up->y = cr * sp * sy + -sr * cy;
	up->z = cr * cp;
}

vec3_t c_math::calculate_angle_alternative(vec3_t& a, vec3_t& b) {
	vec3_t angles;

	vec3_t delta = a - b;

	vector_angles(delta, angles);

	return angles;
}

vec3_t c_math::calculate_angle2(const vec3_t& vecSource, const vec3_t& vecDestination)
{
	vec3_t qAngles;
	vec3_t delta = vec3_t((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	double hyp = std::sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(std::atan(delta[2] / hyp) * (180.0 / M_PI));
	qAngles[1] = (float)(std::atan(delta[1] / delta[0]) * (180.0 / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}

vec3_t c_math::vector_add(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t c_math::vector_subtract(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t c_math::vector_multiply(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

void c_math::normalize_vector(vec3_t& vec)
{
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}



vec3_t c_math::vector_divide(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool c_math::screen_transform(const vec3_t& point, vec3_t& screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool c_math::world_to_screen(const vec3_t &origin, vec3_t &screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	auto find_point = [](vec3_t &point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};

	float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * origin.x
		+ matrix[0][1] * origin.y
		+ matrix[0][2] * origin.z
		+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * origin.x
		+ matrix[1][1] * origin.y
		+ matrix[1][2] * origin.z
		+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}

float c_math::distance_to_ray(vec3_t &pos, vec3_t &ray_start, vec3_t &ray_end, float *along, vec3_t *point_on_ray) {
	vec3_t to = pos - ray_start;
	vec3_t dir = ray_end - ray_start;
	float length = dir.normalize_float();

	float range_along = dir.dot(to);
	if (along)
		*along = range_along;

	float range;

	if (range_along < 0.0f)
	{
		range = -(pos - ray_start).length();

		if (point_on_ray)
			*point_on_ray = ray_start;
	}
	else if (range_along > length)
	{
		range = -(pos - ray_end).length();

		if (point_on_ray)
			*point_on_ray = ray_end;
	}
	else
	{
		vec3_t on_ray = ray_start + range_along * dir;
		range = (pos - on_ray).length();

		if (point_on_ray)
			*point_on_ray = on_ray;
	}

	return range;
}





























































































































































































