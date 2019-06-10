#pragma once
#include "../../dependencies/math/math.hpp"
#include <array>
#include "collideable.hpp"
#include "client_class.hpp"
#include "../../dependencies/utilities/netvar_manager.hpp"
#include "../../core/features/NEW STUFF/anim_layers.hpp"

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};
enum item_definition_indexes {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
};

class c_flashlight_effect {
public:
	bool m_is_on; //0x0000 
	char pad_0x0001[0x3]; //0x0001
	int m_idx; //0x0004 
	WORD m_handle; //0x0008 
	char pad_0x000A[0x2]; //0x000A
	float m_muzzle_flash_brightness; //0x000C 
	float m_fov; //0x0010 
	float m_far_z; //0x0014 
	float m_linear_atten; //0x0018 
	bool m_casts_shadow; //0x001C 
	char pad_0x001D[0x3]; //0x001D
	float m_current_pullback_dist; //0x0020 
	DWORD m_muzzle_flash_texture; //0x0024 
	DWORD m_texture; //0x0028 
	char m_texture_name[64]; //0x1559888 
}; //Size=0x006C


class entity_t {
public:
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}

	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[155](this);
	}
	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[163](this);
	}
	vec3_t get_absolute_origin() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}


	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this) {
			return false;
		}

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}
	vec3_t &get_world_space_center() {
		vec3_t vec_origin = origin();

		vec3_t min = this->collideable()->mins() + vec_origin;
		vec3_t max = this->collideable()->maxs() + vec_origin;

		vec3_t size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}

	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	OFFSET(bool, dormant, 0xED);
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long);
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float);
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t);
	NETVAR("DT_CSPlayer", "m_iTeamNum", team, int);
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool);



};

class ragdoll_t : public entity_t {
public:
	NETVAR("CCSRagdoll", "m_vecForce", vec_force, vec3_t);
	NETVAR("CCSRagdoll", "m_vecRagdollVelocity", vec_ragdoll_vel, vec3_t);
};



class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool);
	NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int);
	NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int);
	NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int);

};

class base_view_model : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int);
	NETVAR("DT_BaseViewModel", "m_hWeapon", m_hweapon, int);
	NETVAR("DT_BaseViewModel", "m_hOwner", m_howner, int);
};

class attributable_item_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", original_owner_xuid_low, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", original_owner_xuid_high, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fallback_stattrak, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fallback_paint_kit, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fallback_seed, int);
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, int);
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fallback_wear, float);
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", world_model_handle, unsigned long);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short);
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int); //jakby crash to wez offset z hazedumpera

	econ_view_item_t& item() {
		return *(econ_view_item_t*)this;
	}
};

class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float);
	NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int);
	NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int);
	NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int);
	NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float);
	NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short);
	NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int);

	float get_innacuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[476](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[446](this);
	}


	bool is_pistol(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_GLOCK, WEAPON_ELITE, WEAPON_P250, WEAPON_TEC9, WEAPON_CZ75A, WEAPON_DEAGLE, WEAPON_REVOLVER, WEAPON_USP_SILENCER, WEAPON_HKP2000, WEAPON_FIVESEVEN };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_sniper(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_AWP, WEAPON_SSG08, WEAPON_G3SG1, WEAPON_SCAR20 };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_smg(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_MAC10, WEAPON_MP7, WEAPON_UMP45, WEAPON_P90, WEAPON_BIZON, WEAPON_MP9, WEAPON_MP5SD };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_heavy(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_NOVA, WEAPON_XM1014, WEAPON_SAWEDOFF, WEAPON_M249, WEAPON_NEGEV, WEAPON_MAG7 };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_knife(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_KNIFE, WEAPON_KNIFEGG, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GHOST, WEAPON_KNIFE_GUT, WEAPON_KNIFE_GYPSY_JACKKNIFE, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_STILETTO, WEAPON_KNIFE_T, WEAPON_KNIFE_TACTICAL, WEAPON_KNIFE_URSUS, WEAPON_KNIFE_WIDOWMAKER, WEAPON_BAYONET };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_grenade(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_FRAG_GRENADE, WEAPON_HEGRENADE, WEAPON_INCGRENADE, WEAPON_SMOKEGRENADE, WEAPON_TAGRENADE, WEAPON_MOLOTOV, WEAPON_FLASHBANG, WEAPON_DECOY };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_bomb(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_C4 };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	bool is_rifle(void* weapon) {
		if (weapon == nullptr)
			return false;

		auto current_weapon = (weapon_t*)weapon;
		int id = current_weapon->item_definition_index();
		static const std::vector<int> v = { WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,WEAPON_M249,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_NEGEV,WEAPON_SG556 };
		return (std::find(v.begin(), v.end(), id) != v.end());
	}

	short* fixskins()
	{
		return (short*)((uintptr_t)this + 0x2FAA);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[477](this);
	}
	weapon_info_t* get_weapon_data() 
	{
		using original_fn = weapon_info_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[454](this); //skinchanger crash
	}
};

class CBaseAnimState
{
public:
	char pad[4];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	vec_t m_vOrigin; //0xB0, 0xB4, 0xB8
	vec_t m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	int8_t m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
	float* feetyaw() //rofl
	{
		return reinterpret_cast<float*>((DWORD)this + 0x0080);
	}
};

class AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Siz


