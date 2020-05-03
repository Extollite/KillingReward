#pragma once

#include <yaml.h>
#include <string>
#include <map>
#include <log.h>
#include "playerdb.h"
#include <Actor/Actor.h>
#include <Actor/Player.h>
#include <economy.h>
#include <command.h>
#include <Command/Command.h>
#include <Command/CommandRegistry.h>
#include <Command/CommandPermissionLevel.h>
#include <Command/CommandFlag.h>
#include <Command/CommandParameterData.h>
#include <Command/CommandOutput.h>
#include <Command/CommandOrigin.h>

#include <Packet/TransferPacket.h>
#include <Packet/TextPacket.h>

#include <Core/ServerInstance.h>
#include <Command/CommandContext.h>
#include <Core/MCRESULT.h>
#include <Command/MinecraftCommands.h>

#include <Core/json.h>


struct Settings {
  std::map<std::string, long long> money;

  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.money, node["killingMoney"]);
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