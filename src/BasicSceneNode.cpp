/*
 * BasicSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>

#include <glm/gtc/matrix_transform.hpp>

#include "common/logger/Logger.h"

#include "BasicSceneNode.h"

#include "glw/shaders/GlslShaderProgram.h"


namespace glr {

BasicSceneNode::BasicSceneNode(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(const std::string name, glw::IOpenGlDevice* openGlDevice) : name_(name), openGlDevice_(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(const std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice)
	 : name_(name), openGlDevice_(openGlDevice)
{
	setPosition(position);
	rotate(orientation);
	setScale(scale);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::~BasicSceneNode()
{
}

void BasicSceneNode::attach(models::IModel* model)
{
	model_ = model;
}

void BasicSceneNode::detach(models::IModel* model)
{
	// TODO: implement
}

void BasicSceneNode::attach(shaders::IShaderProgram* shaderProgram)
{
	shaderProgram_ = shaderProgram;
}

/*
ISceneNode* BasicSceneNode::createChild(const std::string name, glm::vec3& position, glm::vec3& lookAt)
{
	ISceneNode* node = sceneManager_->createSceneNode(name);

	node->setPosition(position);
	node->setLookAt(lookAt);

	if ( node != nullptr )
		children_[node->getName()] = node;

	return node;
}

void BasicSceneNode::addChild(ISceneNode* node)
{
	children_[node->getName()] = node;
}

ISceneNode* BasicSceneNode::getChild(const std::string& name)
{
	if ( children_.find(name) != children_.end())
		return children_[name];

	return nullptr;
}

void BasicSceneNode::removeChild(ISceneNode* node)
{
	if ( children_.find(node->getName()) != children_.end())
		children_.erase(node->getName());
}

void BasicSceneNode::removeAllChildren()
{
	children_.clear();
}

glmd::uint32 BasicSceneNode::getNumChildren()
{
	return children_.size();
}
*/

std::string BasicSceneNode::getName()
{
	return name_;
}

glm::vec3& BasicSceneNode::getPosition()
{
	return pos_;
}

void BasicSceneNode::setPosition(glm::vec3& newPos)
{
	pos_ = newPos;
}

void BasicSceneNode::setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	pos_ = glm::vec3(x, y, z);
}

glm::vec3& BasicSceneNode::getScale()
{
	return scale_;
}

void BasicSceneNode::setScale(const glm::vec3& scale)
{
	scale_ = scale;
}

void BasicSceneNode::setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	scale_ = glm::vec3(x, y, z);
}

void BasicSceneNode::translate(const glm::vec3& trans, TransformSpace relativeTo)
{
	pos_ += trans;
}

void BasicSceneNode::translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo)
{
	pos_ += glm::vec3(x, y, z);
}

const glm::quat& BasicSceneNode::getOrientation()
{
	return orientationQuaternion_;
}

void BasicSceneNode::rotate(const glm::quat& quaternion, TransformSpace relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			orientationQuaternion_ = orientationQuaternion_ * glm::normalize( quaternion );
			break;
		
		case TransformSpace::TS_WORLD:
			orientationQuaternion_ =  glm::normalize( quaternion ) * orientationQuaternion_;
			break;
			
		default:
			// TODO: error
			break;
	}		
}

void BasicSceneNode::rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			orientationQuaternion_ = glm::normalize( glm::angleAxis(degrees, axis) ) * orientationQuaternion_;
			break;
		
		case TransformSpace::TS_WORLD:
			orientationQuaternion_ =  orientationQuaternion_ * glm::normalize( glm::angleAxis(degrees, axis) );
			break;
			
		default:
			// TODO: error
			break;
	}
}

void BasicSceneNode::lookAt(const glm::vec3& lookAt)
{
	assert(lookAt != pos_);
	
	glm::mat4 lookAtMatrix = glm::lookAt(pos_, pos_ + lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	orientationQuaternion_ =  glm::normalize( orientationQuaternion_ * glm::quat_cast( lookAtMatrix ) );
}

models::IModel* BasicSceneNode::getModel()
{
	return model_;
}

shaders::IShaderProgram* BasicSceneNode::getShaderProgram()
{
	return shaderProgram_;
}

void BasicSceneNode::render()
{
	if ( model_ != nullptr )
	{
		if (shaderProgram_ != nullptr) 
		{
			shaderProgram_->bind();
			
			int modelMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "modelMatrix");
			int pvmMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "pvmMatrix");
			int normalMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "normalMatrix");
		
			const glm::mat4 modelMatrix = openGlDevice_->getModelMatrix();
			const glm::mat4 projectionMatrix = openGlDevice_->getProjectionMatrix();
			const glm::mat4 viewMatrix = openGlDevice_->getViewMatrix();
			
			glm::mat4 newModel = glm::translate(modelMatrix, pos_);
			
			newModel = newModel * glm::mat4_cast( orientationQuaternion_ );
			
			//newModel = glm::rotate(newModel, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
			//newModel = glm::rotate(newModel, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
			//newModel = glm::rotate(newModel, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
			newModel = glm::scale(newModel, scale_);
			
			
			// Send uniform variable values to the shader		
			glm::mat4 pvmMatrix(projectionMatrix * viewMatrix * newModel);
			glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
		
			glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(viewMatrix * newModel)));
			glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
		
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &newModel[0][0]);
		}
		
		//if (shaderProgram_ != nullptr)
		//	shaderProgram_->bind();
		//else
		//	shaders::ShaderProgram::unbindAll();
		model_->render(shaderProgram_);
	}
}

}
