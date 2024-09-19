#pragma once

#include "NiNodes.h"

// UNMODIFIED OBSE FILE. I need NiNode class definition.

/***										type
 *	NiAVObject
 *		NiDynamicEffect
 *			NiLight
 *				NiDirectionalLight			01
 *				NiPointLight				02
 *					NiSpotLight				03
 *				NiAmbientLight				00
 *			NiTextureEffect					04
 *		NiNode
 *			SceneGraph
 *			BSTempNodeManager
 *			BSTempNode
 *			BSCellNode
 *			BSClearZNode
 *			BSFadeNode
 *			BSScissorNode
 *			BSTimingNode
 *			BSFaceGenNiNode
 *			NiBillboardNode
 *			NiSwitchNode
 *				NiLODNode
 *					NiBSLODNode
 *			NiSortAdjustNode
 *			NiBSPNode
 *			ShadowSceneNode
 *		NiCamera
 *			BSCubeMapCamera					RTTI data incorrect
 *			NiScreenSpaceCamera
 *		NiGeometry
 *			NiLines
 *			NiTriBasedGeom
 *				NiTriShape
 *					BSScissorTriShape
 *					NiScreenElements
 *					NiScreenGeometry
 *					TallGrassTriShape
 *				NiTriStrips
 *					TallGrassTriStrips
 *			NiParticles
 *				NiParticleSystem
 *					NiMeshParticleSystem
 *				NiParticleMeshes
 */

class NiCamera;
class NiLODData;
class ShadowSceneLight;	// 220, derives from NiRefObject
class BSImageSpaceShader;
class NiScreenPolygon;
class NiScreenTexture;
class NiPSysModifier;
class NiRenderer;
class NiGeometryData;

class bhkNiCollisionObject;

static struct NiUpdateData
{
	float fTime;
	UInt8 isUpdateControllers;
	UInt8 bIsMultiThreaded;
	UInt8 byte06;
	UInt8 bUpdateGeomorphs;
	UInt8 bUpdateShadowSceneNode;
	UInt8 gap09[3];
} DefaultNodeUpdateParams;

class NiFrustumPlanes {
public:
	NiFrustumPlanes() {
		m_uiActivePlanes = 63;
	}

	enum ActivePlanes {
		NEAR_PLANE		= 0,
		FAR_PLANE		= 1,
		LEFT_PLANE		= 2,
		RIGHT_PLANE		= 3,
		TOP_PLANE		= 4,
		BOTTOM_PLANE	= 5,
		MAX_PLANES		= 6
	};

	NiPlane	m_akCullingPlanes[MAX_PLANES];
	UInt32	m_uiActivePlanes;
};

class NiProperty : public NiObjectNET {
public:
	NiProperty();
	virtual ~NiProperty();

	virtual UInt32	Type();
	virtual void	Update(NiUpdateData& apUpdateData);

	enum PropertyType {
		ALPHA,
		CULLING,
		MATERIAL,
		SHADE,
		STENCIL,
		TEXTURING,
		MAX_TYPES
	};
};

class NiShadeProperty : public NiProperty {
public:
	NiShadeProperty();
	virtual ~NiShadeProperty();

	enum ShaderPropType : UInt32 {
		PROP_Lighting			= 1,
		PROP_DistantLOD			= 2,
		PROP_GeometryDecal		= 3,
		PROP_TallGrass			= 4,
		//						  5,
		PROP_SpeedTreeLeaf		= 6,
		//						  7,
		PROP_PPLighting			= 8,
		PROP_Hair				= 9,
		PROP_SpeedTreeBranch	= 10,
		PROP_SpeedTreeBillboard = 11,
		PROP_Lighting30			= 12,
		PROP_Sky				= 13,
		PROP_Water				= 14,
		PROP_Bolt				= 15,
		//						  16,
		PROP_Particle			= 17,
		PROP_Precipitation		= 18,
		PROP_Tile				= 19,
		//						  20,
		PROP_NoLighting			= 21,
		PROP_VolumetricFog		= 22,
		PROP_BloodSplatter		= 23,
		PROP_DistantTree		= 24,
	};


	Bitfield16		m_usFlags;
	ShaderPropType	m_eShaderType;

	bool IsLightShader() const { return m_eShaderType < 13; };
};

// 9C
class NiAVObject : public NiObjectNET
{
public:
	NiAVObject();
	~NiAVObject();

