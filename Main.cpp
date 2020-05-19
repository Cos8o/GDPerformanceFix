#include "Main.hpp"

//Error message

void logError(std::string const& s, bool gle)
{
	std::string msg("ERROR: " + s);

	if (gle)
		msg += " (" + std::to_string(GetLastError()) + ')';

	std::cout << msg << '\n';
	system("pause>nul");
}

//GD version as string

std::string getVersionAsString(uint32_t const v)
{
	switch (v)
	{
		//1.9XX
	case GD_VERSION_1900:
		return std::string("1.900 (Original)");
	case GD_VERSION_1900_PATCHED:
		return std::string("1.900 (Patched)");
	case GD_VERSION_1910:
		return std::string("1.910 (Original)");
	case GD_VERSION_1910_PATCHED:
		return std::string("1.910 (Patched)");
	case GD_VERSION_1920:
		return std::string("1.920 (Original)");
	case GD_VERSION_1920_PATCHED:
		return std::string("1.920 (Patched)");
		//2.0XX
	case GD_VERSION_2000:
		return std::string("2.000 (Original)");
	case GD_VERSION_2000_PATCHED:
		return std::string("2.000 (Patched)");
	case GD_VERSION_2001:
		return std::string("2.001 (Original)");
	case GD_VERSION_2001_PATCHED:
		return std::string("2.001 (Patched)");
	case GD_VERSION_2010:
		return std::string("2.010 (Original)");
	case GD_VERSION_2010_PATCHED:
		return std::string("2.010 (Patched)");
	case GD_VERSION_2020:
		return std::string("2.020 (Original)");
	case GD_VERSION_2020_PATCHED:
		return std::string("2.020 (Patched)");
		//2.1XX
	case GD_VERSION_2100:
		return std::string("2.100 (Original)");
	case GD_VERSION_2100_PATCHED:
		return std::string("2.100 (Patched)");
	case GD_VERSION_2101:
		return std::string("2.101 (Original)");
	case GD_VERSION_2101_PATCHED:
		return std::string("2.101 (Patched)");
	case GD_VERSION_2102:
		return std::string("2.102 (Original)");
	case GD_VERSION_2102_PATCHED:
		return std::string("2.102 (Patched)");
	case GD_VERSION_2110:
		return std::string("2.110 (Original)");
	case GD_VERSION_2110_PATCHED:
		return std::string("2.110 (Patched)");
	case GD_VERSION_2111:
		return std::string("2.111 (Original)");
	case GD_VERSION_2111_PATCHED:
		return std::string("2.111 (Patched)");
	case GD_VERSION_2112:
		return std::string("2.112 (Original)");
	case GD_VERSION_2112_PATCHED:
		return std::string("2.112 (Patched)");
	case GD_VERSION_2113:
		return std::string("2.113 (Original)");
	case GD_VERSION_2113_PATCHED:
		return std::string("2.113 (Patched)");
	default:
		return std::string("UNKNOWN");
	}
}

//Mapping utilities

HANDLE createMap(std::wstring const& s)
{
	HANDLE map = NULL;

	auto file = CreateFileW(
		s.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file != INVALID_HANDLE_VALUE)
	{
		map = CreateFileMappingW(
			file,
			NULL,
			PAGE_READWRITE,
			NULL,
			NULL,
			NULL);

		CloseHandle(file);
	}

	return map;
}

uintptr_t initMap(HANDLE const map)
{
	if (map)
	{
		return reinterpret_cast<uintptr_t>(
			MapViewOfFile(
				map,
				FILE_MAP_READ | FILE_MAP_WRITE,
				NULL,
				NULL,
				NULL));
	}

	return NULL;
}

bool freeMap(HANDLE map, uintptr_t base)
{
	return UnmapViewOfFile(reinterpret_cast<LPVOID>(base)) &&
		CloseHandle(map);
}

//Find GD path from running process

void getProcessPath(std::wstring& path)
{
	DWORD id = 0, retCode = 0;
	auto p = new wchar_t[MAX_UNICODE_PATH];

	auto window = FindWindowW(NULL, L"Geometry Dash");

	std::cout << "Waiting for Geometry Dash...\n";

	while (!window)
	{
		Sleep(250);
		window = FindWindowW(NULL, L"Geometry Dash");
	}

	GetWindowThreadProcessId(window, &id);

	auto process = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | SYNCHRONIZE,
		FALSE,
		id);

	if (process != INVALID_HANDLE_VALUE)
	{
		auto len = GetModuleFileNameExW(
			process,
			NULL,
			p,
			MAX_UNICODE_PATH);

		if (len)
		{
			path = std::wstring(
				p,
				p + len);

			std::cout << "Close the game to continue.\n";

			WaitForSingleObject(process, INFINITE);
		}
	}

	delete[] p;
}

//Main

int wmain(
	int const argc,
	wchar_t const* const* argv)
{
	std::wstring filePath;

	SetConsoleTitleA("Geometry Dash Performance Fix");

	system("cls");

	std::cout << "Geometry Dash Performance Fix\n";
	std::cout << "By SMJS & Cos8o\n";

	if (argc > 1)
	{
		auto len = wcslen(argv[1]);
		filePath = std::wstring(argv[1], argv[1] + len);
	}
	else
	{
		getProcessPath(filePath);
	}

	if (!filePath.size())
	{
		logError("File not found.", false);
		return 1;
	}

	//Map file

	auto map = createMap(filePath);
	auto base = initMap(map);

	if (!map || !base)
	{
		logError("Could not map file in memory.", true);
		return 1;
	}

	//Find GD version

	auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
	auto nt = reinterpret_cast<PIMAGE_NT_HEADERS32>(base + dos->e_lfanew);

	if (dos->e_magic != IMAGE_DOS_SIGNATURE ||
		nt->Signature != IMAGE_NT_SIGNATURE)
	{
		logError("Invalid file.", false);
		return 1;
	}

	auto version = nt->FileHeader.TimeDateStamp;
	auto patches = BYTE_PATCHES.find(version);

	if (patches == BYTE_PATCHES.end())
	{
		logError("Invalid version.", false);
		return 1;
	}

	std::cout << "Version found: " << getVersionAsString(version) << '\n';

	//Apply patches

	std::cout << "Patching..." << '\n';

	for (auto const& patch : patches->second)
	{
		auto address = base + patch.first;
		auto const& bytes = patch.second;

		memcpy(
			reinterpret_cast<void*>(address),
			bytes.data(),
			bytes.size());

		if (!FlushViewOfFile(
			reinterpret_cast<LPVOID>(address),
			bytes.size()))
		{
			logError("Patch failed.", true);
			return 1;
		}
	}

	//Patch timestamp

	nt->FileHeader.TimeDateStamp ^= 0xDEADBEEF;

	if (!FlushViewOfFile(
		reinterpret_cast<LPVOID>(&nt->FileHeader.TimeDateStamp),
		sizeof(DWORD)))
	{
		logError("Patch failed.", false);
		return 1;
	}

	//Cleanup

	if (!freeMap(map, base))
	{
		logError("Patch failed.", true);
		return 1;
	}

	std::cout << "Patch applied!\n";
	std::cout << "Press any button to exit." << std::endl;
	system("pause>nul");

	return 0;
}