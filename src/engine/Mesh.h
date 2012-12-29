/*
 * Mesh.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

//#include "../../vmath/Vector3f.h"


namespace oglre {

namespace engine {

class Mesh {
public:
	Mesh(const aiMesh* mesh);
	virtual ~Mesh();
	
	void render();

private:
	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	
	std::string textureFileName_;

};

}

}

#endif /* MESH_H_ */