	virtual void			UpdateControllers(NiUpdateData& arData);
	virtual void			ApplyTransform(NiMatrix33& arMat, NiPoint3& arTrn, bool abOnLeft);
	virtual void			Unk_39();
	virtual NiAVObject*		GetObject_(const NiFixedString& arName);
	virtual NiAVObject*		GetObjectByName(const NiFixedString& arName);
	virtual void			SetSelectiveUpdateFlags(bool& arSelectiveUpdate, BOOL abSelectiveUpdateTransforms, bool& arRigid);
	virtual void			UpdateDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateSelectedDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateRigidDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdatePropertiesDownward(NiPropertyState* apParentState);
	virtual void			Unk_47();
	virtual void			UpdateWorldData(const NiUpdateData& arData);
	virtual void			UpdateWorldBound();
	virtual void			UpdateTransformAndBounds(const NiUpdateData& arData);
	virtual void			PreAttachUpdate(NiNode* apEventualParent, const NiUpdateData& arData);
	virtual void			PreAttachUpdateProperties(NiNode* apEventualParent);
	virtual void			DetachParent();
	virtual void			UpdateUpwardPassParent(void* arg);
	virtual void			OnVisible(NiCullingProcess* apCuller);
	virtual void			PurgeRendererData(NiDX9Renderer* apRenderer);

	enum
	{
		kNiFlag_Culled = 0x1,
		kNiFlag_SelectiveUpdate = 0x2,
		kNiFlag_SelUpdTransforms = 0x4,
		kNiFlag_SelUpdController = 0x8,
		kNiFlag_SelUpdRigid = 0x10,
		kNiFlag_DisplayObject = 0x20,
		kNiFlag_DisableSorting = 0x40,
		kNiFlag_SelUpdTransformsOverride = 0x80,
		kNiFlag_UnkBit8 = 0x100,
		kNiFlag_SaveExternalGeomData = 0x200,
		kNiFlag_NoDecals = 0x400,
		kNiFlag_AlwaysDraw = 0x800,
		kNiFlag_MeshLOD = 0x1000,
		kNiFlag_FixedBound = 0x2000,
		kNiFlag_TopFadeNode = 0x4000,
		kNiFlag_IgnoreFade = 0x8000,
		kNiFlag_NoAnimSyncX = 0x10000,
		kNiFlag_NoAnimSyncY = 0x20000,
		kNiFlag_NoAnimSyncZ = 0x40000,
		kNiFlag_NoAnimSyncS = 0x80000,
		kNiFlag_NoDismember = 0x100000,
		kNiFlag_NoDismemberValidity = 0x200000,
		kNiFlag_RenderUse = 0x400000,
		kNiFlag_MaterialsApplied = 0x800000,
		kNiFlag_HighDetail = 0x1000000,
		kNiFlag_ForceUpdate = 0x2000000,
		kNiFlag_PreProcessedNode = 0x4000000,
		kNiFlag_UnkBit27 = 0x8000000,
		kNiFlag_UnkBit28 = 0x10000000,
		kNiFlag_UnkBit29 = 0x20000000,
		kNiFlag_UnkBit30 = 0x40000000,
		kNiFlag_UnkBit31 = 0x80000000,
	};

	NiNode* m_parent;				// 18
	bhkNiCollisionObject* m_collisionObject;		// 1C
	NiSphere* m_kWorldBound;			// 20
	DList<NiProperty>		m_propertyList;			// 24
	UInt32					m_flags;				// 30
	union
	{
		struct
		{
			NiMatrix33				m_localRotate;			// 34
			NiVector3				m_localTranslate;		// 58
			float					m_localScale;			// 64
		};
		NiTransform m_kLocal;
	};

	union
	{
		struct
		{
			NiMatrix33				m_worldRotate;			// 68
			NiVector3				m_worldTranslate;		// 8C
			float					m_worldScale;			// 98
		};
		NiTransform m_kWorld;
	};

	void SetVisible(bool visible) { visible ? (m_flags &= ~kNiFlag_Culled) : (m_flags |= kNiFlag_Culled); };
	bool IsVisible() { return !(m_flags & kNiFlag_Culled); }
	bool IsSelectiveUpdate() { return m_flags & kNiFlag_SelectiveUpdate; };
	void UpdatePropertiesUpward() { ThisCall(0x80E140, this); };

	void Update(NiUpdateData& arData);
};
STATIC_ASSERT(sizeof(NiAVObject) == 0x9C);

#if 0

// DC+
class NiDynamicEffect : public NiAVObject
{
public:
	NiDynamicEffect();
	~NiDynamicEffect();

	virtual UInt32	GetEffectType(void);

	UInt8	enable;		// 0AC
	UInt8	pad0AD[3];	// 0AD
	UInt32	unk0B0;		// 0B0 - init'd to monotonically increasing number (uid?)
	UInt32	unk0B4;		// 0B4
	UInt32	unk0B8;		// 0B8 - init'd to 1

