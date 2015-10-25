#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout = vin;
	vout.pos = mul(matWVP,vin.pos);
	return vout;
}