class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int); //tutaj
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool);
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool);
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int);
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t); //"DT_CSPlayer->m_angEyeAngles[0]");
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int);
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool);
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool);
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool);
	NETVAR("DT_CSPlayer", "m_iAccount", money, int);
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float);
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float);
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float);
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float);
	NETVAR("DT_CSPlayer", "m_bHasNightVision", m_bHasNightVision, float);
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", m_bNightVisionOn, float);
	NETVAR("DT_CSPlayer", "m_iHealth", health, int);
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int);
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int);
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t);
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t);
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float);
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", m_flFOVTime, float);
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long);
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int);
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float);
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool);
	NETVAR("DT_PlantedC4", "m_flC4Blow", c4_blow_time, float);
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int);
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int);
	NETVAR("DT_CSPlayerResource", "m_iCompetitiveRanking", get_comprank, int);
	NETVAR("DT_CSPlayer", "m_hObserverTarget", observer_handle, unsigned long);
	NETVAR("DT_CSPlayer", "m_iObserverMode", observer_mode, int long);
	weapon_t* active_weapon() {
		auto active_weapon = read<DWORD>(sdk::util::getNetVar(sdk::util::fnv::hash("DT_CSPlayer"), sdk::util::fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}

	weapon_t* active_weapon_enemy() {
		auto active_weapon_enemy = read<DWORD>(sdk::util::getNetVar(sdk::util::fnv::hash("DT_BaseCombatCharacter"), sdk::util::fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon_enemy));
	}

	bool has_c4() {
		static auto ret = reinterpret_cast<bool(__thiscall*)(void*)>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "56 8B F1 85 F6 74 31"));
		return ret(this);
	}

	vec3_t get_predicted(vec3_t p0)
	{
		return extrapolate_tick(p0, this->velocity());
	}

	void set_local_viewangles(vec3_t &angle) {
		using fnSetLocalViewAngles = void(__thiscall *)(void *, vec3_t &);
		(*(fnSetLocalViewAngles**)this)[369](this, angle);
	}


	vec3_t get_eye_pos() {
		return origin() + view_offset(); //hazedumper
	}

	vec3_t eye_position() {
		return origin() + view_offset();
	}

	vec3_t get_head_pos()
	{
		return this->get_bone_position(6);
	}

	vec3_t wtf_am_i_doing()
	{
		return this->get_bone_position(1);
		return this->get_bone_position(2);
		return this->get_bone_position(3);
		return this->get_bone_position(4);
		return this->get_bone_position(5);
		return this->get_bone_position(6);
		return this->get_bone_position(7);
		return this->get_bone_position(8);
		return this->get_bone_position(9);
		return this->get_bone_position(10);
		return this->get_bone_position(11);
	}

	matrix_t get_bone_matrix(int BoneID)
	{
		matrix_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

	bool is_valid_player()
	{
		if (!this)
			return false;
		if (!is_alive())
			return false;
		//	if (!(GetHealth() > 0))
		//		return false;
		player_info_t info;
		return true;
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.initialize(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(player_t* entity, int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (entity->setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(entity->model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					c_math::get().transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					c_math::get().transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	vec3_t get_hitbox(int hitbox) {
		matrix_t matrix[128];

		if (this->setup_bones(matrix, 128, 256, interfaces::globals->cur_time)) {
			studio_hdr_t* hdr = interfaces::model_info->get_studio_model(this->model());
			studio_hitbox_set_t* set = hdr->hitbox_set(0);
			studio_box_t* box = set->hitbox(hitbox);

			if (box) {
				vec3_t min, max, vec_center, screen_center;
				c_math::get().transform_vector(box->mins, matrix[box->bone], min);
				c_math::get().transform_vector(box->maxs, matrix[box->bone], max);
				vec_center = (min + max) * 0.5;

				return vec_center;
			}
		}

		return vec3_t(0, 0, 0);
	}

	bool is_enemy() 
	{
		static auto danger_zone = interfaces::console->get_convar("game_type");

		if (!is_in_local_team() || danger_zone->get_int() == 6)
			return true;
		else
			return false;
	}

	bool is_in_local_team() {
		return utilities::call_virtual<bool(__thiscall*)(void*)>(this, 92)(this);
	}

	bool is_alive() {
		return life_state() == 0;
	}

	bool is_moving() {
		if (this->velocity().length() > 0.1f)
			return true;

		return false;
	}



	bool is_in_air() {
		if (this->flags() & fl_onground)
			return false;

		return true;
	}

	bool is_flashed() {
		if (this->flash_duration() > 0.0f)
			return true;

		return false;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[221](this);
	}

	vec3_t & abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	vec3_t & abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}
	int	move_type() {
		return *reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(this) + 0x25C); //hazedumper
	}
	vec3_t eye_pos() {
		vec3_t ret;
		utilities::call_virtual<void(__thiscall*)(void*, vec3_t&)>(this, 281)(this, ret); // this is the real eye pos
		return ret;
	}
	vec3_t get_ragdoll_pos()
	{
		return *reinterpret_cast<vec3_t*>(uintptr_t(this) + 0x17C0);
	}
	bool& client_side_animation()
	{
		return *reinterpret_cast<bool*>(uintptr_t(this) + 0x1600);
	}

	CBaseAnimState *get_animation_state() {
		return *reinterpret_cast<CBaseAnimState **>(reinterpret_cast<void *>(uintptr_t(this) + 0x3884));
	}
	CBaseAnimState *AnimState()
	{
		return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + 0x3900);
	}

	void setAnimationState(CBaseAnimState * state) {
		*reinterpret_cast<CBaseAnimState  **>(uintptr_t(this) + 0x3884) = state;
	}
	void SetNumAnimOverlays(AnimationLayer tranny)
	{
		*reinterpret_cast<AnimationLayer*>(uintptr_t(this) + 0x297C) = tranny;
	}

	CBaseAnimState* GetBasePlayerAnimState()
	{
		return *(CBaseAnimState**)((DWORD)this + 0x3900);
	}

	int* weapons() { //tu jesli skinchanger
		return reinterpret_cast<int*> (uintptr_t(this) + 0x2DF8);
	} 
};

