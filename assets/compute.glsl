#version 460
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_atomic_counters : enable

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D dataImage;
layout(binding = 1, offset=0) uniform atomic_uint counterBuffer[10];
layout(binding = 2) buffer oldCentroidsBuffer {
    uint oldCentroids[];
};
layout(binding = 3) uniform atomic_uint newCentroidsBuffer[10*3];

void main() {
    uvec2 pos = gl_GlobalInvocationID.xy;
    vec3 value = imageLoad(dataImage, ivec2(pos)).rgb;
    // The 32-bit unsigned integer is four uint8_ts
    float minDistance = 10000.0;
    uint minCentroid = 10000;
    uint index = pos.x + pos.y * gl_NumWorkGroups.x;
    for (uint i = 0; i < 10; i++) {
	float dist = distance(value, vec3(oldCentroids[i * 3] / 255.0, oldCentroids[i * 3 + 1]/255.0, oldCentroids[i * 3 + 2]/255.0));
	if (dist <= minDistance) {
	    minDistance = dist;
	    minCentroid = i;
	}
    }
    // Offset counter buffer to minCentroid
    atomicCounterIncrement(counterBuffer[minCentroid]);
    // Write to newCentroidsBuffer
    atomicCounterAdd(newCentroidsBuffer[minCentroid * 3], uint(value.r * 255.0));
    atomicCounterAdd(newCentroidsBuffer[minCentroid * 3 + 1], uint(value.g * 255.0));
    atomicCounterAdd(newCentroidsBuffer[minCentroid * 3 + 2], uint(value.b * 255.0));
}