	NiTPointerList <NiNode *>	affectedNodes;		// 0BC
	NiTPointerList <NiNode *>	unaffectedNodes;	// 0CC
};

// 108
class NiLight : public NiDynamicEffect
{
public:
	NiLight();
	~NiLight();

	float	m_fDimmer;	// 0DC
	NiColor	m_kAmb;		// 0E0
	NiColor	m_kDiff;	// 0EC
	NiColor	m_kSpec;	// 0F8
	UInt32	unk104;		// 104
};

// 114
class NiDirectionalLight : public NiLight
{
public:
	NiDirectionalLight();
	~NiDirectionalLight();

	// UpdateTransform ticks unk0B8 and copies m_direction

	enum { kType = 1 };

	NiVector3	m_direction;	// 108
};

// 114
class NiPointLight : public NiLight
{
public:
	NiPointLight();
	~NiPointLight();

	// UpdateTransform ticks unk0B8

	enum { kType = 2 };

	float	m_fAtten0;	// 108
	float	m_fAtten1;	// 10C
	float	m_fAtten2;	// 110
};

// 128
class NiSpotLight : public NiPointLight
{
public:
	NiSpotLight();
	~NiSpotLight();

	// UpdateTransform ticks unk0B8 and copies m_kWorldDir

	enum { kType = 3 };

	NiVector3	m_kWorldDir;		// 114
	float		m_fSpotAngle;		// 120
	float		m_fSpotExponent;	// 124
};

// 108
class NiAmbientLight : public NiLight
{
public:
	NiAmbientLight();
	~NiAmbientLight();

	enum { kType = 0 };
};

// 174
class NiTextureEffect : public NiDynamicEffect
{
public:
	NiTextureEffect();
	~NiTextureEffect();

	enum { kType = 4 };

	enum
	{
		kTextureMode_ProjectedLight = 0,
		kTextureMode_ProjectedShadow,
		kTextureMode_EnvironmentMap,
		kTextureMode_FogMap,
	};

	enum
	{
		kCoordMode_WorldParallel = 0,
		kCoordMode_WorldPerspective,
		kCoordMode_SphereMap,
		kCoordMode_SpecularCubeMap,
		kCoordMode_DiffuseCubeMap,
	};

	NiMatrix33	m_kModelProjMat;	// 0DC
	NiVector3	m_kModelProjTrans;	// 100
	NiMatrix33	m_kWorldProjMat;	// 10C
	NiVector3	m_kWorldProjTrans;	// 130
	NiObject	* m_texture;		// 13C
	UInt32		m_eFilter;			// 140 - see NiTexturingProperty::kFilterMode_* without the shift
	UInt32		m_eClamp;			// 144 - see NiTexturingProperty::kClampMode_* without the shift
	UInt32		m_eTextureMode;		// 148
	UInt32		m_eCoordMode;		// 14C
	UInt8		m_bPlaneEnable;		// 150
	UInt8		pad151[3];			// 151
	NiPlane		m_kModelPlane;		// 154
	NiPlane		m_kWorldPlane;		// 164
};

#endif

// AC
class NiNode : public NiAVObject
{
public:
	NiNode();
	~NiNode();

	virtual void	AttachChild(NiAVObject* apChild, bool abFirstAvail);
	virtual void	InsertChildAt(UInt32 i, NiAVObject* apChild);
	virtual void	DetachChild(NiAVObject* apChild, NiPointer<NiAVObject>& arResult);
	virtual void	DetachChildAlt(NiAVObject* apChild);
	virtual void	DetachChildAt(UInt32 i, NiPointer<NiAVObject>& arResult);
	virtual void	DetachChildAtAlt(UInt32 i);
	virtual void	SetAt(UInt32 i, NiAVObject* apChild, NiPointer<NiAVObject>& arResult);
	virtual void	SetAtAlt(UInt32 i, NiAVObject* apChild);
	virtual void	UpdateUpwardPass();

	NiTArray <NiAVObject *>		m_children;	// 09C

	void SetAlphaRecurse(float alpha) { CdeclCall(0x90A1C0, this, alpha); };

};	// 0AC

static_assert(sizeof(NiNode) == 0xAC);

class BSNiNode : public NiNode {
public:
	virtual ~BSNiNode();
	virtual void ReparentSkinInstances(NiNode* apNode, NiAVObject* apParent);
};

class BSMultiBoundRoom;
class BSMultiBound;

class BSMultiBoundShape : public NiObject {
public:
	enum CullResult {
		BS_CULL_UNTESTED = 0,
		BS_CULL_VISIBLE = 1,
		BS_CULL_CULLED = 2,
		BS_CULL_OCCLUDED = 3,
	};

