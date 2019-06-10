#pragma once
#include "../../source-sdk/structs/vertex_t.hpp"
#include "../../source-sdk/misc/color.hpp"

typedef  vertex_t fontvertex_t;

enum gradient_type_t {
	horizontal,
	vertical
};
class i_surface {
public:
	/*
	void set_drawing_color_alt(color col)
	{
		using original_fn = void(__thiscall*)(i_surface*, color);
		return (*(original_fn**)this)[14](this, col);
	}
	*/
	void set_drawing_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}

	void play_sound(const char * sound_path) {
		using original_fn = void(__thiscall*)(i_surface*, const char*);
		return (*(original_fn**)this)[82](this, sound_path);
	}

	void set_text_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}
	void draw_polygon(int n, vertex_t *vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(i_surface*, int, vertex_t*, bool);
		return (*(original_fn**)this)[106](this, n, vertice, clip_vertices);
	}
	void draw_filled_rectangle(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x, y, x + w, y + h);
	}

	void draw_filled_rectangle_alt(int x0, int y0, int x1, int y1) {
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		return (*(original_fn**)this)[16](this, x0, y0, x1, y1);
	}
	void set_texture(int id) {
		using original_fn = void(__thiscall*)(i_surface*, int);
		return (*(original_fn**)this)[38](this, id);
	}
	inline void draw_textured_rectangle(int x, int y, int w, int h) {
		typedef void(__thiscall *original_fn)(void *, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, w, h);
	}
	void set_texture_rgba(int id, const unsigned char *rgba, int wide, int tall) {
		using original_fn = void(__thiscall*)(i_surface*, int, const unsigned char *, int, int, int, bool);
		return (*(original_fn**)this)[37](this, id, rgba, wide, tall, 0, false);
	}
	int create_new_texture_id(bool procedural = false) {
		using original_fn = int(__thiscall*)(i_surface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}
	void draw_outlined_rect(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[18](this, x, y, x + w, y + h);
	}
	void draw_outlined_rect_alt(int x0, int y0, int x1, int y1) {
		using original_fn = void(__thiscall*)(PVOID, int, int, int, int);
		return (*(original_fn**)this)[18](this, x0, y0, x1, y1);
	}
	void draw_line(int x1, int y1, int x2, int y2) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[19](this, x1, y1, x2, y2);
	}
	void draw_text_font(unsigned long font) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long);
		return (*(original_fn**)this)[23](this, font);
	}
	void draw_text_pos(int x, int y) {
		using original_fn = void(__thiscall*)(i_surface*, int, int);
		return (*(original_fn**)this)[26](this, x, y);
	}
	void draw_render_text(const wchar_t* text, int textLen) {
		using original_fn = void(__thiscall*)(i_surface*, const wchar_t*, int, int);
		return (*(original_fn**)this)[28](this, text, textLen, 0);
	}
	unsigned long font_create() {
		using original_fn = unsigned int(__thiscall*)(i_surface*);
		return (*(original_fn**)this)[71](this);
	}
	void set_font_glyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long, const char*, int, int, int, int, int, int, int);
		return (*(original_fn**)this)[72](this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) {
		using original_fn = void(__thiscall*)(i_surface*, unsigned long, const wchar_t*, int&, int&);
		return (*(original_fn**)this)[79](this, font, text, wide, tall);
	}
	void unlock_cursor() {
		using original_fn = void(__thiscall*)(i_surface*);
		return (*(original_fn**)this)[66](this);
	}

	auto draw_gradient_rect(int x, int y, int w, int h, bool reversed, float alpha, int type) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int, unsigned int, unsigned int, bool);
		return (*(original_fn * *)this)[123](this, x, y, x + w, y + h, reversed ? alpha : 0, reversed ? 0 : alpha, type == 0);
	}

	void filled_rect(int x, int y, int w, int h, color_alt color)
	{
		set_drawing_color(color.red, color.green, color.blue, color.alpha);
		draw_filled_rectangle_alt(x, y, x + w, y + h);
	}

	void outlined_rect(int x, int y, int w, int h, color_alt color)
	{
		set_drawing_color(color.red, color.green, color.blue, color.alpha);
		draw_outlined_rect_alt(x, y, x + w, y + h);
	}

	void set_text_color_alt(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(i_surface*, int, int, int, int);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}

	void draw_text(int x, int y, unsigned long font, const char* string, bool text_centered, color_alt colour) {
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, string);
		_vsnprintf(buf, sizeof(buf), string, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		int width, height;
		get_text_size(font, wbuf, width, height);

		set_text_color_alt(colour.red, colour.green, colour.blue, colour.alpha);
		draw_text_font(font);
		if (text_centered)
			draw_text_pos(x - (width / 2), y);
		else
			draw_text_pos(x, y);
		draw_render_text(wbuf, wcslen(wbuf));
	}

	void rounded_filled_rect(int x, int y, int width, int height, float radius, color_alt col) { // UC https://www.unknowncheats.me/forum/1498179-post4.html
		// TODO: make the quality not hardcoded -green
		// don't you have to give it quality in the formula you wrote? 8 + 4 * ( quality ) ? -dex
		constexpr int quality = 24;

		static vertex_t verts[quality];

		vec2_t add = { 0, 0 };

		for (int i = 0; i < quality; i++) {
			float angle = (static_cast <float> (i) / -quality) * 6.28f - (6.28f / 16.f);

			verts[i].position.x = radius + x + add.x + (radius * sin(angle));
			verts[i].position.y = height - radius + y + add.y + (radius * cos(angle));

			if (i == 4) {
				add.y = -height + (radius * 2);
			}
			else if (i == 10) {
				add.x = width - (radius * 2);
			}
			else if (i == 16) {
				add.y = 0;
			}
			else if (i == 22) {
				add.x = 0;
			}
		}
		set_drawing_color(col.red, col.green, col.blue, col.alpha);
		draw_polygon(quality, verts);
	}

	vec2_t get_mouse_position() // bolbi ware
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool mouse_in_region(int x, int y, int x2, int y2) {
		if (get_mouse_position().x > x && get_mouse_position().y > y && get_mouse_position().x < x2 + x && get_mouse_position().y < y2 + y)
			return true;
		return false;
	}

	void draw_t(int X, int Y, color_alt Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int Width = 0, Height = 0;

		if (Center)
		get_text_size(Font, WideBuffer, Width, Height);

		set_drawing_color(Color.red, Color.green, Color.blue, Color.alpha);
		draw_text_font(Font);
		draw_text_pos(X - (Width / 2), Y);
		draw_render_text(WideBuffer, wcslen(WideBuffer));
	}

	RECT get_text_size_alt(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		get_text_size(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}
	void draw_gradient_rect(int x, int y, int w, int h, color_alt first, color_alt second, gradient_type_t type) {
		first.alpha = (static_cast<int>(first.alpha));
		second.alpha = (static_cast<int>(second.alpha));

		static auto blend = [](const color_alt & first, const color_alt & second, float t) -> color_alt {
			return color_alt(
				first.red + t * (second.red - first.red),
				first.green + t * (second.green - first.green),
				first.blue + t * (second.blue - first.blue),
				first.alpha + t * (second.alpha - first.alpha));
		};

		if (first.alpha == 255 || second.alpha == 255) {
			set_drawing_color(blend(first, second, 0.5f).red, blend(first, second, 0.5f).green, blend(first, second, 0.5f).blue, blend(first, second, 0.5f).alpha);
			draw_filled_rectangle(x, y, w, h);
		}

		set_drawing_color(first.red, first.green, first.blue, first.alpha);
		draw_gradient_rect(x, y, w, h, true, first.alpha, type);

		set_drawing_color(second.red, second.green, second.blue, second.alpha);
		draw_gradient_rect(x, y, w, h, false, second.alpha, type);
	}

	RECT get_text_size_rect(DWORD font, const char* text)
	{
		if (this != nullptr) {
			size_t origsize = strlen(text) + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t wcstring[newsize];
			mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

			RECT rect; int x, y;
			get_text_size(font, wcstring, x, y);
			rect.left = x; rect.bottom = y;
			rect.right = x;
			return rect;
		}
	}
};