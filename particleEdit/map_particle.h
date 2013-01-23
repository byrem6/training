/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeParticleSystem helper class header
*/

#ifndef __MAP_PARTICLE_H__
#define __MAP_PARTICLE_H__

#include "cocos2d.h"
#include "util.h"
#include "map_resource_type.h"

using namespace cocos2d;

struct mapParticleHandle
{
	unsigned short texture;
	unsigned short blend;
};

struct mapParticleSystemInfo
{
	mapParticleHandle	handle;    // texture + blend mode
	float				m_fElapsed;
	//modeA
	CCPoint				gravity;
	float				speed;
	float				speedVar;
	float				tangentialAccel;
	float				tangentialAccelVar;
	float				radialAccel;
	float				radialAccelVar;
	//modeB
	float				startRadius;
	float				startRadiusVar;
	float				endRadius;
	float				endRadiusVar;
	float				rotatePerSecond;
	float				rotatePerSecondVar;

	float				m_fEmitCounter;
	float				m_fDuration;

	CCPoint				m_tSourcePosition;
	CCPoint				m_tPosVar;

	float				m_fLife;
	float				m_fLifeVar;

	float				m_fAngle;
	float				m_fAngleVar;

	float				m_fStartSize;
	float				m_fStartSizeVar;
	float				m_fEndSize;
	float				m_fEndSizeVar;

	ccColor4F			m_tStartColor;
	ccColor4F			m_tStartColorVar;
	ccColor4F			m_tEndColor;
	ccColor4F			m_tEndColorVar;

	float				m_fStartSpin;
	float				m_fStartSpinVar;
	float				m_fEndSpin;
	float				m_fEndSpinVar;

	float				m_fEmissionRate;

	unsigned int		m_uTotalParticles;

	int					m_nEmitterMode;
};

class map_particleResource
{
public:
	map_particleResource();
	~map_particleResource();
	static map_particleResource* sharedParticleResource()
	{
		static map_particleResource* single = NULL;
		if(single==NULL)
		{
			single = new map_particleResource();
			if(!single->init())
			{
				delete single;
				single=NULL;
			}
		}
		return single;
	}
public:
	bool init();
	CCParticleSystemQuad* loadPSI(const char* fname);
	static bool loadPSI(mapParticleSystemInfo* info,const char* fname);

#ifdef WIN32
	static void savePSI(mapParticleSystemInfo info,int n);
#endif//WIN32
protected:
	float			 m_rTexRect[64];
	CCTexture2D		*m_hTexture;
};

class map_particle : public CCParticleSystemQuad
{
public:
	friend class map_particleResource;
	map_particle(){setTag(KMapParticleTag);}
	~map_particle(){}
public:
	virtual bool init()
	{
		if(CCParticleSystemQuad::initWithTotalParticles(200))
		{
			return true;
		}
		return false;
	}
	// implement the "static node()" method manually
    LAYER_NODE_FUNC(map_particle);
public:
	void setParticle(const mapParticleSystemInfo& info);
};

#endif//__MAP_PARTICLE_H__