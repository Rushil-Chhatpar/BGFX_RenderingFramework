$input a_position

#include <bgfx_shader.sh>

uniform vec4 u_Scale;
uniform vec4 u_Rotation;
uniform vec4 u_Position;
uniform vec4 u_ProjectionScale;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

vec2 TransformIntoWorldSpace(vec2 pos);

void main()
{
	vec4 objectSpacePosition = vec4(a_position, 1);
	vec4 worldSpacePosition = mul(u_WorldMatrix, objectSpacePosition);
	vec4 viewSpacePosition = mul(u_ViewMatrix, worldSpacePosition);
	vec4 clipSpacePosition = mul(u_ProjMatrix, viewSpacePosition);
	gl_Position = clipSpacePosition;
}

vec2 TransformIntoWorldSpace(vec2 pos)
{
	// Scale.
	pos *= u_Scale.xy;
	
	// Rotate.
	float newX = cos(u_Rotation.x)*pos.x + -sin(u_Rotation.x)*pos.y;
	float newY = sin(u_Rotation.x)*pos.x + cos(u_Rotation.x)*pos.y;
	pos = vec2(newX, newY);
	
	// Translate.
	pos += u_Position.xy;

	return pos;
}
