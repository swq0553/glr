#include <iostream>
#include <algorithm>
#include <mutex>
#include <stdexcept>

#include <cef_app.h>

#include "Macros.h"

class DuplicateFunction : public std::runtime_error {
public:
	DuplicateFunction(std::wstring funcName) : std::runtime_error("Function with name already exists.")
	{
	}
};

class DuplicateAttribute : public std::runtime_error {
public:
	DuplicateAttribute(std::wstring attrName) : std::runtime_error("Attribute with name already exists.")
	{
	}
};

class FunctionBinding
{
public:
	FunctionBinding(std::wstring name) : name_(name)
	{
	}

	GETSET(std::wstring, name_, Name)

private:
	std::wstring name_;
};


class ObjectBinding;
class AttributeBinding
{
public:
	AttributeBinding(std::wstring name) : name_(name)
	{
	}
	
	AttributeBinding(std::wstring name, std::wstring valueWstring) : name_(name), valueWstring_(valueWstring), isWstring_(true)
	{
	}
	
	AttributeBinding(std::wstring name, std::string valueString) : name_(name), valueString_(valueString), isString_(true)
	{
	}
	
	AttributeBinding(std::wstring name, int valueInt) : name_(name), valueInt_(valueInt), isInt_(true)
	{
	}
	
	AttributeBinding(std::wstring name, uint valueUint) : name_(name), valueUint_(valueUint), isUint_(true)
	{
	}
	
	AttributeBinding(std::wstring name, bool valueBool) : name_(name), valueBool_(valueBool), isBool_(true)
	{
	}
	
	AttributeBinding(std::wstring name, float valueFloat) : name_(name), valueFloat_(valueFloat), isFloat_(true)
	{
	}
	
	AttributeBinding(std::wstring name, double valueDouble) : name_(name), valueDouble_(valueDouble), isDouble_(true)
	{
	}
	
	AttributeBinding(std::wstring name, ObjectBinding* valueObject) : name_(name), valueObject_(valueObject), isObject_(true)
	{
	}
	
	AttributeBinding(std::wstring name, CefRefPtr<CefV8Value> valueV8) : name_(name), valueV8_(valueV8), isV8_(true)
	{
	}

	GETSET(std::wstring, name_, Name)
	
	GETSET(std::wstring, valueWstring_, ValueWstring)
	GETSET(std::string, valueString_, ValueString)
	GETSET(int, valueInt_, ValueInt)
	GETSET(uint, valueUint_, ValueUint)
	GETSET(bool, valueBool_, ValueBool)
	GETSET(float, valueFloat_, ValueFloat)
	GETSET(double, valueDouble_, ValueDouble)
	GETSET(ObjectBinding*, valueObject_, ValueObject)
	GETSET(CefRefPtr<CefV8Value>, valueV8_, ValueV8)
	
	GETSET(bool, isWstring_, IsWstring)
	GETSET(bool, isString_, IsString)
	GETSET(bool, isInt_, IsInt)
	GETSET(bool, isUint_, IsUint)
	GETSET(bool, isBool_, IsBool)
	GETSET(bool, isFloat_, IsFloat)
	GETSET(bool, isDouble_, IsDouble)
	GETSET(bool, isObject_, IsObject)
	GETSET(bool, isV8_, IsV8)

private:
	std::wstring name_;
	
	std::wstring valueWstring_;
	std::string valueString_;
	int valueInt_;
	uint valueUint_;
	bool valueBool_;
	float valueFloat_;
	double valueDouble_;
	ObjectBinding* valueObject_;
	CefRefPtr<CefV8Value> valueV8_;
	
	bool isWstring_;
	bool isString_;
	bool isInt_;
	bool isUint_;
	bool isBool_;
	bool isFloat_;
	bool isDouble_;
	bool isObject_;
	bool isV8_;
};

class ObjectBinding : public CefV8Handler
{
public:
	ObjectBinding()
	{
	}

	ObjectBinding(std::wstring name) : name_(name)
	{
	}
	