	enum ShapeType {
		BSMB_SHAPE_NONE = 0,
		BSMB_SHAPE_AABB = 1,
		BSMB_SHAPE_OBB = 2,
		BSMB_SHAPE_SPHERE = 3,
		BSMB_SHAPE_CAPSULE = 4,
	};

	enum IntersectResult
	{
		BS_INTERSECT_NONE = 0,
		BS_INTERSECT_PARTIAL = 1,
		BS_INTERSECT_CONTAINSTARGET = 2
	};

	virtual ShapeType		GetType() const;
	virtual double			GetRadius() const;
	virtual IntersectResult	CheckBSBound(BSMultiBound& arTargetBound) const;
	virtual IntersectResult	CheckBound(NiBound& arTargetBound) const;
	virtual bool			WithinFrustum(NiFrustumPlanes& arPlanes) const;
	virtual bool			CompletelyWithinFrustum(NiFrustumPlanes& arPlanes) const;
	virtual void			GetNiBound(NiBound& arBound) const;
	virtual void			CreateDebugGeometry(NiLines* apLines, NiTriShape* apGeometry, NiColorAlpha akColor);
	virtual UInt32			GetDebugGeomLineSize() const;
	virtual UInt32			GetDebugGeomShapeSize() const;
	virtual bool			GetPointWithin(NiPoint3& arPoint) const;
	virtual void			SetCenter(NiPoint3& arCenter);

	struct BoundVertices {
		NiPoint3 point[8];
	};

	CullResult eCullResult;

	inline void ResetCullResult() {
		eCullResult = BS_CULL_UNTESTED;
	};
};

class BSMultiBoundAABB : public BSMultiBoundShape {
public:
	BSMultiBoundAABB();
	virtual ~BSMultiBoundAABB();

	NiPoint3 Center;
	NiPoint3 HalfExtents;

	void GetVertices(BoundVertices* apVerts);
};

class BSMultiBoundOBB : public BSMultiBoundAABB {
public:
	BSMultiBoundOBB();
	virtual ~BSMultiBoundOBB();

	NiMatrix33 Axis;

	void GetVertices(BoundVertices* apPoints);
};

class BSMultiBound : public NiObject {
public:
	BSMultiBound();
	~BSMultiBound();

	virtual bool GetPointWithin(const NiPoint3& arPoint) const;
	virtual void Nullsub024(void*);

	UInt32							uiBoundFrameCount;
	NiPointer<BSMultiBoundShape>	spShape;
};

class BSMultiBoundNode : public BSNiNode {
public:
	BSMultiBoundNode();
	virtual ~BSMultiBoundNode();

	virtual BSMultiBoundRoom*	GetMultiBoundRoom();
	virtual bool				GetPointWithin(NiPoint3& akPoint);
	virtual UInt32				CheckBound(BSMultiBound*);
	virtual UInt32				CheckBoundAlt(NiBound*);

	NiPointer<BSMultiBound> spMultiBound;
	UInt32					uiCullingMode;
};

#if 0

// F0
class SceneGraph : public NiNode
{
public:
	SceneGraph();
	~SceneGraph();

	NiCamera			* camera;			// 0DC
	UInt32				unk0E0;				// 0E0
	NiCullingProcess	* cullingProcess;	// 0E4
	UInt32				unk0E8;				// 0E8
	float				cameraFOV;			// 0EC
};

STATIC_ASSERT(sizeof(SceneGraph) == 0x0F0);

// E0
class BSTempNodeManager : public NiNode
{
public:
	BSTempNodeManager();
	~BSTempNodeManager();

	// overrides Unk_18 and related to scan children for BSTempNodes
	// removes them? tests against unk0DC

	float	unk0DC;	// 0DC
};

// E0
class BSTempNode : public NiNode
{
public:
	BSTempNode();
	~BSTempNode();

	float	unk0DC;	// 0DC
};

// BSCellNode - in RTTI but unref'd

// E8
class BSClearZNode : public NiNode
{
public:
	BSClearZNode();
	~BSClearZNode();

	// in Draw, scan for BSShaderAccumulator on the renderer, set 21E0 to 0 before calling parent Draw, reset to 1 afterwards

	UInt8	unk0DC;	// 0DC
	UInt8	unk0DD;	// 0DD
	float	unk0E0;	// 0E0 - init'd to 283840
	float	unk0E4;	// 0E4 - init'd to 100
};

// F0
class BSFadeNode : public NiNode
{
public:
	BSFadeNode();
	~BSFadeNode();

	// overload Draw to do LOD-based fade

