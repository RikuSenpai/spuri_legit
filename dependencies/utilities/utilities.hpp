#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

#define M_PI 3.14159265358979323846

using ServerRankRevealAll2 = char(__cdecl*)(int*);

namespace utilities {
	template<typename FuncType>
	__forceinline static FuncType call_virtual(void* ppClass, int index) {
		int* pVTable = *(int**)ppClass;
		int dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}
	namespace math {
		template <typename t> t clamp_value(t value, t min, t max) {
			if (value > max) {
				return max;
			}
			if (value < min) {
				return min;
			}
			return value;
		}
	}
	namespace game {
		void* capture_interface(const char* mod, const char* iface);
	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};

	void change_name(const char* name);

	extern ServerRankRevealAll2 ServerRankRevealAllEx;
	void force_update();
	void force_update_alt();
	void console_warning(const char * msg, ...);
	void NormalizeLemon(vec3_t & angle);
	void ClampAnglesLemon(vec3_t & angles);
	bool ClampLemon(vec3_t & angles);
	void normalise_viewangle(vec3_t & angle);
	const char * hitgroup_name(int hitgroup);
	float csgo_armor(float damage, int armor_value);
	std::uint8_t* pattern_scan(void* module, const char* signature);
	int epoch_time();
	int get_player_comprank();
	void apply_clan_tag(const char * name);
	bool is_behind_smoke(vec3_t start_pos, vec3_t end_pos);
	void material_setup();
	extern void ServerRankRevealAll();

	/////////////////new stuff//////////////
	bool world_to_screen(const vec3_t & origin, vec3_t & screen);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	template<class T>
	static T * find_hud_element(const char * name);
}


