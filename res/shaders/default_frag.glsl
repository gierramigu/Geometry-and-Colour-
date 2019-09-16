#version 330 core
//Assignment_1
// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

//color 
uniform vec3 u_Color; 

//Light 
uniform vec3 lightColor;
uniform vec3 specColor;


// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
} f_in;

// framebuffer output
out vec4 fb_color;
//out vec4 FragColor;
//out vec4 colorOut;

void main() {
	// calculate shading
	//vec3 surfaceColor = vec3(0.066, 0.341, 0.215); //original colour
	//vec3 surfaceColor = vec3(0.255, 0.000, 0.000); 

	//vec3 eye = normalize(-f_in.position); // direction towards the eye
	vec3 eye = vec3(lightColor.x, lightColor.y, lightColor.z); // direction towards the eye

	float light = abs(dot(normalize(f_in.normal), eye)); // difference between the surface normal and direction towards the eye

	vec3 finalColor = mix(u_Color / 4, u_Color, light);
	//vec3 finalColor = mix(surfaceColor / 4, surfaceColor, light);

	// output to the framebuffer
	fb_color = vec4(finalColor, 1);
	
	
}
