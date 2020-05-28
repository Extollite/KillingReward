#include "global.h"

//class WildCommand : public Command {
//public:
//  WildCommand() {}
//
//  void execute(CommandOrigin const &origin, CommandOutput &output) {
//    if (origin.getOriginType() != CommandOriginType::Player) {
//      output.error("commands.generic.error.invalidPlayer");
//      return;
//    }
//    auto source = (Player *) origin.getEntity();
//    auto &db    = Mod::PlayerDatabase::GetInstance();
//    auto entry  = db.Find(source);
//    if (!entry) return;
//    Vec3 pos = entry->player->getPos();
//    std::string name = entry->name;
//    int x            = pos.x;
//    int z            = pos.z;
//    auto originCommand      = Mod::CustomCommandOrigin::CopyFrom(origin);
//    std::string command =
//        "spreadplayers "+std::to_string(x)+" "+std::to_string(z)+" 0.0 5000.0 @s";
//    auto value              = Mod::CommandSupport::GetInstance().ExecuteCommand(std::move(originCommand), command);
//    output.success("Teleporting!");
//  }
//
//  static void setup(CommandRegistry *registry) {
//    using namespace commands;
//    registry->registerCommand("wild", "", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);
//    registry->registerOverload<WildCommand>("wild");
//  }
//};

class SuicideCommand : public Command {
public:
  SuicideCommand() {}

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    if (origin.getOriginType() != CommandOriginType::Player) {
      output.error("commands.generic.error.invalidPlayer");
      return;
    }
    auto source = (Player *) origin.getEntity();
    source->kill();
    output.success("You killed yourself!");
  }

  static void setup(CommandRegistry *registry) {
    using namespace commands;
    registry->registerCommand("suicide", "", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);
    registry->registerOverload<SuicideCommand>("suicide");
  }
};

class BackCommand : public Command {
public:
  BackCommand() {}

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    if (origin.getOriginType() != CommandOriginType::Player) {
      output.error("commands.generic.error.invalidPlayer");
      return;
    }
    auto source = (Player *) origin.getEntity();
    auto &db    = Mod::PlayerDatabase::GetInstance();
    auto it     = db.Find(source);
    if (!it) return;
    if (deaths.count(it->xuid) == 0) { 
        output.error("You don't have death point!");
      return;
    }
    DeathData death = deaths[it->xuid];
    long long now =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    if (now - death.death_time > 30000) {
      output.error("It has been 30 second from your death, you can't teleport back!");
    } else {
      source->teleport(death.pos, {0}, death.dim);
      output.success("Teleporting to death position!");
    }
    deaths.erase(it->xuid);
  }

  static void setup(CommandRegistry *registry) {
    using namespace commands;
    registry->registerCommand("back", "", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);
    registry->registerOverload<BackCommand>("back");
  }
};

void initCommand(CommandRegistry *registry) {
  //WildCommand::setup(registry);
  SuicideCommand::setup(registry);
  BackCommand::setup(registry);
}