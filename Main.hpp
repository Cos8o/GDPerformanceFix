#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <Windows.h>
#include <Psapi.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

static constexpr auto MAX_UNICODE_PATH = 32767u;

//1.9XX

static constexpr auto GD_VERSION_1900 =			0x5496DCBD;
static constexpr auto GD_VERSION_1900_PATCHED = 0x8A3B6252;
static constexpr auto GD_VERSION_1910 =			0x54A1A988;
static constexpr auto GD_VERSION_1910_PATCHED = 0x8A0C1767;
static constexpr auto GD_VERSION_1920 =			0x54BE1CBD;
static constexpr auto GD_VERSION_1920_PATCHED = 0x8A13A252;

//2.0XX

static constexpr auto GD_VERSION_2000 =			0x55DE64F7;
static constexpr auto GD_VERSION_2000_PATCHED = 0x8B73DA18;
static constexpr auto GD_VERSION_2001 =			0x55DE7B57;
static constexpr auto GD_VERSION_2001_PATCHED =	0x8B73C5B8;
static constexpr auto GD_VERSION_2010 =			0x56033EB0;
static constexpr auto GD_VERSION_2010_PATCHED = 0x88AE805F;
static constexpr auto GD_VERSION_2020 =			0x56039ED7;
static constexpr auto GD_VERSION_2020_PATCHED = 0x88AE2038;

//2.1XX

static constexpr auto GD_VERSION_2100 =			0x587D6503;
static constexpr auto GD_VERSION_2100_PATCHED = 0x86D0DBEC;
static constexpr auto GD_VERSION_2101 =			0x587D9AE2;
static constexpr auto GD_VERSION_2101_PATCHED = 0x86D0240D;
static constexpr auto GD_VERSION_2102 =			0x587F4AB7;
static constexpr auto GD_VERSION_2102_PATCHED =	0x86D2F458;
static constexpr auto GD_VERSION_2110 =			0x5A08CFC2;
static constexpr auto GD_VERSION_2110_PATCHED = 0x84A5712D;
static constexpr auto GD_VERSION_2111 =			0x5A08FB6B;
static constexpr auto GD_VERSION_2111_PATCHED = 0x84A54584;
static constexpr auto GD_VERSION_2112 =			0x5A0A3875;
static constexpr auto GD_VERSION_2112_PATCHED = 0x84A7869A;
static constexpr auto GD_VERSION_2113 =			0x5A13638C;
static constexpr auto GD_VERSION_2113_PATCHED = 0x84BEDD63;

typedef std::pair<
	uintptr_t,
	std::vector<uint8_t>> BytePatch;

static std::unordered_map<
	uint32_t,
	std::vector<BytePatch>> const BYTE_PATCHES =
{
	{
		GD_VERSION_2113,
		{
			{ 0x3C1F7, { 0x00 }, },
			{ 0x3C209, { 0x00 }, },
		},
	},
	{
		GD_VERSION_2113_PATCHED,
		{
			{ 0x3C1F7, { 0x06 }, },
			{ 0x3C209, { 0x01 }, },
		},
	}
};

#endif /* _MAIN_HPP */