//
//  SoundSystem.h
//  vdrift
//
//  Created by Pierre on 20171022.
//

#ifndef SoundSystem_h
#define SoundSystem_h

#include <map>
#include <string>

using namespace std;

//#include "mathvector.h"
#include <fmod_studio.h>
#include <fmod/fmod_studio.hpp>

class SoundEvent;

class SoundSystem {
	
public:
    /**
     *  Constructor
     *  \param soundBanksPath ruta del directorio donde se encuentran los bancos generados con FMOD Studio
     */
	SoundSystem(string soundBanksPath);
    
	~SoundSystem();
	
    /**
     *  Método factoría que Construye un SoundEvent a partir del nombre de un evento de FMOD Studio, 
     *  p. ej. "event:/Ambience/Country"
     *
     *  \param eventPath nombre del evento según la nomenclatura de FMOD Studio
     */
    SoundEvent* getEvent(string eventPath);
    
    /**
     *  Modifica el volumen general del motor de sonido
     */
    void setVolume(float vol);
    
    /**
     *  Modifica la posición del punto de escucha (en esta aplicación sólo hay uno)
     */
    //void setListernerPosition(Vec3 pos);
    void setListernerPosition(float x, float y, float z);
    
    /**
     *  Actualiza el motor de audio
     *  \param paused indica si hay que pausar el motor de audio o no
     */
    void update(bool paused);
private:
    string banksPath;
	FMOD::Studio::System*	system = NULL;
	FMOD::System*			lowLevelSystem = NULL;
	FMOD::Studio::Bank* masterBank = NULL;
	FMOD::Studio::Bank* stringsBank = NULL;
    map<string, FMOD::Studio::Bank*> banks;
    map<string, FMOD::Studio::EventDescription*> eventDescriptions;
    map<string, SoundEvent*> soundEvents;
    
};

class SoundEvent {
public:
    SoundEvent();
    virtual ~SoundEvent() = 0; /**>  SoundEvent es una clase abstracta */
    
    /**
     *  Comienza a reproducir el evento
     */
    virtual void start();
    
    /**
     *  Detiene la reproducción del evento inmediatamente
     */
    void stop();
    
    /**
     *  Pausa la reproducción del evento. 
     *  Para continuar la reproducción, usar start()
     *  \sa start()
     */
    void pause();
    
    /**
     *  Modifica el volumen del evento
     *  \param vol volumen del evento: 0=silencio, 1=máximo volumen
     */
    void setVolume(float vol);
    
    /**
     *  Modifca el volumen del evento multiplicando por un factor de ganancia
     *  \param gain factor de ganancia. 0=silencio, 1=mantener el volumen
     */
    void setGain(float gain);
    
    /**
     *  Modifica la posición 3D del evento de sonido
     *  \param pos nuevo vector de posición
     */
    //void setPosition(Vec3 pos);
    void setPosition(float x, float y, float z);
    
    /**
     *  Consulta si el evento está sonando
     *  \return devuelve cierto si el evento está sonando
     */
    bool isPlaying();
    
protected:
    FMOD::Studio::EventInstance* soundInstance;
    /**
     * Este método crea un SoundEvent (EngineSound, WindSound, etc.) correspondiente
     * al evento que recibe como argumento
     */
    virtual SoundEvent* newSoundEvent(FMOD::Studio::EventInstance*) = 0;
};


#endif /* SoundSystem_h */
