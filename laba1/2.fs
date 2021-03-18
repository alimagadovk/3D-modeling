varying vec4 pos; // координата пикселя в пространстве
varying vec2 texCoord; // текстурная координата пикселя
uniform float param; // некоторый параметр, например, время
uniform sampler2D texture1; // текстура
varying float attr1_toFragment; // атрибут пикселя (интерполяция атрибута вершины), например, яркость
float gc;

void main() {
	vec4 texColor = texture2D(texture1, texCoord); // цвет из  текстур
	texColor[0] = texColor[0]/3 + texColor[1]/3 + texColor[2]/3;
	texColor[1] = texColor[0];
	texColor[2] = texColor[0];

	vec4 brightness = vec4(attr1_toFragment, attr1_toFragment, attr1_toFragment, 1); // яркость пикселя
	gl_FragColor = texColor * brightness; // покомпонентное умножение
}