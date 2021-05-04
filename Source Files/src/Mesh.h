// by Mark Featherstone (C) 2013 All Rights Reserved.
#ifndef MESH_H
#define MESH_H

#include <d3dx9.h>
#include <string>
#include <vector>

namespace MSH
{



//handle loading and rendering of .x meshes
class Mesh
{
public:
	Mesh() : mpMesh(0) {};
	//we cna let the mesh clear up after itself or force it to do it
	~Mesh() {
		Release();
	}
	void Release();
	//load in and setup a new mesh
	void InitialiseMesh(LPDIRECT3DDEVICE9 device, const std::string &path, const std::string &filename);
	//render it with the provided material
	void RenderMesh(LPDIRECT3DDEVICE9 device, D3DMATERIAL9 mats[]=nullptr, LPDIRECT3DTEXTURE9 texs[]=nullptr) const;
	
	//accessors
	LPD3DXMESH GetMesh() const { return mpMesh; }
	void SetMesh(LPD3DXMESH val) { mpMesh = val; }
	DWORD GetNumMaterials() const { return mdwNumMaterials; }

private:
	LPD3DXMESH            mpMesh;						 // The mesh object
	D3DMATERIAL9		* mMeshMaterials;			 // Materials for the mesh
	LPDIRECT3DTEXTURE9  * mMeshTextures;				 // Textures for the mesh
	DWORD                 mdwNumMaterials;			 // Number of materials in the mesh
};

void RenderMesh(LPDIRECT3DDEVICE9 device, Mesh &mesh,
				D3DMATERIAL9 mats[], LPDIRECT3DTEXTURE9 texs[],
				float x, float y, float z, /*position */ 
				float scaleX=1.f, float scaleY=1.f, float scaleZ=1.f,  /*scale to apply */
				float rotX=0, float rotY=0, float rotZ=0 ); /*rotation to apply */


}

#endif