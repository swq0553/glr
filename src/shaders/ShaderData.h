
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
	{ "glr_basic.program", std::string(
		  R"<STRING>(
#name glr_basic
#type program

#include "shader.vert"
#include "shader.frag"

)<STRING>"
		  ) }
	,
	{ "shader.vert", std::string(
		  R"<STRING>(
#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>
#include <material>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec4 pass_Color;


@bind texture0
uniform sampler2D texture;

@bind texture0
uniform
sampler2D
texture2
;

@bind Light
layout(std140)
uniform
LightSources
{
	LightSource lightSources[ NUM_LIGHTS ];
};

@bind Light2
layout(std140) uniform LightSources2 {
	LightSource lightSources2[ NUM_LIGHTS ];
};


@bind Light2
uniform LightSources3
{
	LightSource lightSources3[ NUM_LIGHTS ];
};

@bind Material
Material material = Material(
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);

@bind Color
Material color = Material (
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);

@bind Color
Material color2 = Material (
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
)
;


void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;
	
	vec3 normalDirection = normalize(normalMatrix * in_Normal);
	vec3 lightDirection = normalize(vec3(lightSources[0].direction));
	
	vec3 diffuseReflection = vec3(lightSources[0].diffuse) * vec3(material.diffuse) * max(0.0, dot(normalDirection, lightDirection));
	
	/*
	float bug = 0.0;	
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	diffuseReflection.x += bug;
	*/
	
	/*
	float bug2 = 0.0;
	bool result2 = dot(normalDirection, lightDirection) > 0.0;
	if(result2) bug2 = 1.0;
	diffuseReflection.x += bug2;
	*/
	
	pass_Color = vec4(diffuseReflection, 1.0);
	//pass_Color = vec4((0.5 * normalDirection) + vec3(0.5), 1.0);
	//pass_Color = ambient;
}

)<STRING>"
		  ) }
	,
	{ "material", std::string(
		  R"<STRING>(
#type na

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

)<STRING>"
		  ) }
	,
	{ "light", std::string(
		  R"<STRING>(
#type na

struct LightSource {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 direction;
};

)<STRING>"
		  ) }
	,
	{ "shader.frag", std::string(
		  R"<STRING>(
#version 150 core

#type fragment

@bind texture0
uniform sampler2D texture;

in vec4 pass_Color;
in vec2 textureCoord;

void main() {	
	vec4 out_Color = texture2D(texture, textureCoord);
	
	gl_FragColor = pass_Color;
	
	//gl_FragColor = out_Color;
}

)<STRING>"
		  ) }
	,
	{ "glr", std::string(
		  R"<STRING>(
#type na

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 pvmMatrix;
uniform mat3 normalMatrix;

)<STRING>"
		  ) }
};
}
}
