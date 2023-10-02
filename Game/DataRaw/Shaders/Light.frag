$input v_texcoord0, v_normal, v_LightColor, v_WSPos, v_LightPos, v_CameraEyePos

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );

uniform vec4 u_LightPosition[8];
uniform vec4 u_LightRange[8];


void main()
{
    vec3 finalColor;
    vec3 additiveColor = vec3(0,0,0);
    vec4 color = texture2D( u_TextureColor, v_texcoord0 );
    for(int i=0;i<8;i++)
    {
    int index = i;

    vec3 dirToLight = u_LightPosition[i].xyz - v_WSPos;

	vec3 normalizedDir;

	normalizedDir = normalize(dirToLight);

	float diffuse = dot(normalizedDir, v_normal);

    //float range = u_LightRange[index].x; //this doesnt work for some reason
    float range = 60.0f;

    float shininess = 200.0f;

    float reffect = 1 - (length(dirToLight)/range);

//    if(reffect < 0)
//    {
//    reffect = 0.0f;
//    }

    reffect = max(0, reffect);

    reffect = pow(reffect, 3.7f);

    vec3 distToCam = v_WSPos - v_CameraEyePos.xyz;

    vec3 halfVector = normalize(dirToLight + distToCam);

    float specPerc = max(0,dot(halfVector, v_normal));

    specPerc = pow(specPerc, shininess);

    //float specPerc = pow(dot(v_normal, halfVector), shininess);

    finalColor += color.rgb * v_LightColor.rgb * diffuse;

    finalColor += v_LightColor.rgb * specPerc;

    finalColor *= reffect;

    additiveColor += finalColor;
    }

    gl_FragColor = vec4(additiveColor, color.a);

}
    //gl_FragColor = vec4(v_normal, 1);
    //gl_FragColor = color * v_LightColor;