	/*
	 * When your javascript calls a method that has been defined as a native api call, it will call this method.
	 * 
	 * This method will encode the function call, and send it to the browser process over IPC.
	 * 
	 * The format that is sent via IPC looks like this:
	 * <function_name> <number_of_arguments> [<argument>, ..]
	 * 
	 * The browser process will process the function call, execute the requested function (if it exists), and send the result via IPC to this
	 * process.
	 */
	bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
	{		
		std::wstring s = name.ToWString();
		
		// Implementation for “setMessageCallback”
		if ( hasFunction(s) )
		{
			std::wcout << "cef3_client Execute " << name_ << "." << s << std::endl;
			
			// Send function call to browser process
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ExecuteFunction");
			
			message->GetArgumentList()->SetString( 0, name_ );
			message->GetArgumentList()->SetString( 1, s );
			message->GetArgumentList()->SetInt( 2, arguments.size() );
			
			// Encode function parameters
			for ( int i = 0; i < arguments.size(); i++ )
			{
				if ( arguments[i]->IsString() )
					message->GetArgumentList()->SetString( i+2, arguments[i]->GetStringValue() );
				else if ( arguments[i]->IsInt() )
					message->GetArgumentList()->SetInt( i+2, arguments[i]->GetIntValue() );
				else if ( arguments[i]->IsUInt() )
					message->GetArgumentList()->SetInt( i+2, arguments[i]->GetUIntValue() );
				else if ( arguments[i]->IsBool() )
					message->GetArgumentList()->SetBool( i+2, arguments[i]->GetBoolValue() );
				else if ( arguments[i]->IsDouble() )
					message->GetArgumentList()->SetDouble( i+2, arguments[i]->GetDoubleValue() );
			}
			
			CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
			context->GetBrowser()->SendProcessMessage(PID_BROWSER, message);
			
			return true;
		}
		else
		{
			// TODO: error
			std::wcout << "CEF Object " << name_ << " has no method '" << s << "'" << std::endl;
		}
		
		// Function does not exist.
		return false;
	}

	bool hasFunction(std::wstring funcName)
	{
		for ( auto& f : functions_ )
		{
			if (f.getName() == funcName)
				return true;
		}
		
		return false;
	}
	
	bool hasAttribute(std::wstring attrName)
	{
		for ( auto& a : attributes_ )
		{
			if (a.getName() == attrName)
				return true;
		}
		
		return false;
	}
	
	void addFunction(FunctionBinding function)
	{
		if ( hasFunction(function.getName()) )
			throw DuplicateFunction( function.getName() );
		
		functions_.push_back( function );
	}
	
	void addAttribute(AttributeBinding attribute)
	{
		if ( hasAttribute(attribute.getName()) )
			throw DuplicateAttribute( attribute.getName() );
		
		attributes_.push_back( attribute );
	}
	
	const std::vector< FunctionBinding > getFunctions()
	{
		return functions_;
	}
	
	const std::vector< AttributeBinding > getAttributes()
	{
		return attributes_;
	}
	
	GETSET(std::wstring, name_, Name)
	
private:
	std::wstring name_;
	std::vector< FunctionBinding > functions_;
	std::vector< AttributeBinding > attributes_;

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING( ObjectBinding )
};

class ClientApp : public CefApp, public CefRenderProcessHandler, public CefV8Handler
{
public:
    ClientApp()
    {
		std::cout << "cef3_client here woo" << std::endl;
		totalBindingsSent_ = 0;
		totalBindingsReceived_ = 0;
		allBindingsSentMessageReceived_ = false;
		readyForBindingsMessageSent_ = false;
	};
	
	
	
	/** HELPER FUNCTIONS **/
	void copyToList(CefRefPtr<CefV8Value> value, int i, CefRefPtr<CefListValue> list)
	{
		SetListValue(list, i, value);
	}
	
