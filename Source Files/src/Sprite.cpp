// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "Sprite.h"
#include "D3DManager.h"
#include "defines.h"

//sprite vertex format
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	float u,v;
};

// The structure of a vertex in our vertex buffer...
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

//make it easier to setup each vertex
void SetupVTGeometry( CUSTOMVERTEX* pV, int index, 
	float px, float py, float pz, 
	float u, float v)
{
	pV[index].position.x = px;	// Vertex co-ordinate.
	pV[index].position.y = py;
	pV[index].position.z = pz;
	pV[index].u = u;
	pV[index].v = v;
}

//*********************************************************************************************
/*
Animated sprites use a sequence of fixed size rectangular portions
of the texture, played back at a fixed rate to show an animation - engine thrust
*/
class SpriteAnim
{
	friend class SpriteManager;
public:
	int mCurrFrame;
	float mLastFTime;
	typedef std::vector<UVPair> AnimFrames;
	AnimFrames mAnimFrames;
	int mNumPlays;
	float mFrameRate;
	float mDegrees;
	bool mLoop;
	bool mPlaying;

	SpriteAnim() : mCurrFrame(0), mLastFTime(0), mNumPlays(0), mFrameRate(0), mLoop(false), mDegrees(0) {};
	~SpriteAnim() {}
	void Update(float aTime);
	void ReserveFrames(int nFrames) { mAnimFrames.reserve(nFrames); }
	void Play() { 
		MPOD_ASSERT_MSG(!mAnimFrames.empty(),"No frames to play");
		mPlaying=true; 
	}
	void Stop() { mPlaying=false; }
	const UVPair &GetTexCoords() const	{ 
		return mAnimFrames.at(mCurrFrame); 
	}
	void AddFrames(UVPair &uvs)	{ 
		mAnimFrames.push_back(uvs); }
	void AddFrames(float u0, float v0, float u1, float v1);
	void AddMultipleFrames(int nFrames, float tw, float th);

};


//*********************************************************************************************
//we need to track the texture loaded so we don't duplicate any
//animated sprites need a unique vertex buffer setting up
class SpriteData
{
public:
	SpriteData() : mpTexture(NULL), mpVerts(NULL), mOwnsBuffer(false), mpAnimFrames(NULL), mHdl(0xffffffff/*MAX_UINT*/) {}
	void Release()
	{
		//a unique textured sprite - potentially animated with its own vertex buffer
		if(mpTexture)
			mpTexture->Release();
		mpTexture=NULL;
		if(mOwnsBuffer && mpVerts)
			mpVerts->Release();
		mpVerts=NULL;
		if(mpAnimFrames)
			delete mpAnimFrames;
		mpAnimFrames=NULL;
	}

	std::string mTexName;
	LPDIRECT3DTEXTURE9 mpTexture;
	LPDIRECT3DVERTEXBUFFER9 mpVerts; //if null we aren't suing a custom vertex buffer
	SpriteAnim::AnimFrames *mpAnimFrames;
	unsigned int mHdl;
	bool mOwnsBuffer;
};


//********************************************************************************************
void SpriteManager::SetupMaterial(float r, float g, float b)
{
	// Define a material.
	// Reflects only diffuse colour.
	mMtl.Diffuse.r = r;
	mMtl.Diffuse.g = g;
	mMtl.Diffuse.b = b;

	mMtl.Ambient.r = r;
	mMtl.Ambient.g = g;
	mMtl.Ambient.b = b;
	mMtl.Ambient.a = 1;

	// Set material as current material
	GetD3DManager()->GetD3DDevice()->SetMaterial(&mMtl);
}


//create a vertex buffer for a quad
void CreateVB(LPDIRECT3DVERTEXBUFFER9 &pVerts)
{
	// Define the vertex buffer for the square...
	const int Vertices( 2 * 3);	// Square comprised of two triangles, 3 vertices per triangle.
	const int BufferSize( Vertices * sizeof(CUSTOMVERTEX));

	if (FAILED(GetD3DManager()->GetD3DDevice()->CreateVertexBuffer(BufferSize, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &pVerts, NULL)))
	{
		MPOD_ASSERT_MSG(false,"Vertex buffer could not be created");
		return;			
	}
	// Create a pointer to the first vertex in the buffer and lock it.
	CUSTOMVERTEX *pVertices( nullptr);

	// Lock it and fill it with appropriate vertices to describe the floor
	if (FAILED(pVerts->Lock(0, 0, (void**)&pVertices, 0)))
	{
		MPOD_ASSERT_MSG(false, "if the pointer to the vertex buffer could not be established");
		return;  // .
	}

	SetupVTGeometry( pVertices,  0,  0,  0, 0, 0, 0);
	SetupVTGeometry( pVertices,  1,  1, -1, 0, 1, 1);
	SetupVTGeometry( pVertices,  2,  0, -1, 0, 0, 1);
	SetupVTGeometry( pVertices,  3,  0,  0, 0, 0, 0);
	SetupVTGeometry( pVertices,  4,  1,  0, 0, 1, 0);
	SetupVTGeometry( pVertices,  5,  1, -1, 0, 1, 1 );

	// Unlock the vertex buffer
	pVerts->Unlock();
}

