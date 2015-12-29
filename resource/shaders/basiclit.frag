#version 410 core

in vec2 fUV;
in vec3 fNormal;
in vec3 fViewSpace;

out vec4 colour;

uniform sampler2D sTexture0;
uniform vec3 directionalLight;	// Should be normalised!

void main()
{
	// Get our colour from the texture.
	vec4 col = texture(sTexture0, fUV);

	// Get the dot product of the normal with the light.
	// We rescale this to make sure that the faces don't get too dark.
	// The rescaling is [0.5 1]
	float dp = dot(fNormal, directionalLight);
        dp = 0.5 + (0.5 * ((0.5*dp) + 0.5));

	// Multiply the texture colour value with the dot product.
        colour = vec4((dp * col.xyz), col.w);
}
