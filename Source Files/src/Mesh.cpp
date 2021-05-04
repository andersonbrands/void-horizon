// by Mark Featherstone (C) 2013 All Rights Reserved.
#include "Mesh.h"
#include "Utils.h"

using namespace std;

namespace MSH
{


//a mesh can have meterials and textures too
void Mesh::Release()
{
	if (mMeshMaterials != NULL) 
	{
		delete[] mMeshMaterials;
		mMeshMaterials=nullptr;
	}

	if (mMeshTextures)
	{
		for (DWORD i(0); i < mdwNumMaterials; ++i)
		{
			if(mMeshTextures[i]) 
				mMeshTextures[i] -> Release();
		}
		delete[] mMeshTextures;
		mMeshTextures=nullptr;
	}
	if (mpMesh != NULL) 
	{
		mpMesh -> Release();
		mpMesh = nullptr;
	}
}



// Load a mesh into this object.
void Mesh::InitialiseMesh(LPDIRECT3DDEVICE9 device, const string &path, const string &filename)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX((path+filename).c_str(), D3DXMESH_MANAGED,
		device,
		NULL, 
		&pD3DXMtrlBuffer,
		NULL,
		&mdwNumMaterials, 
		&mpMesh)))
	{
		MessageBox(NULL, "Cannot find X file", "Load Mesh", MB_OK);
		return;
	}

	// Extract the material properties and texture names from the pD3DXMtrlBuffer
	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer -> GetBufferPointer();
	// Create new textures and materials in memory.
	mMeshMaterials = new D3DMATERIAL9[mdwNumMaterials];
	mMeshTextures  = new LPDIRECT3DTEXTURE9[mdwNumMaterials];

	for (DWORD i(0); i < mdwNumMaterials; ++i)
	{
		// Copy the material
		mMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		mMeshMaterials[i].Ambient = mMeshMaterials[i].Diffuse;

		mMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
		{
			// Create the texture
			string str(path);
			string fName(d3dxMaterials[i].pTextureFilename);
			size_t marker = fName.find_last_of("/");
			if(marker==string::npos)
				marker = fName.find_last_of("\\");
			if(marker!=string::npos)
			{
				++marker;
				fName = fName.substr(marker,fName.length()-marker);
			}
			str+=fName;
			if (FAILED(D3DXCreateTextureFromFile(device, str.c_str(), &mMeshTextures[i])))
			{
				MessageBox(NULL, "Cannot find texture map file", "Load Mesh", MB_OK);
				return;
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer -> Release();

}


// Draw the mesh in sections - according to the different textures and materials.
void Mesh::RenderMesh(LPDIRECT3DDEVICE9 device, D3DMATERIAL9 mats[], LPDIRECT3DTEXTURE9 texs[]) const
{		
	
	for (DWORD i(0); i < mdwNumMaterials; ++i)
	{
		// Select a material and texture.
		if(mats)
			device->SetMaterial(&(mats[i]));
		else
			device->SetMaterial(&(mMeshMaterials[i]));
		if(texs)
			device->SetTexture(0, texs[i]);
		else
			device->SetTexture(0, mMeshTextures[i]);
		// Render the section that uses this material and texture.
		mpMesh->DrawSubset(i);
	}
}



void MSH::RenderMesh( LPDIRECT3DDEVICE9 device, Mesh &mesh, 
					 D3DMATERIAL9 mats[], LPDIRECT3DTEXTURE9 texs[],
					 float x, float y, float z, /*position */ 
					 float scaleX/*=1.f*/, float scaleY/*=1.f*/, float scaleZ/*=1.f*/, /*scale to apply */ 
					 float rotX/*=0*/, float rotY/*=0*/, float rotZ/*=0 */ )
{
	SetTransform(device,x,y,z, scaleX,scaleY,scaleZ, rotX,rotY,rotZ);
	mesh.RenderMesh(device, mats, texs);
}



}
