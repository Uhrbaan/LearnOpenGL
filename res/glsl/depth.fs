#version 460 core
out vec4 FragColor;

float near = .1;
float far  = 100.;

// makes the depth buffer z_coo linerar -> precision
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // / far for demo
    FragColor = vec4(vec3(depth), 1.0);
}
