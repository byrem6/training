#include "mapstdafx.h"
#include "map_particle.h"

using namespace cocos2d;

#if (BYTE_ORDER == LITTLE_ENDIAN)

#define RevisionInfo(info)	revisionParticleInfo(info)

void revisionParticleInfo(mapParticleSystemInfo& info)
{
//	TO_MACHINE_ENDIAN
	TO_MACHINE_ENDIAN(info.handle.texture);
	TO_MACHINE_ENDIAN(info.handle.blend);
	/*
	TO_MACHINE_ENDIAN(info.m_fElapsed);
	TO_MACHINE_ENDIAN(info.gravity.x);
	TO_MACHINE_ENDIAN(info.gravity.y);
	TO_MACHINE_ENDIAN(info.speed);
	TO_MACHINE_ENDIAN(info.speedVar);
	TO_MACHINE_ENDIAN(info.tangentialAccel);
	TO_MACHINE_ENDIAN(info.tangentialAccelVar);
	TO_MACHINE_ENDIAN(info.radialAccel);
	TO_MACHINE_ENDIAN(info.radialAccelVar);
	*/
	int size = sizeof(mapParticleSystemInfo)-4;
	XU8 *handle = (XU8*)(&info);
	handle+=4;
	for(int i=0;i<size;i+=4)
	{
		TO_MACHINE_ENDIAN(*((XU32*)(handle+i)));
	}
}
#else

#define RevisionInfo(info)

#endif

map_particleResource::map_particleResource()
{
	m_hTexture = NULL;
}
map_particleResource::~map_particleResource()
{
	if(m_hTexture != NULL)
		delete m_hTexture;
}

bool map_particleResource::init()
{
#define particleTexture "particles.png"
	CCImage img;
	CCFileData data(particleTexture,"rb");
	XU32 size = data.getSize();
	XU8 *buf = data.getBuffer();
	img.initWithImageData(buf,size);
	CCTexture2D *texture = new CCTexture2D();
	bool err = texture->initWithImage(&img);
	if(!err)
	{
		delete texture;
		return false;
	}
	for(int i = 0 ; i<4 ; i++)
	{
		for(int j=0 ; j<4 ; j++)
		{
			int offset = i*16+j*4;
			m_rTexRect[offset] = j*32;
			m_rTexRect[offset+1] = i*32;
			m_rTexRect[offset+2] = 32;
			m_rTexRect[offset+3] = 32;
		}
	}
	return true;
}
CCParticleSystemQuad* map_particleResource::loadPSI(const char* fname)
{
	mapParticleSystemInfo	info;
	util_file fl;
	fl.Open(fname,util_file::XREAD | util_file::XBINARY);
	fl.Read(&info,sizeof(mapParticleSystemInfo));
	fl.Close();

	RevisionInfo(info);

	map_particle *particle= (map_particle*)map_particle::node();

	particle->m_fElapsed = info.m_fElapsed;
	//modeA
	particle->modeA.gravity.x = info.gravity.x;
	particle->modeA.gravity.y = info.gravity.y;
	particle->modeA.speed = info.speed;
	particle->modeA.speedVar = info.speedVar;
	particle->modeA.tangentialAccel = info.tangentialAccel;
	particle->modeA.tangentialAccelVar = info.tangentialAccelVar;
	particle->modeA.radialAccel = info.radialAccel;
	particle->modeA.radialAccelVar = info.radialAccelVar;
	//modeB
	particle->modeB.startRadius = info.startRadius;
	particle->modeB.startRadiusVar = info.startRadiusVar;
	particle->modeB.endRadius = info.endRadius;
	particle->modeB.endRadiusVar = info.endRadiusVar;
	particle->modeB.rotatePerSecond = info.rotatePerSecond;
	particle->modeB.rotatePerSecondVar = info.rotatePerSecondVar;

	particle->m_fEmitCounter = info.m_fEmitCounter;
	particle->m_fDuration = info.m_fDuration;

	particle->m_tSourcePosition.x = info.m_tSourcePosition.x;
	particle->m_tSourcePosition.y = info.m_tSourcePosition.y;
	particle->m_tPosVar.x = info.m_tPosVar.x;
	particle->m_tPosVar.y = info.m_tPosVar.y;

	particle->m_fLife = info.m_fLife;
	particle->m_fLifeVar = info.m_fLifeVar;

	particle->m_fAngle = info.m_fAngle;
	particle->m_fAngleVar = info.m_fAngleVar;

	particle->m_fStartSize = info.m_fStartSize;
	particle->m_fStartSizeVar = info.m_fStartSizeVar;
	particle->m_fEndSize = info.m_fEndSize;
	particle->m_fEndSizeVar = info.m_fEndSizeVar;

#define C2C(C1,C2) {C1.r=C2.r; C1.g = C2.g; C1.b=C2.b; C1.a=C2.a;}
	C2C(particle->m_tStartColor,info.m_tStartColor);
	C2C(particle->m_tStartColorVar,info.m_tStartColorVar);
	C2C(particle->m_tEndColor,info.m_tEndColor);
	C2C(particle->m_tEndColorVar,info.m_tEndColorVar);
#undef C2C

	particle->m_fStartSpin = info.m_fStartSpin;
	particle->m_fStartSpinVar = info.m_fStartSpinVar;
	particle->m_fEndSpin = info.m_fEndSpin;
	particle->m_fEndSpinVar = info.m_fEndSpinVar;

	particle->m_fEmissionRate = info.m_fEmissionRate;

	particle->m_uTotalParticles = info.m_uTotalParticles;

	particle->m_bIsBlendAdditive = info.handle.blend;

	particle->m_nEmitterMode = info.m_nEmitterMode;

	int offset = info.handle.texture*4;
	particle->setTextureWithRect(m_hTexture,CCRect(m_rTexRect[offset],m_rTexRect[offset+1],m_rTexRect[offset+2],m_rTexRect[offset+3]));

	//0xbf800000  float -1
	return particle;
}

