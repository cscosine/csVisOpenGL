uniform sampler2D tex;

in vec4 outputColor;
in vec2 tc;

out vec4 FragmentColor;

void main()
{

	FragmentColor = outputColor * texture(tex, tc);
}
