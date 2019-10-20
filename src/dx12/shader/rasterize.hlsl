

//RootSignature
#define RS \
	"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)," \
	"DescriptorTable(CBV(b0))," /* ConstantBuffer */ \
	"DescriptorTable(SRV(t0, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE, space = 1))," /* texlist */ \
	"StaticSampler(s0,"\
	"           filter = FILTER_MIN_MAG_MIP_LINEAR,"\
	"           addressU = TEXTURE_ADDRESS_WRAP,"\
	"           addressV = TEXTURE_ADDRESS_WRAP,"\
	"           addressW = TEXTURE_ADDRESS_WRAP )," /* texsampler */ \


cbuffer Constants : register(b0)
{
	row_major matrix wvp : packoffset(c4);
};

struct VSInput
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 uv : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

[RootSignature(RS)]
PSInput vsmain(VSInput input)
{
	PSInput result;
	result.position = mul(input.position, wvp);
	result.normal = input.normal.xyz;
	result.uv = input.uv.xy;
	return result;
}

float4 psmain(PSInput input) : SV_TARGET0
{
	float4 output = float4(1, 0, 0, 1);

	return output;
}
