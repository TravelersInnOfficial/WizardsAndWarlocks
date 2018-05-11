#include "AllEffects.h"

#include "./../Players/PlayerController.h"
#include <algorithm>    // std::random_shuffle
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GParticle.h>
#include <SoundEngine/SoundSystem.h>
#include "./../Players/Player.h"

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
	particle = nullptr;
}

void Burned::ApplyEffect(Player* p){
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/FireParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Burned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Burned::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Burn");
}

void Burned::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void Burned::RemoveEffect(Player* p){
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
}

//================================================================
// FROZEN
//================================================================
Frozen::Frozen(float time, float d):Effect(time, WEAK_FROZEN){
	createSoundEvent();
	damage = d;
	particle = nullptr;
}

void Frozen::UpdateEffect(Player* p){
	//slow gradually and then freeze
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Frozen::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_max_velocity /= 1000.0f;
	p->m_CanJump = false;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/FreezeParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Frozen::RemoveEffect(Player* p){
	p->m_max_velocity *= 1000.0f;
	p->m_CanJump = true;
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
}

void Frozen::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Freezing");
}

void Frozen::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

//================================================================
// POISON
//================================================================
Poisoned::Poisoned(float time, float d):Effect(time, WEAK_POISONED){
	createSoundEvent();
	playEffectEvent();
	damage = d;
	particle = nullptr;
}

void Poisoned::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void Poisoned::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Poison");
}

void Poisoned::ApplyEffect(Player* p){
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/PoisonParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Poisoned::RemoveEffect(Player* p){
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
}

void Poisoned::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
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
	p->m_max_velocity /= factor;
}

void SlowedDown::RemoveEffect(Player* p){
	p->m_max_velocity *= factor;
}

void SlowedDown::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Slow Down");
}

//================================================================
// PARALYZED
//================================================================
Paralyzed::Paralyzed(float time):Effect(time, WEAK_PARALYZED){
	createSoundEvent();
	particle = nullptr;
}

void Paralyzed::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_max_velocity /= 1000.0f;
	p->m_CanJump = false;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/ElectricParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Paralyzed::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Paralyzed::RemoveEffect(Player* p){
	p->m_max_velocity *= 1000.0f;
	p->m_CanJump = true;	
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
}

void Paralyzed::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Paralysis");
}

void Paralyzed::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

//================================================================
// SILENCED
//================================================================
Silenced::Silenced(float time):Effect(time, WEAK_SILENCED){
	createSoundEvent();
	particle = nullptr;
}

void Silenced::ApplyEffect(Player* p){
	playEffectEvent();
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/SilencedParticles.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Silenced::RemoveEffect(Player* p){
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
}

void Silenced::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Silenced::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Silence");
}

void Silenced::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
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
	particle = nullptr;
}

void Madness::ApplyEffect(Player* p){
	playEffectEvent();
	std::random_shuffle ( actions.begin(), actions.end() );
	
	// create random key assign
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);
	
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/ConfusedParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Madness::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Madness::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void Madness::RemoveEffect(Player* p){
	p->GetController()->SwapActions(actions[0], actions[1]);
	p->GetController()->SwapActions(actions[2], actions[3]);
	actions.clear();
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
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
	particle = nullptr;
}

void DeathSnare::UpdateEffect(Player* p){
	p->ChangeHP(-damage);
	effectEvent->setPosition(p->GetPos());
}

void DeathSnare::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_max_velocity /= 1000.0f;
	p->m_CanJump = false;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/SnareParticle.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void DeathSnare::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void DeathSnare::RemoveEffect(Player* p){
	p->m_max_velocity *= 1000.0f;
	p->m_CanJump = true;
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
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
DamageUp::DamageUp(float time, float  damage):Effect(time,POWERUP_DAMAGE){
	createSoundEvent();
	m_dmg = damage;
}

void DamageUp::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_DamageMult *= m_dmg;
}

void DamageUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void DamageUp::RemoveEffect(Player* p){
	p->m_DamageMult /= m_dmg;				// 1.6 * 0.625 = 1
}

void DamageUp::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Imprecantes Tarion");
}

//================================================================
// DEFENSE UP (OHMIO)
//================================================================
DefenseUp::DefenseUp(float time):Effect(time, POWERUP_DEFENSE){
	createSoundEvent();
	particle = nullptr;
}

void DefenseUp::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_Defense *= 1.6;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/DefenseParticles.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void DefenseUp::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void DefenseUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void DefenseUp::RemoveEffect(Player* p){
	p->m_Defense /= 1.6;
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
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
SpeedUp::SpeedUp(float time, float vel):Effect(time, POWERUP_SPEED){
	createSoundEvent();
	m_vel = vel;
}

void SpeedUp::ApplyEffect(Player* p){
	p->m_max_velocity *= m_vel;
	// TODO: APPLY COOLDOWN REDUCTION
	playEffectEvent();
}

void SpeedUp::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void SpeedUp::RemoveEffect(Player* p){
	p->m_max_velocity /= m_vel;
}

void SpeedUp::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Hermes' Slippers");
}

//================================================================
// UNTARGETABLE (Poncho)
//================================================================
Untargetable::Untargetable(float time, float def):Effect(time, POWERUP_UNTARGET){
	createSoundEvent();
	particle = nullptr;
	m_def = def;
}

void Untargetable::ApplyEffect(Player* p){
	playEffectEvent();
	p->m_Defense *= m_def;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/DefenseParticles2.png");
		particle->SetType(EFFECT_PARTICLE);
		particle->SetQuantityPerSecond(100);
	}
}

void Untargetable::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void Untargetable::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Untargetable::RemoveEffect(Player* p){
	p->m_Defense /= m_def;
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
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
	p->m_shotEffect = WEAK_SOFTPOISONED;
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
	particle = nullptr;
	createSoundEvent();
}

void Invisible::ApplyEffect(Player* p){
	
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(p->GetPos());
		particle->SetTexture("./../assets/textures/particles/InvisibleParticle.png");
		particle->SetType(INVISIBLE_PARTICLE);
		particle->SetQuantityPerSecond(20);
	}

	playEffectEvent();
	p->SetVisible(false);
}

void Invisible::UpdateEffect(Player* p) {
	effectEvent->setPosition(p->GetPos());
}

void Invisible::UpdateEffectParticles(Player* p) {
	if(particle != nullptr){
		particle->SetPosSpecial(p->GetPos());
		particle->Update();
	}
}

void Invisible::RemoveEffect(Player* p){
	if(particle != nullptr){
		delete particle;
		particle = nullptr;
	}
	p->SetVisible(true);
}

void Invisible::createSoundEvent() {
	effectEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Invisibility Cape");
}