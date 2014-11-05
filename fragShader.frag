uniform sampler2D texture;
uniform sampler2D normal_texture;
varying vec4 texc;
varying vec3 lPos;

void main(void)
{
    vec3 nColor = texture2D(normal_texture, texc.st);
    vec4 color = texture2D(texture, texc.st).rgba;

    vec3 normal = normalize(nColor * 2.0 - 1.0);

    vec3 deltaPos = vec3( (lPos.xy - gl_FragCoord.xy) / vec2(500.0, 500.0).xy, lPos.z );
    vec3 lightDir = normalize(deltaPos);

    float lambert = clamp(dot(normal, lightDir), 0.0, 1.0);

    float d = sqrt(dot(deltaPos, deltaPos));
    float att = 1.0 / (2+(d*d));
    vec3 result = 0.25 + lambert * att;
    result *= color.rgb;
    gl_FragColor = vec4(result, color.a);
}