/*
Create a vertex buffer with one quad for each frame of an animation
These are made once at load for each unique animated sprite, we don't 
want to allocate buffers at run time if we can avoid it.
A shader would be ideal for this.
*/
void CreateVB(LPDIRECT3DVERTEXBUFFER9 &pVerts, SpriteAnim &spr)
{
	const int Vertices( spr.mAnimFrames.size() * 3 * 2);	// Square comprised of two triangles, 3 vertices per triangle.
	const int BufferSize( Vertices * sizeof(CUSTOMVERTEX));

	if (FAILED(GetD3DManager()->GetD3DDevice()->CreateVertexBuffer(BufferSize, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &pVerts, NULL)))
	{
		MPOD_ASSERT_MSG(false,"Vertex buffer could not be created");
		return;			
	}
	// Create a pointer to the first vertex in the buffer and lock it.
	CUSTOMVERTEX *pVertices( nullptr);

	// Lock it and fill it with appropriate vertices to describe the floor
	if (FAILED(pVerts->Lock(0, 0, (void**)&pVertices, 0)))
	{
		MPOD_ASSERT_MSG(false, "if the pointer to the vertex buffer could not be established");
		return;  // .
	}

	int count(0);
	for(SpriteAnim::AnimFrames::iterator it=spr.mAnimFrames.begin();it!=spr.mAnimFrames.end();++it)
	{
		SetupVTGeometry( pVertices,  count++,  0,  0, 0, (*it).mUVTL.x, (*it).mUVTL.y);
		SetupVTGeometry( pVertices,  count++,  1, -1, 0, (*it).mUVBR.x, (*it).mUVBR.y);
		SetupVTGeometry( pVertices,  count++,  0, -1, 0, (*it).mUVTL.x, (*it).mUVBR.y);
		SetupVTGeometry( pVertices,  count++,  0,  0, 0, (*it).mUVTL.x, (*it).mUVTL.y);
		SetupVTGeometry( pVertices,  count++,  1,  0, 0, (*it).mUVBR.x, (*it).mUVTL.y);
		SetupVTGeometry( pVertices,  count++,  1, -1, 0, (*it).mUVBR.x, (*it).mUVBR.y);
	}
	// Unlock the vertex buffer
	pVerts->Unlock();
}

SpriteManager::SpriteManager()
{
	ZeroMemory(&mMtl, sizeof(D3DMATERIAL9));
	mMtl.Diffuse.a = 1.0f;
	//the only buffer we need for an unanimated quad
	CreateVB(mpVerts);
	//we don't want to duplicate any vertex buffers or textures 
	SpriteData *p(NULL);
	mChunks.insert(mChunks.begin(),50,p);
	unsigned int count(0);
	for(Chunks::iterator it=mChunks.begin();it!=mChunks.end();++it)
	{
		(*it)=new SpriteData;
		(*it)->mHdl=count++;
	}
}

SpriteManager::~SpriteManager()
{
	if(mpVerts)
		mpVerts->Release();
	mpVerts=NULL;

	for(Chunks::iterator it=mChunks.begin();it!=mChunks.end();++it)
	{
		(*it)->Release();
		delete *it;
	}
	mChunks.clear();
}


