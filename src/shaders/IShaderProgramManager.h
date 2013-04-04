/*
 * IShaderProgramManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISHADERPROGRAMMANAGER_H_
#define ISHADERPROGRAMMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include "IShaderProgram.h"

namespace glr {
namespace shaders {
class IShaderProgramManager {
public:
virtual ~IShaderProgramManager()
{
}
;

virtual IShaderProgram* getShaderProgram(const std::string filename) = 0;

virtual void loadShaderPrograms(const std::string directory) = 0;
};
}
}

#endif /* ISHADERPROGRAMMANAGER_H_ */
