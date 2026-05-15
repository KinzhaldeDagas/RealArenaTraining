#pragma once
#include <vector>
#include <cstdint>

struct TESNPC;
struct TESRace;

namespace cs
{
    // Placeholder: enumerate NPCs by race using your project's DataHandler iteration utilities.
    // Implement this using your canonical DataHandler API (project source of truth).
    //
    // Contract: return *base* NPC forms (TESNPC*) that have the specified race.
    std::vector<TESNPC*> EnumerateNPCsByRace(TESRace* race);
}
