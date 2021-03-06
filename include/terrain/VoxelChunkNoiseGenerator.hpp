#ifndef VOXELCHUNKNOISEGENERATOR_H_
#define VOXELCHUNKNOISEGENERATOR_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace glr
{
namespace terrain
{

class IFieldFunction;
class VoxelChunk;

/**
 * Will fill the VoxelChunk with noise generated using the field function.
 * 
 * Once the method is finished, the VoxelChunk will have the following dimensions:
 * (SIZE + POINT_FIELD_OFFSET + POINT_FIELD_OVERSET) x (SIZE + POINT_FIELD_OFFSET + POINT_FIELD_OVERSET)
 * 
 * We generate 'extra' dimensions of density data for the benefit of the smoothing functions - it allows them to use the extra data
 * for interpolation, etc.
 * 
 */
void generateNoise(VoxelChunk& chunk, glm::detail::int32 length, glm::detail::int32 width, glm::detail::int32 height, glr::terrain::IFieldFunction& fieldFunction);

/**
 * 
 */
bool determineIfEmptyOrSolid(VoxelChunk& chunk);

}
}

#endif /* VOXELCHUNKNOISEGENERATOR_H_ */
