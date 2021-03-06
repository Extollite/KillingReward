#pragma once

#include <yaml.h>
#include <string>
#include <map>
#include <base/log.h>
#include "base/playerdb.h"
#include <Actor/Actor.h>
#include <Actor/Player.h>
#include <Mods/Economy.h>
#include <Mods/CommandSupport.h>

#include <Packet/TransferPacket.h>
#include <Packet/TextPacket.h>

#include <Core/ServerInstance.h>
#include <Command/CommandContext.h>
#include <Core/MCRESULT.h>
#include <Command/MinecraftCommands.h>

#include <Core/json.h>


struct Settings {
  std::map<std::string, long long> money;
  bool looseMoneyOnDeath;
  bool suicideCommand  = true;
  bool backCommand     = true;
  float percentOfMoney = 0.05;

  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.money, node["killingMoney"]) && f(settings.looseMoneyOnDeath, node["looseMoneyOnDeath"]) &&
           f(settings.percentOfMoney, node["percentOfMoney"]) && f(settings.suicideCommand, node["suicideCommand"]) &&
           f(settings.backCommand, node["backCommand"]);
  }
};

struct DeathData {
  Vec3 pos;
  long long death_time;
  AutomaticID<Dimension, int> dim;
};

DEF_LOGGER("KM");

extern Settings settings;

extern std::unordered_map<uint64_t, DeathData> deaths;

void initCommand(CommandRegistry *registry);