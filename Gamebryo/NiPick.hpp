#pragma once

#include "NiColorA.hpp"
#include "NiMemObject.hpp"

class NiPick : public NiMemObject {
public:
	NiPick(uint32_t auiResultsArraySize = 0, uint32_t auiResultsArrayGrowBy = 8);
	~NiPick();

	class Record : public NiMemObject {
	public:
		Record(NiAVObject* apObject);

		NiAVObject*	m_spObject;
		NiNode*		m_spProxyParent;
		NiPoint3		m_intersect;
		float			m_fDistance;
		uint16_t		m_usTriangleIndex;
		uint16_t		m_usVertexIndex[3];
		NiPoint2		m_texture;
		NiPoint3		m_normal;
		NiColorA		m_color;

		void Initialize(NiAVObject* apObject);
	};

	class Results : public NiTArray<Record*> {
	public:
		Results(uint32_t auiInitialSize, uint32_t auiGrowBy);
		~Results();

		uint32_t m_uiValidResults;
	};

	enum PickType {
		FIND_ALL = 0,
		FIND_FIRST = 1,
	};

	enum SortType {
		SORT = 0,
		NO_SORT = 1,
	};

	enum IntersectType {
		BOUND_INTERSECT = 0,
		TRIANGLE_INTERSECT = 1,
	};

	enum CoordinateType {
		MODEL_COORDINATES = 0,
		WORLD_COORDINATES = 1,
	};

	PickType		m_ePickType;
	SortType		m_eSortType;
	IntersectType	m_eIntersectType;
	CoordinateType	m_eCoordinateType;
	bool			m_bFrontOnly;
	bool			m_bObserveAppCullFlag;
	NiAVObject*	m_spRoot;
	Results			m_pickResults;
	Record* m_pLastAddedRecord;
	bool			m_bReturnTexture;
	bool			m_bReturnNormal;
	bool			m_bReturnSmoothNormal;
	bool			m_bReturnColor;

	void SetTarget(NiAVObject* apTarget);
	void RemoveTarget();

	bool PickObjects(const NiPoint3& arOrigin, const NiPoint3& arDir, bool abAppend = false);

	void ClearResultsArray(bool abFree);
};

ASSERT_SIZE(NiPick::Record, 0x44);
ASSERT_SIZE(NiPick::Results, 0x14);
ASSERT_SIZE(NiPick, 0x34);