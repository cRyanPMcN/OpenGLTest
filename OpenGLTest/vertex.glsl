#version 460 core

struct Location {
	float scale;
	vec3 orientation;
	vec4 position;
};

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 modelPosition;
uniform vec4 modelOrientation;
uniform vec3 cameraPosition;
uniform vec4 cameraOrientation;
//struct model {
//	vec3 orientation;
//	vec4 position;
//};
//struct camera {
//	vec3 orientation;
//	vec4 position;
//};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColour;
out vec2 TexCoord;

vec4 QuaternionConjugate(vec4 quaternion) {
	return vec4(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
};

vec4 QuaternionMultiply(vec4 quatLeft, vec4 quatRight) {
	//vec4 qr;
	//qr.w = (quatLeft.w * quatRight.w) - (quatLeft.x * quatRight.x) - (quatLeft.y * quatRight.y) - (quatLeft.z * quatRight.z);
	//qr.x = (quatLeft.w * quatRight.x) + (quatLeft.x * quatRight.w) + (quatLeft.y * quatRight.z) - (quatLeft.z * quatRight.y);
	//qr.y = (quatLeft.w * quatRight.y) - (quatLeft.x * quatRight.z) + (quatLeft.y * quatRight.w) + (quatLeft.z * quatRight.x);
	//qr.z = (quatLeft.w * quatRight.z) + (quatLeft.x * quatRight.y) - (quatLeft.y * quatRight.x) + (quatLeft.z * quatRight.w);

	//qr.w = (quatLeft.w * quatRight.w) - (quatLeft.x * quatRight.x) - (quatLeft.y * quatRight.y) - (quatLeft.z * quatRight.z);
	//qr.x = (quatLeft.w * quatRight.x) + (quatLeft.x * quatRight.w) + (quatLeft.y * quatRight.z) - (quatLeft.z * quatRight.y);
	//qr.y = (quatLeft.w * quatRight.y) + (quatLeft.y * quatRight.w) + (quatLeft.z * quatRight.x) - (quatLeft.x * quatRight.z);
	//qr.z = (quatLeft.w * quatRight.z) + (quatLeft.z * quatRight.w) + (quatLeft.x * quatRight.y) - (quatLeft.y * quatRight.x);
	//return qr;
	
	return vec4(quatLeft.w * quatRight.xyz + quatRight.w * quatLeft.xyz + cross(quatLeft.xyz, quatRight.xyz), quatLeft.w * quatRight.w - dot(quatLeft.xyz, quatRight.xyz));
};

vec3 QuaternionRotate(const vec4 quaternion, const vec3 point) {
	vec4 rotateOne = vec4(quaternion.w * point + cross(quaternion.xyz, point), dot(quaternion.xyz, point));
	return vec3(quaternion.w * rotateOne.xyz + rotateOne.w * quaternion.xyz + cross(quaternion.xyz, rotateOne.xyz));
};

void main() {
    //gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
	//gl_Position = projection * view * vec4(QuaternionRotate(modelOrientation, vertexPosition) + modelPosition, 1.0);
	//gl_Position = projection * vec4(QuaternionRotate(modelOrientation, vertexPosition) + modelPosition, 1.0);

	gl_Position = projection * vec4(QuaternionRotate(cameraOrientation, QuaternionRotate(modelOrientation, vertexPosition * .01f) + modelPosition - cameraPosition), 1.0);
	//gl_Position = projection * vec4(QuaternionRotate(QuaternionMultiply(cameraOrientation, modelOrientation), vertexPosition) + QuaternionRotate(cameraOrientation, modelPosition + cameraPosition), 1.0);
	TexCoord = aTexCoord;
}