	UInt8	unk0DC;			// 0DC
	UInt8	unk0DD[3];		// 0DD
	float	fNearDistSqr;	// 0E0
	float	fFarDistSqr;	// 0E4
	float	fCurrentAlpha;	// 0E8
	UInt8	cMultType;		// 0EC
	UInt8	unk0ED[3];		// 0ED
};

// EC
class BSScissorNode : public NiNode
{
public:
	BSScissorNode();
	~BSScissorNode();

	// mess about with renderer in Draw (presumably to set scissor mode)

	UInt32	unk0DC;	// 0DC
	UInt32	unk0E0;	// 0E0
	UInt32	unk0E4;	// 0E4
	UInt32	unk0E8;	// 0E8
};

// BSTimingNode - unref'd

class Actor;

// 118
class BSFaceGenNiNode : public NiNode
{
public:
	BSFaceGenNiNode();
	~BSFaceGenNiNode();

	virtual NiObject *	GetAnimationData(void);
	virtual void		SetAnimationData(NiObject * obj);
	virtual float *		GetRotation(float * dst);
	virtual void		SetRotation(float * src);	// ticks bRotatedLastUpdate
	virtual bool		GetAnimationUpdate(void);
	virtual void		SetAnimationUpdate(bool arg);
	virtual bool		GetApplyRotationToParent(void);
	virtual void		SetApplyRotationToParent(bool arg);
	virtual bool		GetFixedNormals(void);
	virtual void		SetFixedNormals(bool arg);
	virtual void		Unk_31(void * arg);	// scans children, "could not find bone for the head node"

	// spAnimationData->Unk15(idx) returns emotion weighting (max 0x0C)
	// spAnimationData->Unk17(idx) returns movement weighting (max 0x10)
	// spAnimationData->Unk1A(idx) returns phoneme weighting (max 0x0F)

	NiObject	* spAnimationData;		// 0DC
	float		m_rotation[9];			// 0E0 - init'd to identity mtx
	UInt8		bForceBaseMorph;		// 104
	UInt8		bFixedNormals;			// 105
	UInt8		bAnimationUpdate;		// 106
	UInt8		bRotatedLastUpdate;		// 107
	UInt8		bApplyRotationToParent;	// 108
	UInt8		pad109[3];				// 109
	float		fLastTime;				// 10C - init'd to -1
	UInt8		bUsingLoResHead;		// 110
	UInt8		bIAmPlayerCharacter;	// 111
	UInt8		bIAmInDialouge;			// 112
	UInt8		pad113;					// 113
	Actor		* pActor;				// 114
};

// E4
class NiBillboardNode : public NiNode
{
public:
	NiBillboardNode();
	~NiBillboardNode();

	enum
	{
		kMode_AlwaysFaceCamera =	0,
		kMode_RotateAboutUp,
		kMode_RigidFaceCamera,
		kMode_AlwaysFaceCenter,
		kMode_RigidFaceCenter,
		kMode_BSRotateAboutUp,
		kMode_Mask =				7,

		kFlag_Unk3 =				1 << 3,
	};

	// update functions set kFlag_Unk3

	UInt16	flags;				// 0DC
	UInt8	pad0DE[2];			// 0DE
	float	m_lastUpdateTime;	// 0E0
};

// FC
class NiSwitchNode : public NiNode
{
public:
	NiSwitchNode();
	~NiSwitchNode();

	// only updates/displays/etc child with index m_iIndex
	// keeps track of the last time each child was updated to save work

	enum
	{
		kUpdateOnlyActive =	1 << 0,
	};

	UInt16				flags;			// 0DC
	UInt8				pad0DE[2];		// 0DE
	UInt32				m_iIndex;		// 0E0 - active child idx
	float				lastUpdateTime;	// 0E4
	UInt32				curUpdateIdx;	// 0E8
	NiTArray <UInt32>	childUpdateIdx;	// 0EC - last update idx for each child
};

// 104
class NiLODNode : public NiSwitchNode
{
public:
	NiLODNode();
	~NiLODNode();

	NiLODData	* lodData;	// 0FC
	UInt8		unk100;		// 100
	UInt8		pad101[3];	// 101
};

// NiBSLODNode - unref'd

// E0
class NiSortAdjustNode : public NiNode
{
public:
	NiSortAdjustNode();
	~NiSortAdjustNode();

	UInt32	unk0DC;	// 0DC
};

// FC
class NiBSPNode : public NiNode
{
public:
	NiBSPNode();
	~NiBSPNode();

	NiPlane	m_kModelPlane;	// 0DC
	NiPlane	m_kWorldPlane;	// 0EC
};

