#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCords;
layout (location = 3) in vec4 aBoneIds;
layout (location = 4) in vec4 aBoneWeights;


out vec2 TexCoords;
out vec3 Normal;

uniform mat4 bone_transforms[100];
uniform mat4 CameraMat;
uniform mat4 ModelMat;

void main()
{
	mat4 boneTransform  =  mat4(0.0);
		boneTransform  +=    bone_transforms[int(aBoneIds.x)] * aBoneWeights.x;
		boneTransform  +=    bone_transforms[int(aBoneIds.y)] * aBoneWeights.y;
		boneTransform  +=    bone_transforms[int(aBoneIds.z)] * aBoneWeights.z;
		boneTransform  +=    bone_transforms[int(aBoneIds.w)] * aBoneWeights.w;

	gl_Position = CameraMat*ModelMat*boneTransform*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoords = aTexCords;
	Normal = aNormal;

}