/*
render a sprite - possibly animated
This is v1 code, sprites need to be batched together and rendered in larger 
more efficient groups ideally. Plus, setting all these render states and matrices all 
the time is not efficient, the states + world matrix need tracking so we only change the ones
we really need to change.
*/
void SpriteManager::Render( Sprite &spr )
{
	SetupMaterial(spr.mColour.r, spr.mColour.g, spr.mColour.b);

	//setup the world matrix
	D3DXMATRIX WorldMat, TranslateMat, ScaleMat, ZRotateMat;
	D3DXMatrixIdentity(&WorldMat);
	if(spr.mCentred)
	{
		//position refers to sprite centre
		D3DXMatrixScaling(&ScaleMat,spr.mWorldSize.x, spr.mWorldSize.y, 1);
		D3DXMatrixMultiply(&WorldMat, &WorldMat,&ScaleMat);
		D3DXMatrixTranslation(&TranslateMat, -spr.mWorldSize.x*0.5f, spr.mWorldSize.y*0.5f, spr.mZ);
		D3DXMatrixMultiply(&WorldMat, &WorldMat, &TranslateMat);

		D3DXMatrixRotationZ(&ZRotateMat,UDEF::D2R(spr.mDegrees));
		D3DXMatrixMultiply(&WorldMat, &WorldMat,&ZRotateMat);

		D3DXMatrixTranslation(&TranslateMat, spr.mPos.x, spr.mPos.y, 0);
		D3DXMatrixMultiply(&WorldMat, &WorldMat, &TranslateMat);
	}
	else
	{
		//position refers to sprite top left corner
		D3DXMatrixRotationZ(&ZRotateMat,UDEF::D2R(spr.mDegrees));
		D3DXMatrixTranslation(&TranslateMat, spr.mPos.x, spr.mPos.y, spr.mZ);
		D3DXMatrixScaling(&ScaleMat,spr.mWorldSize.x, spr.mWorldSize.y, 1);
		D3DXMatrixMultiply(&WorldMat, &ScaleMat, &ZRotateMat);
		D3DXMatrixMultiply(&WorldMat, &WorldMat, &TranslateMat);
	}

	LPDIRECT3DDEVICE9 d3dDev = GetD3DManager()->GetD3DDevice(); 
	d3dDev->SetTransform(D3DTS_WORLD, &WorldMat);
	
	//setup blend modes based on the flags
	d3dDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	if(spr.mFlags&RenderFlags::MODULATEX2)
		d3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	else if(spr.mFlags&RenderFlags::MODULATEX4)
		d3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
	else
		d3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3dDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	d3dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//screen blend
	d3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, spr.mFlags&RenderFlags::ALPHA_BLEND_ENABLE);
	if(spr.mFlags&RenderFlags::ALPHA_BLEND_ADDITIVE)
	{
		d3dDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
		d3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		d3dDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		d3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	//z writes
	if(spr.mFlags&RenderFlags::NO_Z_BUFFER)
		d3dDev->SetRenderState(D3DRS_ZENABLE, false);
	else
		d3dDev->SetRenderState(D3DRS_ZENABLE, true);
	//mip mapping
	d3dDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	

	d3dDev->SetRenderState( D3DRS_AMBIENT, spr.mColour );

	SpriteData &data=*mChunks.at(spr.mSprHdl);

	// Select the vertex buffer format and vertex buffer containing the data
	d3dDev->SetFVF(D3DFVF_CUSTOMVERTEX );
	d3dDev->SetStreamSource(0, data.mpVerts, 0, sizeof(CUSTOMVERTEX));

	// Draw required number of triangles
	d3dDev->SetTexture(0, data.mpTexture); 
	if(spr.mpAnim)
		d3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, spr.mpAnim->mCurrFrame*2*3, 2);
	else
		d3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	if(spr.mFlags&RenderFlags::NO_Z_BUFFER)
		d3dDev->SetRenderState(D3DRS_ZENABLE, true);
}

unsigned int SpriteManager::Load( const std::string &texture, Sprite &spr )
{
	//we only load each texture once, even though many objects may ask for it
	for(Chunks::iterator it=mChunks.begin();it!=mChunks.end();++it)
	{
		//do we have an existing match?
		if((*it)->mTexName!=texture)
			continue;
		
		//a simple sprite we loaded previously so just return it
		if(!spr.mpAnim)
			return (*it)->mHdl;
		//an animated sprite, but the animation is the same as a previous one so just return it
		if(spr.mpAnim->mAnimFrames==(*(*it)->mpAnimFrames))
			return (*it)->mHdl;
	}

	for(Chunks::iterator it=mChunks.begin();it!=mChunks.end();++it)
	{
		//this slot is busy so skip it
		if((*it)->mpTexture)
			continue;

		//found an empty slot, try to load the texture
		if(FAILED(D3DXCreateTextureFromFile(GetD3DManager()->GetD3DDevice(), texture.c_str(), &(*it)->mpTexture)))
			MPOD_ASSERT_MSG(false, "%s texture not loaded", texture.c_str());

		(*it)->mTexName=texture;
		if(spr.mpAnim)
		{
			//it's an animation so we need a vertex buffer
			(*it)->mOwnsBuffer=true;
			CreateVB((*it)->mpVerts, *spr.mpAnim);
			(*it)->mpAnimFrames=new SpriteAnim::AnimFrames;
			*(*it)->mpAnimFrames=spr.mpAnim->mAnimFrames;
		}
		else
		{
			(*it)->mpVerts=mpVerts;
			(*it)->mOwnsBuffer=false;
		}
		return (*it)->mHdl;
	}
	MPOD_ASSERT_MSG(false,"No room");
	return 0;
}

