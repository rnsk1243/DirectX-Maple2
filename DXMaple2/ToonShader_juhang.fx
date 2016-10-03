//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ToonShader
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string ToonShader_Pass_0_Model : ModelData = ".\\house_non_left.X";

struct VS_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal : NORMAL;
      float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float3 mDiffuse : TEXCOORD1;
      float2 mTexCoord : TEXCOORD0;
};

float4 gWorldLightPosition
<
   string UIName = "gWorldLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );

float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
float4x4 gInvWorldMatrix : WorldInverse;

VS_OUTPUT ToonShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul(Input.mPosition,gWorldViewProjectionMatrix);
// Output.mPosition = mul(gWorldViewProjectionMatrix,Input.mPosition);
   float3 objectLightPosition = mul(gWorldLightPosition,gInvWorldMatrix);
//   float3 objectLightPosition = mul(gInvWorldMatrix,gWorldLightPosition);
   float3 lightDir = normalize(Input.mPosition.xyz - objectLightPosition);
   
   Output.mDiffuse = dot(-lightDir,normalize(Input.mNormal));
   Output.mTexCoord = Input.mTexCoord;
   return Output;
}
float3 gSurfaceColor
<
   string UIName = "gSurfaceColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 1.00, 0.50, 0.00 );
texture DiffuseMap_Tex
<
   string ResourceName = ".\\house.png";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct PS_INPUT
{
   float3 mDiffuse : TEXCOORD1;
   float2 mTexCoord : TEXCOORD0;
};

float4 ToonShader_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float3 diffuse = saturate(Input.mDiffuse);
    float4 albedo = tex2D(DiffuseSampler,Input.mTexCoord);
   diffuse = ceil(diffuse * 5.0f)/5.0f;

   return float4(diffuse * gSurfaceColor + albedo.rgba, 1);
}
//--------------------------------------------------------------//
// Technique Section for ToonShader
//--------------------------------------------------------------//
technique ToonShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 ToonShader_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ToonShader_Pass_0_Pixel_Shader_ps_main();
   }

}