// 130
class ShadowSceneNode : public NiNode
{
public:
	ShadowSceneNode();
	~ShadowSceneNode();

	NiObject			* unk0DC;	// 0DC
	UInt16				unk0E0;		// 0E0
	UInt8				pad0E2[2];	// 0E2
	NiTPointerList <ShadowSceneLight *>	lights;		// 0E4
	NiTPointerList <ShadowSceneLight *>	lights2;	// 0F4
	UInt32				pad104;		// 104
	UInt32				unk108;		// 108
	UInt32				unk10C;		// 10C
	ShadowSceneLight	* unk110;	// 110
	ShadowSceneLight	* unk114;	// 114
	UInt32				unk118;		// 118
	UInt8				unk11C;		// 11C
	UInt8				pad11D[3];	// 11D
	NiObject			* unk120;	// 120
	UInt32				unk124;		// 124
	UInt32				unk128;		// 128
	UInt8				unk12C;		// 12C
	UInt8				pad12D[3];	// 12D
};

#endif

// 124
class NiCamera : public NiAVObject
{
public:
	NiCamera();
	~NiCamera();

	UInt32		unk0AC[(0xEC - 0xAC) >> 2];	// 0AC
	NiFrustum	m_kViewFrustum;				// 0EC
	float		m_fMinNearPlaneDist;		// 108
	float		m_fMaxFarNearRatio;			// 10C
	NiViewport	m_kPort;					// 110
	float		unk120;						// 120

	bool WindowPointToRay(int x, int y, NiPoint3* origin, NiPoint3* direction, float width = 0.0F)
	{
		return ThisCall<bool>(0x817480, this, x, y, origin, direction, width);
	}
};

#if 0

// 150
class BSCubeMapCamera : public NiCamera
{
public:
	BSCubeMapCamera();
	~BSCubeMapCamera();

	// 1C
	class Str14C
	{
	public:
		Str14C();
		~Str14C();

		NiTPointerList <BSImageSpaceShader *>	shaders;	// 00
		NiObject	* unk010;	// 10
		NiObject	* unk014;	// 14
		UInt32		unk018;		// 18
	};

	UInt32		unk124;			// 124
	NiObject	* unk128[6];	// 128
	NiObject	* unk140;		// 140
	UInt32		pad144;			// 144
	NiObject	* unk148;		// 148
	Str14C		* unk14C;		// 14C
};

// 144
class NiScreenSpaceCamera : public NiCamera
{
public:
	NiScreenSpaceCamera();
	~NiScreenSpaceCamera();

	NiTArray <NiScreenPolygon *>	polygons;	// 124
	NiTArray <NiScreenTexture *>	textures;	// 134
};

// C0
class NiGeometry : public NiAVObject
{
public:
	NiGeometry();
	~NiGeometry();

	virtual void	Render(NiRenderer * arg);
	virtual void	Unk_22(NiRenderer * arg);
	virtual void	SetGeomData(NiObject * obj);
	virtual void	Unk_24(void);		// geomData->Unk_15()
	virtual void	Unk_25(NiRenderer * arg);

	NiPropertyState			* unk0AC;	// 0AC
	NiDynamicEffectState	* unk0B0;	// 0B0
	NiGeometryData			* geomData;	// 0B4
	NiObject				* skinData;	// 0B8
	NiObject				* shader;	// 0BC
};

// C0
class NiLines : public NiGeometry
{
public:
	NiLines();
	~NiLines();
};

// C0
class NiTriBasedGeom : public NiGeometry
{
public:
	NiTriBasedGeom();
	~NiTriBasedGeom();

	// use this API for SUPER MAX SPEED
	virtual void	GetTri(UInt32 idx, NiVector3 ** a, NiVector3 ** b, NiVector3 ** c) = 0;
};

// C0
class NiTriShape : public NiTriBasedGeom
{
public:
	NiTriShape();
	~NiTriShape();
};

// D0
class BSScissorTriShape : public NiTriShape
{
public:
	BSScissorTriShape();
	~BSScissorTriShape();

	UInt32	unk0C0;	// 0C0
	UInt32	unk0C4;	// 0C4
	UInt32	unk0C8;	// 0C8
	UInt32	unk0CC;	// 0CC
};

// C0
class NiScreenElements : public NiTriShape
{
public:
	NiScreenElements();
	~NiScreenElements();
};

// C0
class NiScreenGeometry : public NiTriShape
{
public:
	NiScreenGeometry();
	~NiScreenGeometry();
};

// D4
class TallGrassTriShape : public NiTriShape
{
public:
	TallGrassTriShape();
	~TallGrassTriShape();

	UInt16		instanceCount;	// 0C0
	UInt8		pad0C2[2];		// 0C2
	NiSphere	pLocalBound;	// 0C4
};