bool map_particleResource::loadPSI(mapParticleSystemInfo* info,const char* fname)
{
	util_file fl;
	if(!fl.Open(fname,util_file::XREAD | util_file::XBINARY))
		return false;
	fl.Read(info,sizeof(mapParticleSystemInfo));
	fl.Close();

	RevisionInfo(*info);
	return true;
}

#ifdef WIN32
void map_particleResource::savePSI(mapParticleSystemInfo info,int n)
{
	char fname[256];
	sprintf(fname,"particle%d.psi",n);

	RevisionInfo(info);

	util_file fl;
	fl.Open(fname,util_file::XCREATE|util_file::XBINARY);
	fl.Write(&info,sizeof(mapParticleSystemInfo));
	fl.Close();
}
#endif//WIN32

void map_particle::setParticle(const mapParticleSystemInfo& info)
{
	CCTextureCache *cache = CCTextureCache::sharedTextureCache();
	CCTexture2D *texture = cache->addImage("particles.png");

	m_fElapsed = info.m_fElapsed;
	//modeA
	modeA.gravity.x = info.gravity.x;
	modeA.gravity.y = info.gravity.y;
	modeA.speed = info.speed;
	modeA.speedVar = info.speedVar;
	modeA.tangentialAccel = info.tangentialAccel;
	modeA.tangentialAccelVar = info.tangentialAccelVar;
	modeA.radialAccel = info.radialAccel;
	modeA.radialAccelVar = info.radialAccelVar;
	//modeB
	modeB.startRadius = info.startRadius;
	modeB.startRadiusVar = info.startRadiusVar;
	modeB.endRadius = info.endRadius;
	modeB.endRadiusVar = info.endRadiusVar;
	modeB.rotatePerSecond = info.rotatePerSecond;
	modeB.rotatePerSecondVar = info.rotatePerSecondVar;

	m_fEmitCounter = info.m_fEmitCounter;
	m_fDuration = info.m_fDuration;

	m_tSourcePosition.x = info.m_tSourcePosition.x;
	m_tSourcePosition.y = info.m_tSourcePosition.y;
	m_tPosVar.x = info.m_tPosVar.x;
	m_tPosVar.y = info.m_tPosVar.y;

	m_fLife = info.m_fLife;
	m_fLifeVar = info.m_fLifeVar;

	m_fAngle = info.m_fAngle;
	m_fAngleVar = info.m_fAngleVar;

	m_fStartSize = info.m_fStartSize;
	m_fStartSizeVar = info.m_fStartSizeVar;
	m_fEndSize = info.m_fEndSize;
	m_fEndSizeVar = info.m_fEndSizeVar;

#define C2C(C1,C2) {C1.r=C2.r; C1.g = C2.g; C1.b=C2.b; C1.a=C2.a;}
	C2C(m_tStartColor,info.m_tStartColor);
	C2C(m_tStartColorVar,info.m_tStartColorVar);
	C2C(m_tEndColor,info.m_tEndColor);
	C2C(m_tEndColorVar,info.m_tEndColorVar);
#undef C2C

	m_fStartSpin = info.m_fStartSpin;
	m_fStartSpinVar = info.m_fStartSpinVar;
	m_fEndSpin = info.m_fEndSpin;
	m_fEndSpinVar = info.m_fEndSpinVar;

	m_fEmissionRate = info.m_fEmissionRate;

	m_uTotalParticles = info.m_uTotalParticles;

	m_bIsBlendAdditive = info.handle.blend;

	m_nEmitterMode = info.m_nEmitterMode;

	int offset = info.handle.texture;
	CCRect rt(offset%4*32,offset/4*32,32,32);
	setTextureWithRect(texture,CCRect(offset%4*32,offset/4*32,32,32));
}