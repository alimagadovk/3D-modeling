varying vec2 texCoord;
varying vec3 v;
attribute vec3 T;
attribute vec3 B;
attribute vec3 N;
varying vec3 vT;
varying vec3 vB;
varying vec3 vN;

void main(void)
{
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
	vT = T;
	vB = B;
	vN = N;
	texCoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
