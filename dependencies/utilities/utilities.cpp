#include "utilities.hpp"
#include "../common_includes.hpp"
#include <psapi.h>

ServerRankRevealAll2 utilities::ServerRankRevealAllEx;

void utilities::change_name(const char* name_to_change) {
	auto name = interfaces::console->get_convar("name");
	name->callbacks.set_size(0);

	name->set_value(name_to_change);
}


template<class T>
static T* utilities::find_hud_element(const char* name) {
	static auto fn = *reinterpret_cast<DWORD**>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), ("B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08")) + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), ("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")));
	return (T*)find_hud_element(fn, name);
}

void utilities::force_update() {
	static auto fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

	auto element = find_hud_element<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = fn(hud_weapons, i);

	interfaces::clientstate->full_update();
}


void utilities::console_warning(const char* msg, ...) {
	if (msg == nullptr)
		return;
	typedef void(__cdecl* console_warning_fn)(const char* msg, va_list);
	static console_warning_fn fn = (console_warning_fn)GetProcAddress(GetModuleHandle("tier0.dll"), "Warning");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list);
}

void utilities::NormalizeLemon(vec3_t& angle)
{
	while (angle.x > 89.0f) {
		angle.x -= 180.f;
	}
	while (angle.x < -89.0f) {
		angle.x += 180.f;
	}
	while (angle.y > 180.f) {
		angle.y -= 360.f;
	}
	while (angle.y < -180.f) {
		angle.y += 360.f;
	}
}

void utilities::ClampAnglesLemon(vec3_t &angles)
{
	if (angles.y > 180.0f)
		angles.y = 180.0f;
	else if (angles.y < -180.0f)
		angles.y = -180.0f;

	if (angles.x > 89.0f)
		angles.x = 89.0f;
	else if (angles.x < -89.0f)
		angles.x = -89.0f;

	angles.z = 0;
}

bool utilities::ClampLemon(vec3_t &angles)
{
	vec3_t a = angles;
	NormalizeLemon(a);
	ClampAnglesLemon(a);

	if (isnan(a.x) || isinf(a.x) ||
		isnan(a.y) || isinf(a.y) ||
		isnan(a.z) || isinf(a.z)) {
		return false;
	}
	else {
		angles = a;
		return true;
	}
}

extern void utilities::ServerRankRevealAll()
{
		using ServerRankRevealAll2 = bool(__cdecl*)(int*);

		static uint8_t* fnServerRankRevealAll;

		if (!fnServerRankRevealAll)
			fnServerRankRevealAll = utilities::pattern_scan(GetModuleHandleW(L"client_panorama.dll"),
				"55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08");

		int v[3] = { 0,0,0 };
		reinterpret_cast<ServerRankRevealAll2>(fnServerRankRevealAll)(v);
}

void utilities::normalise_viewangle(vec3_t &angle)
{

	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;

}

const char* utilities::hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

float utilities::csgo_armor(float damage, int armor_value) {
	float armor_ratio = 0.5f;
	float armor_bonus = 0.5f;
	if (armor_value > 0) {
		float armor_new = damage * armor_ratio;
		float armor = (damage - armor_new) * armor_bonus;

		if (armor > static_cast<float>(armor_value)) {
			armor = static_cast<float>(armor_value) * (1.f / armor_bonus);
			armor_new = damage - armor;
		}

		damage = armor_new;
	}
	return damage;
}

std::uint8_t* utilities::pattern_scan(void* module, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_headers = (PIMAGE_DOS_HEADER)module;
	auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dos_headers->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scan_bytes[i];
		}
	}
	return nullptr;
}

int utilities::epoch_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void utilities::apply_clan_tag(const char * name) {
	using Fn = int(__fastcall *)(const char *, const char *);
	static auto apply_clan_tag_fn = reinterpret_cast<Fn>(utilities::pattern_scan(GetModuleHandleA("engine.dll"), "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(name, name);
};




bool utilities::is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
	typedef bool(__cdecl* line_goes_through_smoke) (vec3_t, vec3_t);
	static line_goes_through_smoke line_goes_through_smoke_fn = 0;

	if (!line_goes_through_smoke_fn)
		line_goes_through_smoke_fn = (line_goes_through_smoke)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	if (line_goes_through_smoke_fn)
		return line_goes_through_smoke_fn(start_pos, end_pos);
	return false;
}

void* utilities::game::capture_interface(const char* mod, const char* iface) {
	using fn_capture_iface_t = void*(*)(const char*, int*);
	auto fn_addr = (fn_capture_iface_t)GetProcAddress(GetModuleHandleA(mod), "CreateInterface");

	auto iface_addr = fn_addr(iface, nullptr);
	printf("found %s at 0x%p\n", iface, fn_addr(iface, nullptr));

	return iface_addr;
}




void utilities::material_setup() {
	std::ofstream("csgo/materials/aristois_material.vmt") << R"#("VertexLitGeneric" {
            "$basetexture" "vgui/white_additive"
            "$ignorez"      "0"
            "$envmap"       ""
            "$nofog"        "1"
            "$model"        "1"
            "$nocull"       "0"
            "$selfillum"    "1"
            "$halflambert"  "1"
            "$znearer"      "0"
            "$flat"         "0"
			"$phong"		"1"
			"$rimlight"		"1"
        })#";

	std::ofstream("csgo/materials/aristois_reflective.vmt") << R"#("VertexLitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez" "0"
			"$envmap" "env_cubemap"
			"$normalmapalphaenvmapmask" "1"
			"$envmapcontrast"  "1"
			"$nofog" "1"
			"$model" "1"
			"$nocull" "0"
			"$selfillum" "1"
			"$halflambert" "1"
			"$znearer" "0"
			"$flat" "1"
		})#";

	printf("Materials initialized!\n");
}





























































































































































































