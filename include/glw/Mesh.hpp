#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <atomic>

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IMesh.hpp"

#include "shaders/IShaderProgram.hpp"

namespace glr
{
namespace glw
{
	
class IOpenGlDevice;

/**
 * A class that contains mesh data, and can load this data into OpenGL, as well as render that data
 * once it has been transferred.
 */
class Mesh : public IMesh
{
public:
	/**
	 * Basic constructor.  The creator of the Mesh will have to set the data, and then call
	 * 'allocateVideoMemory()' and 'pushToVideoMemory()' before this mesh will render properly.
	 */
	Mesh(IOpenGlDevice* openGlDevice, std::string name);
	
	/**
	 * Standard constructor.  
	 * 
	 * @param initialize If true, will initialize all of the resources required for this mesh.  Otherwise, it will
	 * just create the mesh and return it (without initializing it).
	 */
	Mesh(IOpenGlDevice* openGlDevice,
		std::string name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > vertexBoneData,
		BoneData boneData,
		bool initialize = true
	);
	
	/**
	 * Standard constructor.
	 * 
	 * This type of Mesh does not have any bone data set by default.
	 * 
	 * @param initialize If true, will initialize all of the resources required for this mesh.  Otherwise, it will
	 * just create the mesh and return it (without initializing it).
	 */
	Mesh(IOpenGlDevice* openGlDevice,
		std::string name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		bool initialize = true
	);
	virtual ~Mesh();

	virtual void render();
	
	virtual BoneData& getBoneData();
	
	virtual void allocateVideoMemory();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeVideoMemory();
	virtual bool isVideoMemoryAllocated() const;
	virtual void loadLocalData();
	virtual void freeLocalData();
	virtual bool isLocalDataLoaded() const;
	virtual bool isDirty() const;
	
	virtual const std::string& getName() const;
	void setName(std::string name);
	
	void setVertices(std::vector< glm::vec3 > vertices);
	void setNormals(std::vector< glm::vec3 > normals);
	void setTextureCoordinates(std::vector< glm::vec2 > textureCoordinates);
	void setColors(std::vector< glm::vec4 > colors);
	void setVertexBoneData(std::vector< VertexBoneData > vertexBoneData);
	
	std::vector< glm::vec3 >& getVertices();
	std::vector< glm::vec3 >& getNormals();
	std::vector< glm::vec2 >& getTextureCoordinates();
	std::vector< glm::vec4 >& getColors();
	std::vector< VertexBoneData >& getVertexBoneData();
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);
	
protected:
	IOpenGlDevice* openGlDevice_;
	std::string name_;
	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	std::vector< VertexBoneData > vertexBoneData_;

	BoneData boneData_;

	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboIds_[5];
	
	std::atomic<bool> isLocalDataLoaded_;
	std::atomic<bool> isVideoMemoryAllocated_;
	std::atomic<bool> isDirty_;
	
	glm::detail::uint32 currentNumberOfVertices_ = 0;
	glm::detail::uint32 currentVerticesSpaceAllocated_ = 0;
	
	std::string textureFileName_;

private:
	/**
	 * Required by serialization.
	 */
	Mesh();

	friend class boost::serialization::access;
	
	template<class Archive> void inline serialize(Archive& ar, const unsigned int version);
};

}
}

#include "Mesh.inl"

#endif /* MESH_H_ */