// C0
class NiTriStrips : public NiTriBasedGeom
{
public:
	NiTriStrips();
	~NiTriStrips();
};

// D4
class TallGrassTriStrips : public NiTriStrips
{
public:
	TallGrassTriStrips();
	~TallGrassTriStrips();

	virtual void		SetLocalBound(float x, float y, float z, float radius);
	virtual void		SetInstanceCount(UInt16 count);
	virtual NiSphere *	GetLocalBound(void);

	UInt16		instanceCount;	// 0C0
	UInt8		pad0C2[2];		// 0C2
	NiSphere	pLocalBound;	// 0C4
};

// C0
class NiParticles : public NiGeometry
{
public:
	NiParticles();
	~NiParticles();
};

// F0
class NiParticleSystem : public NiParticles
{
public:
	NiParticleSystem();
	~NiParticleSystem();

	virtual void	Unk_26(float arg);
	virtual void	Update(float time);

	UInt8	inWorldSpace;		// 0C0
	UInt8	pad0C1[3];			// 0C1
	NiTPointerList <NiPSysModifier *>	modifiers;		// 0C4
	NiTPointerMap <NiPSysModifier *>	modifierMap;	// 0D4 - wrong, should be NiTStringPointerMap <NiPSysModifier *>
	UInt8	unk0E4;				// 0E4
	UInt8	pad0E5[3];			// 0E5
	float	systemUpdateTime;	// 0E8
	UInt8	unk0EC;				// 0EC
	UInt8	pad0ED[3];			// 0ED
};

// F8
class NiMeshParticleSystem : public NiParticleSystem
{
public:
	NiMeshParticleSystem();
	~NiMeshParticleSystem();

	float	transformUpdateTime;	// 0F0
	UInt8	unk0F4;					// 0F4
	UInt8	unk0F5[3];				// 0F5
};

// C8
class NiParticleMeshes : public NiParticles
{
public:
	NiParticleMeshes();
	~NiParticleMeshes();

	float	transformUpdateTime;	// 0C0
	UInt8	unk0C4;					// 0C4
	UInt32	unk0C5[3];				// 0C5
};

#endif
class NiWindow;
class ShadowSceneLight;
class BSShaderAccumulator;
class BGSTextureUseMap;

class BSShaderProperty : public NiShadeProperty {
public:
	BSShaderProperty();
	virtual ~BSShaderProperty();

	enum BSShaderFlags {
		Specular					= 0x1,
		Skinned						= 0x2,
		LowDetail					= 0x4,
		Vertex_Alpha				= 0x8,
		Motion_Blur					= 0x10,
		Single_Pass					= 0x20,
		Empty						= 0x40,
		Environment_Mapping			= 0x80,
		Alpha_Texture				= 0x100,
		Z_Prepass					= 0x200,
		FaceGen						= 0x400,
		Parallax_Shader				= 0x800,
		Model_Space_Normals			= 0x1000,
		Non_Projective_Shadows		= 0x2000,
		Landscape					= 0x4000,
		Refraction					= 0x8000,
		Fire_Refraction				= 0x10000,
		Eye_Environment_Mapping		= 0x20000,
		Hair						= 0x40000,
		Dynamic_Alpha				= 0x80000,
		Localmap_Hide_Secret		= 0x100000,
		Window_Environment_Mapping	= 0x200000,
		Tree_Billboard				= 0x400000,
		Shadow_Frustum				= 0x800000,
		Multiple_Textures			= 0x1000000,
		Remappable_Textures			= 0x2000000,
		Decal_Single_Pass			= 0x4000000,
		Dynamic_Decal_Single_Pass	= 0x8000000,
		Parallax_Occlusion			= 0x10000000,
		External_Emittance			= 0x20000000,
		Shadow_Map					= 0x40000000,
		ZBuffer_Test				= 0x80000000,
	};

	enum BSShaderFlags2 {
		ZBuffer_Write					= 0x1,
		LOD_Landscape					= 0x2,
		LOD_Building					= 0x4,
		No_Fade							= 0x8,
		Refraction_Tint					= 0x10,
		Vertex_Colors					= 0x20,
		_1st_person						= 0x40,
		_1st_Light_is_Point_Light		= 0x80,
		_2nd_Light						= 0x100,
		_3rd_Light						= 0x200,
		Vertex_Lighting					= 0x400,
		Uniform_Scale					= 0x800,
		Fit_Slope						= 0x1000,
		Billboard_and_Envmap_Light_Fade = 0x2000,
		No_LOD_Land_Blend				= 0x4000,
		Envmap_Light_Fade				= 0x8000,
		Wireframe						= 0x10000,
		VATS_Selection					= 0x20000,
		Show_in_Local_Map				= 0x40000,
		Premult_Alpha					= 0x80000,
		Skip_Normal_Maps				= 0x100000,
		Alpha_Decal						= 0x200000,
		No_Transparency_Multisampling	= 0x400000,
		stinger_prop					= 0x800000,
		Unknown3						= 0x1000000,
		Unknown4						= 0x2000000,
		Unknown5						= 0x4000000,
		Unknown6						= 0x8000000,
		Unknown7						= 0x10000000,
		Unknown8						= 0x20000000,
		Unknown9						= 0x40000000,
		Wall_RealTimeEnv				= 0x80000000,
	};

