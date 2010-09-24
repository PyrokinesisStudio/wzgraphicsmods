void main(void)
{
	// Pass vertex color information to fragment shader
	gl_FrontColor = gl_Color;

	// Pass texture coordinates to fragment shader
	gl_TexCoord[0] = gl_TextureMatrix [0] * gl_MultiTexCoord0;

	// Translate every vertex according to the Model View and Projection Matrix
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
