#include "AllEffects.h"
#include <KeyStates.h>
#include "./../Players/HumanPlayer.h"
#include <algorithm>    // std::random_shuffle

#include <GraphicEngine/GraphicEngine.h>

////////////////////////////////////////////
///	WEAKENINGS 	////////////////////////////
////////////////////////////////////////////

// ===============================================================================================//
//
//	CONTINOUS DAMAGE EFFECT
//
// ===============================================================================================//

//================================================================
// BURNED
//================================================================
Burned::Burned(float time, float d):Effect(time, WEAK_BURNED){
	damage = d;
	createSoundEvent();
	playEffectEvent();
}

void Burned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Burned::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Burn");
}

//================================================================
// FROZEN
//================================================================
Frozen::Frozen(float time, float d):Effect(time, WEAK_FROZEN){
	createSoundEvent();
	damage = d;
}

void Frozen::UpdateEffect(Player* p){
	//slow gradually and then freeze
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Frozen::ApplyEffect(Player* p){
	playEffectEvent();
	p->max_velocity /= 1000.0f;
	p->canJump = false;
}

void Frozen::RemoveEffect(Player* p){
	p->max_velocity *= 1000.0f;
	p->canJump = true;
}

void Frozen::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Freezing");
}

//================================================================
// POISON
//================================================================
Poisoned::Poisoned(float time, float d):Effect(time, WEAK_POISONED){
	createSoundEvent();
	playEffectEvent();
	damage = d;
}

void Poisoned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Poisoned::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Poison");
}

// ===============================================================================================//
//
//	ACTIVE AND RELEASE EFFECTS
//
// ===============================================================================================//

//================================================================
// SLOWED DOWN
//================================================================
SlowedDown::SlowedDown(float time, float value):Effect(time, WEAK_SLOWEDDOWN){
	createSoundEvent();
	playEffectEvent();
	factor = value;
}

void SlowedDown::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void SlowedDown::ApplyEffect(Player* p){
	p->max_velocity /= factor;
}

void SlowedDown::RemoveEffect(Player* p){
	p->max_velocity *= factor;
}

void SlowedDown::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Slow Down");
}
//================================================================
// PARALYZED
//================================================================
Paralyzed::Paralyzed(float time):Effect(time, WEAK_PARALYZED){
	createSoundEvent();
}

void Paralyzed::ApplyEffect(Player* p){
	playEffectEvent();
	p->max_velocity /= 1000.0f;
	p->canJump = false;
}

void Paralyzed::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Paralyzed::RemoveEffect(Player* p){
	p->max_velocity *= 1000.0f;
	p->canJump = true;	
}

void Paralyzed::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Paralysis");
}

//================================================================
// SILENCED
//================================================================
Silenced::Silenced(float time):Effect(time, WEAK_SILENCED){
	createSoundEvent();
}

void Silenced::ApplyEffect(Player* p){
	playEffectEvent();
}

void Silenced::RemoveEffect(Player* p){
}

void Silenced::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Silenced::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Silence");
}
//================================================================
// MADNESS
//================================================================
Madness::Madness(float time):Effect(time, WEAK_MADNESS){
	actions.push_back(ACTION_MOVE_UP);
	actions.push_back(ACTION_MOVE_DOWN);
	actions.push_back(ACTION_MOVE_LEFT);
	actions.push_back(ACTION_MOVE_RIGHT);
	createSoundEvent();
}

void Madness::ApplyEffect(Player* p){
	playEffectEvent();
	std::random_shuffle ( actions.begin(), actions.end() );
	
	// create random key assign
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);

}

void Madness::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Madness::RemoveEffect(Player* p){
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);
	actions.clear();
}

void Madness::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Madness");
}

//================================================================
// DeathSnare
//================================================================
DeathSnare::DeathSnare(float time, float d):Effect(time, WEAK_DEATHSNARE){
	createSoundEvent();
	damage = d;
}

void DeathSnare::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void DeathSnare::ApplyEffect(Player* p){
	playEffectEvent();
	p->max_velocity /= 1000.0f;
	p->canJump = false;
}

void DeathSnare::RemoveEffect(Player* p){
	p->max_velocity *= 1000.0f;
	p->canJump = true;
}

