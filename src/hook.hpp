#pragma once

#include "pch.h"
#include "hook-manager.h"
#include "il2cpp-appdata.h"
#include "Shellapi.h"
#include "config.hpp"
#include "util.hpp"
#include <string>

namespace hook
{
	std::string uint64_to_hex_string(uint64_t value)
	{
		std::ostringstream os;
		os << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
		return os.str();
	}

	void Load()
	{
		int nArgs;
		auto args = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		util::client_version = config::client_version;
		if (!args[1])
		{
			util::Log("Type 'method' to method RVA Dump");
			util::Log("Type 'field' to field Dump");

			while (true)
			{
				std::string input;
				std::getline(std::cin, input);
				auto cmd = util::split(input, ' ');
				if (cmd.empty())
					continue;
				auto nargs = cmd.size() - 1;

				if (cmd[0] == "method")
				{
					auto start = config::GetLongValue("TypeIndexStart", -1);
					if (
						start > -1 && 
						il2cpp__vm__MetadataCache__GetTypeInfoFromTypeDefinitionIndex != 0 && 
						il2cpp__vm__Type__GetName != 0 && 
						il2cpp__vm__Class__GetMethods != 0 && 
						il2cpp__vm__Method__GetNameWithGenericTypes != 0
						)
					{
						util::DumpMethod(start, config::GetByvalArgMagic(), config::GetMethodpointerMagic());
					}
				}
				else if (cmd[0] == "field")
				{
					auto start = config::GetLongValue("TypeIndexStart", -1);
					if (
						start > -1 &&
						il2cpp__vm__MetadataCache__GetTypeInfoFromTypeDefinitionIndex != 0 &&
						il2cpp__vm__Type__GetName != 0 &&
						il2cpp__vm__Class_GetFields != 0
						)
					{
						util::DumpField(start, config::GetByvalArgMagic());
					}
				}
				else
					util::Log("Invalid command!\n");
			}
		}
		else if (wcscmp(args[1], L"-magicA") == 0)
		{
			printf("ByvalArg = %ws\n", args[2]);
			util::CheckByvalArgMagic(std::stol(args[2]), config::client_version);
		}
		else
		{
			printf("what? %ws", args[1]);
		}
	}
}