	class RenderPass {
	public:
		enum AccumMode : UInt8 {
			ACCUM_NONE				= 0,
			ACCUM_ALPHA_BLEND		= 1,
			ACCUM_UNK_2				= 2,
			ACCUM_PARTICLE			= 3,
			ACCUM_DECAL_SINGLE		= 4,
			ACCUM_DYN_DECAL_SINGLE	= 5,
			ACCUM_REFRACTION		= 6,
			ACCUM_REFRACTION_CLEAR	= 7,
			ACCUM_UNK_8				= 8,
			ACCUM_UNK_9				= 9,
			ACCUM_LOD				= 10,
			ACCUM_UNK_11			= 11,
			ACCUM_UNK_12			= 12,
			ACCUM_UNK_13			= 13,
			ACCUM_ALPHA_FADE		= 14,
		};

		NiGeometry*			pGeometry;
		UInt16				usPassEnum;
		AccumMode			eAccumulationHint;
		bool				bIsFirst;
		bool				bNoFog;
		UInt8				ucNumLights;
		UInt8				ucMaxNumLights;
		UInt8				cCurrLandTexture;
		ShadowSceneLight**	ppSceneLights;
	};

	class RenderPassArray;

	virtual void						CopyTo(BSShaderProperty* apTarget);
	virtual void						CopyData(BSShaderProperty* apTarget);
	virtual void						SetupGeometry(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPasses(const NiGeometry* apGeometry, const UInt32 auiEnabledPasses, UInt16* apusPassCount, const UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
	virtual UInt16						GetNumberofPasses(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPassArray4C();
	virtual RenderPass*					GetDepthPass(NiGeometry* apGeometry);
	virtual BSShaderProperty*			PickShader(NiGeometry* apGeometry, UInt32 unk0 = 0, UInt32 unk2 = 0);
	virtual NiSourceTexture*			GetDiffuseTexture() const;
	virtual RenderPassArray*			GetWaterDepthPass(NiGeometry* apGeometry);
	virtual void						CountTextures(void* apCountFunc, BGSTextureUseMap* apTexMap) const;
	virtual void						PrecacheTextures() const;

	UInt32							ulFlags[2];
	float							fAlpha;
	float							fFadeAlpha;
	float							fEnvMapScale;
	float							fLODFade;
	UInt32							iLastRenderPassState;
	RenderPassArray*				pRenderPassArray;
	RenderPassArray*				pRenderPassArray_depthMap;
	RenderPassArray*				pRenderPassArray_constAlpha;
	RenderPassArray*				pRenderPassArray_localMap;
	RenderPassArray*				pRenderPassArray_unk4C;
	RenderPassArray*				pRenderPassArray_waterDepth;
	RenderPassArray*				pRenderPassArray_silhouette;
	UInt32							uiShaderIndex;
	float							fDepthBias;
};


class NiMaterialProperty : public NiProperty {
public:
	NiMaterialProperty();
	virtual ~NiMaterialProperty();

	SInt32		m_iIndex;
	NiColor		m_spec;
	NiColor		m_emit;
	NiColor*	m_pExternalEmittance;
	float		m_fShine;
	float		m_fAlpha;
	float		m_fEmitMult;
	UInt32		m_uiRevID;
	void*		m_pvRendererData;
};

class NiSkinInstance;
class BSShader;

class NiGeometry : public NiAVObject {
public:
	NiGeometry();
	virtual ~NiGeometry();

	virtual void	RenderImmediate(NiRenderer* pkRenderer);
	virtual void	RenderImmediateAlt(NiRenderer* pkRenderer);
	virtual void	SetModelData(NiGeometryData* pkModelData);
	virtual void	CalculateNormals();
	virtual void	CalculateConsistency(bool bTool);

	NiPropertyState		m_kProperties;
	NiGeometryData*		m_spModelData;
	NiSkinInstance*		m_spSkinInstance;
	BSShader*			m_pShader;
};