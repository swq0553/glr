#ifndef IMODELMANAGER_H_
#define IMODELMANAGER_H_

#include <string>

#include "Id.hpp"

#include "serialize/ITextSerializable.hpp"

namespace glr
{
namespace models
{

class IModel;

/**
 * 
 */
class IModelManager : public virtual serialize::ITextSerializable
{
public:
	virtual ~IModelManager()
	{
	}
	;

	/**
	 * Returns a pointer to an IModel object.  This object is the 'template' that is used when creating
	 * instances of this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param id The id of the IModel template you want to get.
	 * 
	 * @return The raw pointer to an IModel object.  The caller does not own the pointer - it is managed by
	 * the ModelManager.
	 */
	virtual IModel* getModelTemplate(Id id) const = 0;
	
	/**
	 * Returns a pointer to an IModel object.  This object is the 'template' that is used when creating
	 * instances of this model.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param name The name of the IModel template you want to get.
	 * 
	 * @return The raw pointer to an IModel object.  The caller does not own the pointer - it is managed by
	 * the ModelManager.
	 */
	virtual IModel* getModelTemplate(const std::string& name) const = 0;
	
	/**
	 * Loads the model from the given filename, and gives it an alias 'name'.
	 * 
	 * When you want to get this loaded model, you will need to refer to it by this name.
	 * 
	 * **Note**: If initialize is true, this method is *not* thread safe (i.e. it should only be called from the OpenGL thread).  If it is
	 * false, then this method *is* thread safe (i.e. it is safe to call outside the OpenGL thread).
	 * 
	 * @param name
	 * @param filename
	 * @param initialize If true, will initialize all of the resources required for this animation.  Otherwise, it will
	 * just create the animation and return it (without initializing it).
	 */
	virtual void loadModel(const std::string& name, const std::string& filename, bool initialize = true) = 0;
	
	/**
	 * Destroys the template of the IModel object with id 'id'.  Any pointers or references to this model will
	 * become defunct.
	 * 
	 * Note: Any instances of this model will still be valid.
	 * 
	 * If no IModel template is found with id 'id', this method does nothing.
	 * 
	 * @param id The id of the IModel instance you want to destroy.
	 */
	virtual void destroyModelTemplate(Id id) = 0;
	
	/**
	 * Destroys the template of the IModel object with name 'name'.  Any pointers or references to this model will
	 * become defunct.
	 * 
	 * Note: Any instances of this model will still be valid.
	 * 
	 * If no IModel template is found with name 'name', this method does nothing.
	 * 
	 * @param name The name of the IModel instance you want to destroy.
	 */
	virtual void destroyModelTemplate(const std::string& name) = 0;
	
	/**
	 * Destroys the template of the IModel object 'model'.  Any pointers or references to this model will
	 * become defunct.
	 * 
	 * Note: Any instances of this model will still be valid.
	 * 
	 *  If no IModel template is found that points to the IModel passed in, this method does nothing.
	 * 
	 * @param model A pointer to the IModel template you want to destroy.
	 */
	virtual void destroyModelTemplate(IModel* model) = 0;
	
	/**
	 * Creates an instance of the model with name 'name'.  This model can have animations and animation
	 * times set distinctly from other models created with this method.
	 * 
	 * @param name The name of the IModel template you want to create an instance of.
	 * 
	 * @return A pointer to a model object.  The model is a copy of the model that was previously loaded
	 * with the specified name.
	 */
	virtual IModel* createInstance(const std::string& name) = 0;
	
	/**
	 * Destroys the instance of the model with id 'id'.  Any pointers or references to this model will
	 * become defunct.
	 * 
	 * If no IModel instance with id 'id' is found, this method does nothing.
	 * 
	 * @param id The id of the IModel instance you want to destroy.
	 */
	virtual void destroyInstance(Id id) = 0;
	
	/**
	 * Destroys the instance of the IModel 'model'.  Any pointers or references to this model will
	 * become defunct.
	 * 
	 * If no IModel instance is found that points to the IModel passed in, this method does nothing.
	 * 
	 * @param model A pointer to the IModel instance you want to destroy.
	 */
	virtual void destroyInstance(IModel* model) = 0;
	
	/**
	 * Gets the instance of the model with id 'id'.
	 * 
	 * If no IModel instance with id 'id' is found, this method returns nullptr.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param id The id of the IModel instance you want to get.
	 * 
	 * @return A pointer to the IModel instance, or nullptr if the model was not found.
	 */
	virtual IModel* getInstance(Id id) const = 0;
};

}
}

#endif /* IMODELMANAGER_H_ */
