#pragma once
extern std::map<std::string, int> AllowedModules;
int GetModules();
bool CheckIfOutsideModule(void* returnAddress);
void GetWhitelistedModules();
bool CheckForHooks(void* function);