	// Transfer a V8 array to a List.
	void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {
		//assert(source->IsArray());
	
		int argLength = source->GetArrayLength();
		if (argLength == 0)
			return;
		
		// Start with null types in all spaces.
		target->SetSize(argLength);
		
		for (int i = 0; i < argLength; ++i)
			SetListValue(target, i, source->GetValue(i));
	}
	
	// Transfer a V8 value to a List index.
	void SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value) {
		if (value->IsArray()) {
			CefRefPtr<CefListValue> new_list = CefListValue::Create();
			SetList(value, new_list);
			list->SetList(index, new_list);
		}
		else if (value->IsString())
		{
			list->SetString(index, value->GetStringValue());
		}
		else if (value->IsBool())
		{
			list->SetBool(index, value->GetBoolValue());
		}
		else if (value->IsInt())
		{
			list->SetInt(index, value->GetIntValue());
		}
		else if (value->IsDouble())
		{
			list->SetDouble(index, value->GetDoubleValue());
		}
	}
	
	// Transfer a List value to a V8 array index.
	void SetListValue(CefRefPtr<CefV8Value> list, int index, CefRefPtr<CefListValue> value) {
		CefRefPtr<CefV8Value> newValue;
		
		CefValueType type = value->GetType(index);
		switch (type)
		{
			case VTYPE_LIST: 
			{
				CefRefPtr<CefListValue> list = value->GetList(index);
				newValue = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
				SetList(list, newValue);
			} 
				break;
			case VTYPE_BOOL:
				newValue = CefV8Value::CreateBool(value->GetBool(index));
				break;
			case VTYPE_DOUBLE:
				newValue = CefV8Value::CreateDouble(value->GetDouble(index));
				break;
			case VTYPE_INT:
				newValue = CefV8Value::CreateInt(value->GetInt(index));
				break;
			case VTYPE_STRING:
				newValue = CefV8Value::CreateString(value->GetString(index));
				break;
			default:
				break;
		}
	
		if (newValue.get())
		{
			list->SetValue(index, newValue);
		}
		else
		{
			list->SetValue(index, CefV8Value::CreateNull());
		}
	}
	
	// Transfer a List to a V8 array.
	void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
		//ASSERT(target->IsArray());
		
		int argLength = static_cast<int>(source->GetSize());
		if (argLength == 0)
			return;
		
		for (int i = 0; i < argLength; ++i)
			SetListValue(target, i, source);
	}
	
	CefRefPtr<CefV8Value> getValue(CefRefPtr<CefProcessMessage> message, uint index)
	{
		CefValueType type = message->GetArgumentList()->GetType( index );
		switch (type) {
			case VTYPE_STRING:
			{
				std::string arg = message->GetArgumentList()->GetString(index);
				std::cout << "VTYPE_STRING - getValue -  " << arg << std::endl;
				return CefV8Value::CreateString( arg );
			}
				break;
			
			case VTYPE_INT:
			{
				int arg = message->GetArgumentList()->GetInt(index);
				std::cout << "VTYPE_INT - getValue -  " << arg << std::endl;
				return CefV8Value::CreateInt( arg );
			}
				break;
			
			case VTYPE_DOUBLE:
			{
				double arg = message->GetArgumentList()->GetDouble(index);
				std::cout << "VTYPE_DOUBLE - getValue -  " << arg << std::endl;
				return CefV8Value::CreateDouble( arg );
			}
				break;
			
			case VTYPE_BOOL:
			{
				bool arg = message->GetArgumentList()->GetBool(index);
				std::cout << "VTYPE_BOOL - getValue -  " << arg << std::endl;
				return CefV8Value::CreateBool( arg );
			}
				break;
		
			case VTYPE_BINARY:
				// TODO: error
				std::cout << "Error - getValue - VTYPE_BINARY not implemented as CEF3 function result type." << std::endl;
				break;
				
			case VTYPE_DICTIONARY:
				// TODO: error
				std::cout << "Error - getValue - VTYPE_DICTIONARY not implemented as CEF3 function result type." << std::endl;
				break;
			
			case VTYPE_LIST:
				// TODO: error
				std::cout << "Error - getValue - VTYPE_LIST not implemented as CEF3 function result type." << std::endl;
				break;
			
			case VTYPE_INVALID:
				// TODO: error
				std::cout << "Error - getValue - VTYPE_INVALID CEF3 function result type." << std::endl;
				break;
				
			case VTYPE_NULL:
				// TODO: error
				std::cout << "Error - getValue - VTYPE_NULL not implemented as CEF3 function result type." << std::endl;
				break;
			
			default:
				std::cout << "Error - getValue - Unknown CEF3 function result type: " << type << std::endl;
				break;
		}
		
		return nullptr;
	}
	
	
	/**
	 * Implement CEF3's OnContextCreated method in order to add callable native functions to javascript.
	 */
	void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{		
		std::cout << "cef3_client OnContextCreated has been called!" << std::endl;
		
		if ( allBindingsSentMessageReceived_ )
		{			
			// Register our objects, functions and attributes
			objectBindingMapMutex_.lock();
			std::cout << "Binding " << objectBindingMap_.size() << " object(s)." << std::endl;
			for ( auto& it : objectBindingMap_ )
			{
				// Add object
				const std::wstring& objName = it.first;
				std::wcout << "Binding object " << objName << std::endl;
				CefRefPtr<CefV8Value> obj = CefRefPtr<CefV8Value>();
				if (objName.length() != 0)
					obj = CefV8Value::CreateObject(nullptr);
				
				CefRefPtr<CefV8Handler> handler = &it.second;
				
				// Add attributes
				const std::vector< AttributeBinding >& attributes = it.second.getAttributes();
				std::cout << "Binding attributes " << attributes.size() << std::endl;
				for ( auto& a : attributes )
				{
					std::wcout << L"Binding attribute " << a.getName() << std::endl;
					CefRefPtr<CefV8Value> attrValue;
					
					if ( a.getValueV8()->IsString() )
						attrValue = CefV8Value::CreateString( a.getValueV8()->GetStringValue() );
					else if ( a.getValueV8()->IsInt() )
						attrValue = CefV8Value::CreateInt( a.getValueV8()->GetIntValue() );
					else if ( a.getValueV8()->IsUInt() )
						attrValue  = CefV8Value::CreateUInt( a.getValueV8()->GetUIntValue() );
					else if ( a.getValueV8()->IsBool() )
						attrValue = CefV8Value::CreateBool( a.getValueV8()->GetBoolValue() );
					else if ( a.getValueV8()->IsDouble() )
						attrValue = CefV8Value::CreateDouble( a.getValueV8()->GetDoubleValue() );
					
					if (obj.get() != nullptr)
					{
						obj->SetValue(a.getName(), attrValue, V8_PROPERTY_ATTRIBUTE_NONE);
						context->GetGlobal()->SetValue(objName, obj, V8_PROPERTY_ATTRIBUTE_NONE);
					}
					else
					{
						context->GetGlobal()->SetValue(a.getName(), attrValue, V8_PROPERTY_ATTRIBUTE_NONE);
					}
				}
				
				// Add functions
				const std::vector< FunctionBinding >& functions = it.second.getFunctions();				
				std::cout << "Binding functions " << functions.size() << std::endl;
				for ( auto& f : functions )
				{
					std::wcout << L"Binding function " << f.getName() << std::endl;
					
					CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(f.getName(), handler);
					
					if (obj.get() != nullptr)
					{
						obj->SetValue(f.getName(), func, V8_PROPERTY_ATTRIBUTE_NONE);
						context->GetGlobal()->SetValue(objName, obj, V8_PROPERTY_ATTRIBUTE_NONE);
					}
					else
					{
						context->GetGlobal()->SetValue(f.getName(), func, V8_PROPERTY_ATTRIBUTE_NONE);
					}
				}
			}
			objectBindingMapMutex_.unlock();
			
			// Register the standard callback function
			CefRefPtr<CefV8Handler> handler = this;
			std::string callbackFunction = "setMessageCallback";
			CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(callbackFunction, handler);
			context->GetGlobal()->SetValue(callbackFunction, func, V8_PROPERTY_ATTRIBUTE_NONE);
		}
		
		if ( !readyForBindingsMessageSent_ )
		{
			// Indicate that the render process is ready to receive function binding data
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ReadyForBindings");
			//message->GetArgumentList()->SetInt(0, browser->GetIdentifier() );
			//message->GetArgumentList()->SetInt(1, frame->GetIdentifier() );
			browser->SendProcessMessage(PID_BROWSER, message);
			readyForBindingsMessageSent_ = true;
		}
	};
	
	void OnContextInitialized()
	{
		//std::cout << "cef3_client OnContextInitialized has been called!" << std::endl;
	};
	
	void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		std::cout << "cef3_client OnContextReleased has been called!" << std::endl;
	};
	
	void OnWebKitInitialized()
	{
		std::cout << "cef3_client OnWebKitInitialized has been called!" << std::endl;
	};
	
	void sendMessageAllBindingsReceived(CefRefPtr<CefBrowser> browser)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("AllBindingsReceived");
		browser->SendProcessMessage(PID_BROWSER, message);
	}
	
	enum Exception
	{
		BIND_EXCEPTION = 0,
		EXECUTE_EXCEPTION,
		MESSAGE_EXCEPTION
	};
	
	/**
	 * Exceptions available:
	 * 
	 * BIND_EXCEPTION		- when something bad happens during binding
	 * EXECUTE_EXCEPTION	- when something bad happens during execution of a bound function
	 * MESSAGE_EXCEPTION	
	 */
	void sendMessageException(CefRefPtr<CefBrowser> browser, Exception exception, std::wstring message)
	{
		CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create("Exception");
		m->GetArgumentList()->SetInt( 0, exception );
		m->GetArgumentList()->SetString( 1, message );
		browser->SendProcessMessage(PID_BROWSER, m);
	}
	
	/**
	 * 
	 * 
	 * List of available functions:
	 * 
	 * AddFunction
	 * 		funcName
	 * RemoveFunction
	 * 		funcName
	 * AddAttribute
	 * 		attrName
	 * RemoveAttribute
	 * 		attrName
	 * AddObject
	 * 		objName
	 * RemoveObject
	 * 		objName
	 * AddAttributeToObject
	 * 		objName attrName
	 * RemoveAttributeFromObject
	 * 		objName attrName
	 * AddMethodToObject
	 * 		objName methodName
	 * RemoveMethodFromObject
	 * 		objName methodName
	 * FunctionResult
	 * 		resultType resultValue
	 * AllBindingsSent
	 * 
	 */
	bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
	{
		std::string messageName = message->GetName();
		std::cout << "cef3_client OnProcessMessageReceived " << messageName.c_str() << std::endl;
		
		
		if( messageName == "AddObject"/* && browser == m_Browser*/ )
		{
			std::wstring objName = message->GetArgumentList()->GetString(0);
			std::wcout << "cef3_client AddObject: " << objName << std::endl;
			
			objectBindingMapMutex_.lock();
			ObjectBinding& obj = objectBindingMap_[ objName ];
			obj.setName( objName );
			
			totalBindingsReceived_++;
			
			if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
			{
				sendMessageAllBindingsReceived( browser );
			}
			objectBindingMapMutex_.unlock();
		}
		else if( messageName == "AddMethodToObject"/* && browser == m_Browser*/ )
		{
			std::wstring objName = message->GetArgumentList()->GetString(0);
			std::wstring funcName = message->GetArgumentList()->GetString(1);
			std::wcout << "cef3_client AddMethodToObject: " << objName << " | " << funcName << std::endl;
			
			objectBindingMapMutex_.lock();
			ObjectBinding& obj = objectBindingMap_[ objName ];
			
			if ( !obj.hasFunction( funcName ) )
			{
				FunctionBinding func = FunctionBinding(funcName);
				obj.addFunction( func );
			}
			
			totalBindingsReceived_++;
			
			if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
			{
				sendMessageAllBindingsReceived( browser );
			}
			objectBindingMapMutex_.unlock();
		}
		else if( messageName == "AddAttributeToObject"/* && browser == m_Browser*/ )
		{
			std::wstring objName = message->GetArgumentList()->GetString(0);
			std::wstring attrName = message->GetArgumentList()->GetString(1);
			CefRefPtr<CefV8Value> attrValue = getValue(message, 2);
			
			std::wcout << "cef3_client AddAttributeToObject: " << objName << " | " << attrName << " | ";
			if ( attrValue->IsString() )
				std::cout << "(string) " << attrValue->GetStringValue().ToString() << std::endl;
			else if ( attrValue->IsInt() )
				std::cout << "(int) " <<  attrValue->GetIntValue() << std::endl;
			else if ( attrValue->IsUInt() )
				std::cout << "(uint) " <<  attrValue->GetUIntValue() << std::endl;
			else if ( attrValue->IsBool() )
				std::cout << "(bool) " <<  attrValue->GetBoolValue() << std::endl;
			else if ( attrValue->IsDouble() )
				std::cout << "(double) " <<  attrValue->GetDoubleValue() << std::endl;
			else
				sendMessageException(browser, Exception::BIND_EXCEPTION, L"Attribute '" + objName + L"." + attrName + L"' has an unknown value type.");
			
			objectBindingMapMutex_.lock();
			ObjectBinding& obj = objectBindingMap_[ objName ];
			
			if ( !obj.hasAttribute( attrName ) )
			{
				AttributeBinding attr = AttributeBinding(attrName, attrValue);
				obj.addAttribute( attr );
			}
			
			totalBindingsReceived_++;
			
			if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
			{
				sendMessageAllBindingsReceived( browser );
			}
			objectBindingMapMutex_.unlock();
		}
		else if( messageName == "AddFunction"/* && browser == m_Browser*/ )
		{
			std::wstring funcName = message->GetArgumentList()->GetString(0);
			std::wcout << "cef3_client AddFunction: " << funcName << std::endl;
			
			objectBindingMapMutex_.lock();
			ObjectBinding& obj = objectBindingMap_[ L"" ];
			
			if ( !obj.hasFunction( funcName ) )
			{
				FunctionBinding func = FunctionBinding(funcName);
				obj.addFunction( func );
			}
			else
				sendMessageException(browser, Exception::BIND_EXCEPTION, L"Function '" + funcName + L"' already defined.");
			
			totalBindingsReceived_++;
			
			if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
			{
				sendMessageAllBindingsReceived( browser );
			}
			objectBindingMapMutex_.unlock();
		}
		else if( messageName == "AddAttribute"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client AddAttribute message not yet implemented." << std::endl;
		}
		else if( messageName == "RemoveFunction"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client RemoveFunction message not yet implemented." << std::endl;
		}
		else if( messageName == "RemoveAttribute"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client RemoveAttribute message not yet implemented." << std::endl;
		}
		else if( messageName == "RemoveObject"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client RemoveObject message not yet implemented." << std::endl;
		}
		else if( messageName == "RemoveAttributeFromObject"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client RemoveAttributeFromObject message not yet implemented." << std::endl;
		}
		else if( messageName == "RemoveMethodFromObject"/* && browser == m_Browser*/ )
		{
			std::cout << "cef_client RemoveMethodFromObject message not yet implemented." << std::endl;
		}
		else if ( messageName == "AllBindingsSent")
		{
			totalBindingsSent_ = message->GetArgumentList()->GetInt(0);
			allBindingsSentMessageReceived_ = true;
			
			objectBindingMapMutex_.lock();
			if (totalBindingsSent_ == totalBindingsReceived_)
			{
				sendMessageAllBindingsReceived( browser );
			}
			objectBindingMapMutex_.unlock();
			std::cout << "cef_client AllBindingsSent message processed" << std::endl;
		}
		else if( messageName == "FunctionResult" )
		{
			std::wstring funcName = message->GetArgumentList()->GetString(0);
			int numArguments = message->GetArgumentList()->GetInt(1);
			
			CefRefPtr<CefListValue> list = CefListValue::Create();
			list->SetSize( numArguments );
			
			CefRefPtr<CefV8Value> temp;
			
			for (int i = 0; i < numArguments; i++)
			{
				temp = getValue(message, i+2);
				
				if (temp != nullptr)
				{
					copyToList(temp, i, list);
					temp = nullptr;
				}
				
			}
			
			// Execute the registered JavaScript callback if any.
			if (!callbackMap_.empty())
			{
				CallbackMap::const_iterator it = callbackMap_.find( std::make_pair(funcName, browser->GetIdentifier()));
				
				if (it != callbackMap_.end())
				{
					// Keep a local reference to the objects. The callback may remove itself from the callback map.
					CefRefPtr<CefV8Context> context = it->second.first;
					CefRefPtr<CefV8Value> callback = it->second.second;
					
					// Enter the context.
					context->Enter();
					
					CefV8ValueList arguments;
					
					// First argument is the message name.
					arguments.push_back(CefV8Value::CreateString(funcName));
					
					// Second argument is the list of message arguments.
					//CefRefPtr<CefListValue> list = message->GetArgumentList();
					//std::cout << "list->GetSize() " << list->GetSize() << std::endl;
					CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(list->GetSize());
					SetList(list, args);  // Helper function to convert CefListValue to CefV8Value.
					
					arguments.push_back( args );
					
					// Execute the callback.
					CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(NULL, arguments);
					if (retval.get())
					{
						if (retval->IsBool())
						{
							bool handled = retval->GetBoolValue();
							if (!handled)
								sendMessageException(browser, Exception::EXECUTE_EXCEPTION, L"Function '" + funcName + L"' message callback did not return successfully.");
						}
					}
				
					// Exit the context.
					context->Exit();
				}
			}
		}
		
		return true;
	};
	
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
	{
		return this;
	};
	
	/*
	 * When your javascript calls a method that has been defined as a native api call, it will call this method.
	 * 
	 * This method only binds one function - namely, the setMessageCallback method.  It is the method that we call
	 * once we have a return result from a native function call.
	 */
	bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
	{		
		std::wstring s = name.ToWString();
		CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
		
		// Implementation for setMessageCallback
		if (s == L"setMessageCallback" && arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			std::wcout << L"cef3_client Execute --- setMessageCallback" << std::endl;
			std::wstring messageName = arguments[0]->GetStringValue();
			
			int browserId = context->GetBrowser()->GetIdentifier();
			callbackMap_.insert( std::make_pair(std::make_pair(messageName, browserId), std::make_pair(context, arguments[1])) );
			return true;
		}
		else
		{
			// TODO: error
			std::wcout << L"cef3_client cannot execute function '" << s << L"'" << std::endl;
			sendMessageException(context->GetBrowser(), Exception::EXECUTE_EXCEPTION, L"Cannot execute function '" + s + L"'.");
		}
		
		// Function does not exist.
		return false;
	}
	
private:
	// Map of message callbacks.
	typedef std::map<std::pair<std::wstring, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > CallbackMap;
	CallbackMap callbackMap_;
	
	// Object list
	std::map < std::wstring, ObjectBinding > objectBindingMap_;
	std::mutex objectBindingMapMutex_;
	
	int totalBindingsSent_;
	int totalBindingsReceived_;
	bool allBindingsSentMessageReceived_;
	bool readyForBindingsMessageSent_;
	

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(ClientApp)
};

int main(int argc, char *argv[])
{
#ifdef _WIN32
	CefMainArgs args(nullptr);
#else
	CefMainArgs args(argc, argv);
#endif
    CefRefPtr<ClientApp> app = new ClientApp();
	return CefExecuteProcess(args, app.get());
}



