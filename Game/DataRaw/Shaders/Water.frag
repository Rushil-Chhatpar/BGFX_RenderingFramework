$input v_texcoord0, v_normal

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );

void main()
{
    vec4 color = texture2D( u_TextureColor, v_texcoord0 );

    color.rgb += vec3(0.1, 0.3, 0.7);

    float perc = color.x;
    //color.rgb = color1 * (1-perc) + color2 + perc;
    vec3 color1 = vec3(0.1, 0.4, 0.7);
    vec3 color2 = vec3(0.0, 0.0, 0.1);
    color.rgb = mix(color2, color1, perc);

    gl_FragColor = color;
    //gl_FragColor = vec4(v_normal, 1);
}