void DeathSnare::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Death Snare"); //MORTAL INMOVILITATION
}

//================================================================
// FUZZY EFFECT
//================================================================
Fuzzy::Fuzzy(float time):Effect(time, WEAK_FUZZY){
	createSoundEvent();
}

void Fuzzy::ApplyEffect(Player* p){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	g_engine->ChangeMeshShader(DISTORSION_SHADER);
	p->SetShader(STANDARD_SHADER);
}

void Fuzzy::RemoveEffect(Player* p){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	g_engine->ChangeMeshShader(STANDARD_SHADER);
}

////////////////////////////////////////////
///	UPGRADES 	////////////////////////////
////////////////////////////////////////////

//================================================================
// DAMAGE UP (TARION)
//================================================================
DamageUp::DamageUp(float time):Effect(time, POWERUP_DAMAGE){
	createSoundEvent();
}

void DamageUp::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_DamageMult *= 1.6;
}

void DamageUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void DamageUp::RemoveEffect(Player* p){
	p->m_DamageMult /= 1.6;				// 1.6 * 0.625 = 1
}

void DamageUp::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Imprecantes Tarion");
}

//================================================================
// DEFENSE UP (OHMIO)
//================================================================
DefenseUp::DefenseUp(float time):Effect(time, POWERUP_DEFENSE){
	createSoundEvent();
}

void DefenseUp::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_Defense *= 1.6;
}

void DefenseUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void DefenseUp::RemoveEffect(Player* p){
	p->m_Defense /= 1.6;
}

void DefenseUp::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Ohmio Protection");
}

/*
//================================================================
// ELEMENT UP
//================================================================
ElementUp::ElementUp(float time):Effect(time, POWERUP_ELEMDEFENSE){
}

void ElementUp::ApplyEffect(Player* p){
	p->m_ElementDefense *= 1.6;
}

void ElementUp::RemoveEffect(Player* p){
	p->m_ElementDefense /= 1.6;
}
*/

//================================================================
// SPEED UP (Hermes)
//================================================================
SpeedUp::SpeedUp(float time):Effect(time, POWERUP_SPEED){
	createSoundEvent();
}

void SpeedUp::ApplyEffect(Player* p){
	p->max_velocity *= 3.0f;
	// TODO: APPLY COOLDOWN REDUCTION
	playEffectEvent();
}

void SpeedUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void SpeedUp::RemoveEffect(Player* p){
	p->max_velocity /= 3.0f;
}

void SpeedUp::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Hermes' Slippers");
}

//================================================================
// UNTARGETABLE (Poncho)
//================================================================
Untargetable::Untargetable(float time):Effect(time, POWERUP_UNTARGET){
	createSoundEvent();
}

void Untargetable::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_Defense *= 10.0f;
}

void Untargetable::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Untargetable::RemoveEffect(Player* p){
	p->m_Defense /= 10.0f;
}

void Untargetable::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Divina Poncho");
}

//================================================================
// FIRE SHOTS
//================================================================
FireShots::FireShots(float time):Effect(time, POWERUP_FIRE){
	createSoundEvent();
}

void FireShots::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_shotEffect = WEAK_SOFTBURNED;
}

void FireShots::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void FireShots::RemoveEffect(Player* p){
	p->m_shotEffect = WEAK_BASIC;
}

void FireShots::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Beelzebub's Staff");
}

//================================================================
// POISON SHOTS
//================================================================
PoisonShots::PoisonShots(float time):Effect(time, POWERUP_POISON){
	createSoundEvent();
}

void PoisonShots::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_shotEffect = WEAK_SOFTBURNED;
}

void PoisonShots::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void PoisonShots::RemoveEffect(Player* p){
	p->m_shotEffect = WEAK_BASIC;
}

void PoisonShots::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Mandrake Tusk");
}

//================================================================
// INVISIBLE
//================================================================
Invisible::Invisible(float time):Effect(time, POWERUP_INVISIBLE){
	createSoundEvent();
}

void Invisible::ApplyEffect(Player* p){
	playEffectEvent();
	p->SetVisible(false);
}

void Invisible::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Invisible::RemoveEffect(Player* p){
	p->SetVisible(true);	
}

void Invisible::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Invisibility Cape");
}