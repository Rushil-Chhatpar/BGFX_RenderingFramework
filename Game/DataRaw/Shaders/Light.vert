$input a_position, a_texcoord0, a_normal
$output v_texcoord0, v_normal, v_LightColor, v_WSPos, v_LightPos, v_CameraEyePos

#include <bgfx_shader.sh>

uniform vec4 u_UVScale;
uniform vec4 u_UVOffset;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

uniform mat4 u_NormalMatrix;

uniform vec4 u_LightColor;

uniform vec4 u_CameraEyePosition;

void main()
{
	vec4 objectSpacePosition = vec4(a_position, 1);
	vec4 worldSpacePosition = mul(u_WorldMatrix, objectSpacePosition);
	vec4 viewSpacePosition = mul(u_ViewMatrix, worldSpacePosition);
	vec4 clipSpacePosition = mul(u_ProjMatrix, viewSpacePosition);
	gl_Position = clipSpacePosition;

	v_texcoord0 = a_texcoord0 * u_UVScale.xy + u_UVOffset.xy;
	
	v_normal = mul(u_NormalMatrix, vec4(a_normal, 0)).xyz;

	v_LightPos = vec3(0,0,0);

	v_WSPos = worldSpacePosition.xyz;

	v_LightColor = u_LightColor;

	v_CameraEyePos = u_CameraEyePosition;
}
