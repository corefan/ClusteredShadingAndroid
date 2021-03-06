/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");// you may not use this file except in compliance with the License.// You may obtain a copy of the License at//// http://www.apache.org/licenses/LICENSE-2.0//// Unless required by applicable law or agreed to in writing, software// distributed under the License is distributed on an "AS IS" BASIS,// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.// See the License for the specific language governing permissions and// limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __CPUT_SAMPLESTARTDX11_H__
#define __CPUT_SAMPLESTARTDX11_H__

#include <stdio.h>
#include <time.h>
#include "CPUTScene.h"
#include "CPUT_OGL.h"
#include "CPUTSpriteOGL.h"
#include "CPUTRenderTargetOGL.h"
#include "GLProgram.h"
#include "Structures.h"
#include "LightGrid.h"
#include "ShaderDefines.h"

// define some controls
const CPUTControlID ID_MAIN_PANEL = 10;
const CPUTControlID ID_SECONDARY_PANEL = 20;
const CPUTControlID ID_FULLSCREEN_BUTTON = 100;
const CPUTControlID ID_NEXTMODEL_BUTTON = 101;
enum
{
    ID_SHADING_TECH_DROPDOWN,
    ID_NUM_LIGHTS_SLIDER,
    ID_ANIMATE_LIGHTS_CHECKBOX,
    ID_LIGHTING_ONLY_CHECKBOX,
    ID_VISUALIZE_LIGHT_COUNT_CHECKBOX
};

#define FPS_VALUES 90

#define PI    ((float)3.141592654f)

//-----------------------------------------------------------------------------
class ClusteredShadingSample : public CPUT_OGL
{
private:
    float                  mfElapsedTime;
    CPUTAssetSet          *mpAssetSet;
    CPUTCameraController  *mpCameraController;
    CPUTSprite            *mpDebugSprite;

    CPUTAssetSet          *mpShadowCameraSet;
    CPUTFramebufferOGL    *mpShadowRenderTarget;

    CPUTScene             *mpScene;
    std::vector<CPUTMaterial*> mpMaterials;
    CPUTDropdown          *mpShadingTechDropdown;
    CPUTCheckbox          *mpAnimateLightsCheckbox;
    
    CGLProgram m_DeferredNoCullProgram;
    CGLProgram m_GPUQuadProgram;
    CGLProgram m_TiledDeferredCSProgram;
	CGLProgram m_CopyTextureProgram;

    unsigned int mActiveLights;
    std::vector<PointLightInitTransform> mLightInitialTransform;
    std::vector<PointLight> mPointLightParameters;
    std::vector<float> mLightsInfoTextureData;
    std::vector<unsigned int> mLightGridTextureData;
    std::vector<float3> mPointLightPositionWorld;

    LightGridBuilder mLightGridBuilder;

    void InitializeLightParameters();
    void ClusterCullingRasterizeLights(const float4x4& mCameraProj);
    void UpdateLights(const float4x4& cameraView);
    void Move(float elapsedTime);
    float mTotalTime;

    void RenderForward(CPUTRenderParameters &renderParams);
    void RenderForwardClustered(CPUTRenderParameters &renderParams);
    void RenderGBuffer(CPUTRenderParameters &renderParams);
    void ComputeLighting(CPUTRenderParameters &renderParams);
	void SetGLProgramUniforms(GLenum GLProgram, bool BindLightsBuffer = true);
    enum MATERIAL
    {
        MATERIAL_FORWARD = 0,
        MATERIAL_CLUSTERED = 1,
        MATERIAL_GBUFFER = 2
    };
    void RenderScene(CPUTRenderParameters &renderParams, int MaterialIndex);

    CPUTTextureOGL*  mpPointLightsInfoTexture;
    CPUTTextureOGL*  mpLightGridTexture;
    UIConstants mUIConstants;
    CPUTBufferOGL*   mpUIConstantsBuffer;
    CPUTTextureOGL*  mpGBufferDiffuseColor;
    CPUTTextureOGL*  mpGBufferNormal;
    CPUTTextureOGL*  mpGBufferLightMap;
    CPUTTextureOGL*  mpGBufferDepth;
	CPUTTextureOGL*  mpShadedBackBuffer;

    GLuint          m_GBufferFBO;
    GLuint          m_DummyVAO;
	UINT			mBackBufferWidth, mBackBufferHeight;

public:
    ClusteredShadingSample() : 
        mpAssetSet(NULL),
        mpCameraController(NULL),
        mpDebugSprite(NULL),
        mpShadowCameraSet(NULL),
        mActiveLights(0),
        mTotalTime(0),
        mpPointLightsInfoTexture(nullptr),
        mpGBufferDiffuseColor(nullptr),
        mpGBufferNormal(nullptr),
        mpGBufferLightMap(nullptr),
        mpGBufferDepth(nullptr),
		mpShadedBackBuffer(nullptr),
        m_GBufferFBO(0),
        m_DummyVAO(0),
		mBackBufferWidth(0), 
		mBackBufferHeight(0)
    {}
    virtual ~ClusteredShadingSample()
    {
    }
    virtual CPUTEventHandledCode HandleKeyboardEvent(CPUTKey key, CPUTKeyState state);
    virtual CPUTEventHandledCode HandleMouseEvent(int x, int y, int wheel, CPUTMouseState state, CPUTEventID  message);
    virtual void                 HandleCallbackEvent( CPUTEventID Event, CPUTControlID ControlID, CPUTControl *pControl );
    virtual void HandleGUIElementEvent( CPUTEventID Event, CPUTControlID ControlID, CPUTGUIElement *pElement );

    virtual void Create();
    virtual void Render(double deltaSeconds);
    virtual void Update(double deltaSeconds);
    virtual void ResizeWindow(UINT width, UINT height);
    void ReleaseResources();

    void LoadAssets();

    CPUTEventHandledCode HandleCameraControlButtons(CPUTEventID Event, CPUTControlID ControlID, CPUTControl * pControl);

    void SetActiveLights(unsigned int activeLights);
};
#endif // __CPUT_SAMPLESTARTDX11_H__
