uniform vec4 zenithColor;
uniform vec4 horizonColor;
uniform vec4 nadirColor;
uniform mat4 inv_vp;

in vec3 fragmentPosition;

out vec4 FragmentColor;

void main( ){

	vec4 direction = inv_vp * vec4(fragmentPosition.xy, -1.0,0.0);

	float k = direction.z / length(direction.xyz);

    if (k> 0) FragmentColor = mix(horizonColor, zenithColor,sqrt( k) );
    else      FragmentColor = mix(horizonColor, nadirColor, sqrt(-k));

 }
