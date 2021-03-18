varying vec3 v;
varying vec2 texCoord;
uniform sampler2D texture1;
varying vec3 vT;
varying vec3 vB;
varying vec3 vN;

void main(void)
{
    vec4 result = vec4(0.0);
	vec4 normal4 = gl_ModelViewMatrix * (2*texture2D(texture1, texCoord) - 1);
	vec3 n = normalize(normal4.xyz);
	vec3 n2;
	n2[0] = n[0]*vT[0] + n[1]*vB[0] + n[2]*vN[0];
	n2[1] = n[0]*vT[1] + n[1]*vB[1] + n[2]*vN[1];
	n2[2] = n[0]*vT[2] + n[1]*vB[2] + n[2]*vN[2];
	n = normalize(n2);

    for (int i = 0; i < gl_MaxLights; i++)
    {
        vec3 l = normalize(gl_LightSource[i].position);
        vec3 e = normalize(-v);
        vec3 r = normalize(-reflect(l, n));

        vec4 Iamb = gl_FrontLightProduct[i].ambient;

        vec4 Idiff = gl_FrontLightProduct[i].diffuse * dot(n, l);
        Idiff = clamp(Idiff, 0.0, 1.0);

        vec4 Ispec = gl_FrontLightProduct[i].specular * pow(dot(r, e), 50);
        Ispec = clamp(Ispec, 0.0, 1.0);

        result += Iamb + Idiff + Ispec;
    }

    gl_FragColor = gl_FrontLightModelProduct.sceneColor + result;
}

