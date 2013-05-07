
/**
 * This file is automatically generated.  Changes made to this file will
 * not be reflected after compiling.
 *
 * If you wish to make changes to shader information for Glr, edit the shaders
 * and shader programs in the 'data/shaders/' directory.
 *
 */
 
#include <map> 
 
namespace glr {

namespace shaders {

static std::map<std::string, std::string> SHADER_DATA = {

{"glr_basic.program", std::string(
	R"<STRING>(
#name glr_basic
#type program

#include "shader.vert"
#include "shader.frag"

)<STRING>"
)}	
, 
{"shader.vert", std::string(
	R"<STRING>(
#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec3 normalDirection;
out vec3 lightDirection;

@bind Light
layout(std140) uniform Lights 
{
	Light lights[ NUM_LIGHTS ];
};

void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	vec4 lightDirTemp = viewMatrix * lights[0].direction;
	
	textureCoord = in_Texture;

	normalDirection = normalize(normalMatrix * in_Normal);
	lightDirection = normalize(vec3(lightDirTemp));
	
	/*
	float bug = 0.0;	
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	diffuseReflection.x += bug;
	*/
}

)<STRING>"
)}	
, 
{"material", std::string(
	R"<STRING>(
#type na
#name material

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	//float shininess;
	//float strength;
};

)<STRING>"
)}	
, 
{"light", std::string(
	R"<STRING>(
#type na
#name light

struct Light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 direction;
};

)<STRING>"
)}	
, 
{"shader.frag", std::string(
	R"<STRING>(
#version 150 core

#extension GL_EXT_texture_array : enable

#ifndef NUM_MATERIALS
#define NUM_MATERIALS 1
#endif

#type fragment

#include <material>

in vec2 textureCoord;
in vec3 normalDirection;
in vec3 lightDirection;

@bind texture0
uniform sampler2DArray tex;

@bind Material
layout(std140) uniform Materials 
{
	Material materials[ NUM_MATERIALS ];
};


void main() {
	vec3 ct, cf;
	vec4 texel;
	float intensity, at, af;
	intensity = max( dot(lightDirection, normalize(normalDirection)), 0.0 );
 
	cf = intensity * (materials[0].diffuse).rgb + materials[0].ambient.rgb;
	af = materials[0].diffuse.a;
	texel = texture2DArray(tex, vec3(textureCoord, 1));
 
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af);
	
	/*
	float bug = 0.0;	
	bvec4 result = equal( materials[0].diffuse, vec4(0.0, 0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	gl_FragColor.x += bug;
	*/
}

)<STRING>"
)}	
, 
{"glr", std::string(
	R"<STRING>(
#type na
#name glr

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 pvmMatrix;
uniform mat3 normalMatrix;

)<STRING>"
)}	

};

}

}
