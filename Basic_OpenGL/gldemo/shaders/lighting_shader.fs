#version 330

uniform sampler2D texture;

layout (location=0) out vec4 fragColour;

in vec3 texCoord;
in vec3 Brightness;




void main(void) {

   // vec4 texColor = texture2D(texture, texCoord.xy) * Brightness;
   // fragColour = texColor;

   fragColour = texture2D(texture, texCoord.xy) * vec4(Brightness, 1.0);
}