//***********************************************************************************
void Sprite::Render()
{
	GetSpriteManager()->Render(*this);
}

void Sprite::Load( const std::string &fileName )
{
	mSprHdl=GetSpriteManager()->Load(fileName,*this);
}

Sprite::Sprite() : mSprHdl(UDEF::MAX_UINT), mPos(0,0), mWorldSize(1,1), mCentred(true), 
	mZ(0), mColour(1,1,1,1), mFlags(0), mDegrees(0), mpAnim(NULL)
{

}

Sprite::~Sprite()
{
	if(mpAnim)
		delete mpAnim;
	mpAnim=NULL;
}

void Sprite::SetAnim( float frameRate, bool loop, bool play, int numPlays )
{
	MPOD_ASSERT(mpAnim); 
	mpAnim->mFrameRate=frameRate;
	mpAnim->mLoop=loop;
	mpAnim->mPlaying=play;
	mpAnim->mNumPlays=numPlays;
}

void Sprite::ReserveFrames( int nFrames )
{
	CheckAnim(); mpAnim->ReserveFrames(nFrames);
}

void Sprite::Play()
{
	MPOD_ASSERT(mpAnim); mpAnim->Play();
}

void Sprite::Stop()
{
	MPOD_ASSERT(mpAnim); mpAnim->Stop();
}

const UVPair & Sprite::GetTexCoords() const
{
	MPOD_ASSERT(mpAnim); 
	return mpAnim->GetTexCoords();
}

void Sprite::AddFrames( UVPair &uvs )
{
	CheckAnim(); mpAnim->AddFrames(uvs);
}

void Sprite::AddFrames( float u0, float v0, float u1, float v1 )
{
	CheckAnim(); mpAnim->AddFrames(u0,v0,u1,v1);
}

void Sprite::AddMultipleFrames( int nFrames, float tw, float th )
{
	CheckAnim(); mpAnim->AddMultipleFrames(nFrames,tw,th);
}

bool Sprite::GetIsPlaying() const
{
	MPOD_ASSERT(mpAnim); return mpAnim->mPlaying;
}

int Sprite::GetNumPlays() const
{
	MPOD_ASSERT(mpAnim); return mpAnim->mNumPlays;
}

void Sprite::SetNumPlays( int num )
{
	MPOD_ASSERT(mpAnim); mpAnim->mNumPlays=num;
}

float Sprite::GetFrameRate() const
{
	MPOD_ASSERT(mpAnim); return mpAnim->mFrameRate;
}

int Sprite::GetNumFrames() const
{
	MPOD_ASSERT(mpAnim); return (int)mpAnim->mAnimFrames.size();
}

void Sprite::CheckAnim()
{
	if(mpAnim)
		return;
	mpAnim=new SpriteAnim;
}

void Sprite::Update( float aTime )
{
	if(mpAnim)
		mpAnim->Update(aTime);
}



//***********************************************************************************
void SpriteAnim::AddMultipleFrames( int nFrames, float tw, float th )
{
	//sometimes when packing sprites onto a texture, the last
	//sprite on the right edge or along the bottom doesn't quite
	//fit i.e. the artist squeezed it into a power of 2 size 
	//so make sure we get the right hand and bottom set of sprites
	//even if they don't fit by a few texels - blame the artist!
	int nCol = (int)ceilf(1.f/tw);		
	int nRow = (int)ceilf((float)nFrames/nCol);
	int count(0);
	mAnimFrames.reserve(nFrames);
	for( int y(0); y<nRow; ++y)
	{
		for(int x(0); x<nCol; ++x)
		{
			AddFrames(tw*x, th*y, tw*x+tw, th*y+th);
			++count;
			if(count>nFrames)
				break;
		}
		if(count>nFrames)
			break;
	}
}

void SpriteAnim::Update( float aTime )
{
	if(!mPlaying)
		return;
	mLastFTime+=aTime;
	if(mLastFTime<mFrameRate)
		return;
	mLastFTime=0;
	++mCurrFrame;
	if(mCurrFrame>=(int)mAnimFrames.size())
	{
		++mNumPlays;
		mCurrFrame=0;
		if(!mLoop)
			mPlaying=false;
	}
}

void SpriteAnim::AddFrames( float u0, float v0, float u1, float v1 )
{
	if(u1>1) u1=1;
	if(v1>1) v1=1;
	MPOD_ASSERT_MSG(u0<u1 && v0<v1, "Bad uv coords");
	mAnimFrames.push_back(UVPair(u0,v0,u1,v1));
}



