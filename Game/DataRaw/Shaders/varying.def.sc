vec3 a_position  : POSITION;
vec4 a_color0    : COLOR0;
vec2 a_texcoord0 : TEXCOORD0;
vec3 a_normal	 : NORMAL;

vec4 v_color0    : COLOR0    = vec4(0.0, 0.0, 0.0, 1.0);
vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
vec3 v_normal    : NORMAL    = vec3(0.0, 1.0, 0.0);
vec4 v_LightColor: TEXCOORD1 = vec4(0.0, 0.0, 0.0, 1.0);
vec3 v_WSPos     : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
vec3 v_LightPos  : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
vec4 v_CameraEyePos:TEXCOORD4= vec4(0.0, 0.0, 0.0, 1.0);