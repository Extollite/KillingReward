#include "global.h"

#include <dllentry.h>


Settings settings;
std::unordered_map<uint64_t, DeathData> deaths;
DEFAULT_SETTINGS(settings);

void dllenter() { Mod::CommandSupport::GetInstance().AddListener(SIG("loaded"), initCommand); }
void dllexit() {}

void PreInit() {
}
void PostInit() {}

class ActorDamageSource {
private:
  char filler[0x10];

public:
  __declspec(dllimport) virtual void destruct1(unsigned int)     = 0;
  __declspec(dllimport) virtual bool isEntitySource() const      = 0;
  __declspec(dllimport) virtual bool isChildEntitySource() const = 0;

private:
  __declspec(dllimport) virtual void *unk0() = 0;
  __declspec(dllimport) virtual void *unk1() = 0;
  __declspec(dllimport) virtual void *unk2() = 0;
  __declspec(dllimport) virtual void *unk3() = 0;
public:
  __declspec(dllimport) virtual ActorUniqueID getEntityUniqueID() const = 0;
  __declspec(dllimport) virtual int getEntityType() const               = 0;

private:
  __declspec(dllimport) virtual int getEntityCategories() const = 0;
};



THook(void*, "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z", Mob& mob, ActorDamageSource* src) {
  if (mob.getEntityTypeId() == ActorType::Player) return original(mob, src);
  if (src->isChildEntitySource() || src->isEntitySource()) {
    Actor *ac = LocateService<Level>()->fetchEntity(src->getEntityUniqueID(), false);
    if (ac && ac->getEntityTypeId() == ActorType::Player) {
      auto &db   = Mod::PlayerDatabase::GetInstance();
      auto entry = db.Find((Player *) ac);
      if (!entry) return original(mob, src);
      if (entry->player->getCommandPermissionLevel() > CommandPermissionLevel::Any) return original(mob, src);
      Mod::Economy::UpdateBalance(entry->player, settings.money[mob.getEntityName()], "killed entity");
    }
  }
  return original(mob, src);
}

THook(void *, "?die@Player@@UEAAXAEBVActorDamageSource@@@Z", Player &thi, void *src) {
  long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  Vec3 pos = thi.getPos();
  auto &db              = Mod::PlayerDatabase::GetInstance();
  auto it               = db.Find(&thi);
  if (!it) return original(thi, src);
  DeathData deathData;
  deathData.death_time = now;
  deathData.pos        = pos;
  deathData.dim        = thi.getDimensionId();
  auto pk = TextPacket::createTextPacket<TextPacketType::SystemMessage>("You have 30 seconds to use /back!");
  thi.sendNetworkPacket(pk);
  deaths[it->xuid] = deathData;
  return original(thi, src);
}