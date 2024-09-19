#pragma once
#include "Utilities.h"

#if RUNTIME

#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
const UInt32 _NiTMap_Lookup = 0x00853130;
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
const UInt32 _NiTMap_Lookup = 0x00844740;
#else
#error
#endif

#else
const UInt32 _NiTMap_Lookup = 0;
#endif

typedef void* NiTMapIterator;

// 8
struct NiRTTI
{
	const char	* name;
	NiRTTI		* parent;
};

// C
struct NiVector3
{
	float	x, y, z;
};

// 10 - always aligned?
struct NiVector4
{
	float	x, y, z, w;
};

struct NiPoint3
{
	float x, y, z;

	static const NiPoint3 UNIT_X;
	static const NiPoint3 UNIT_Y;
	static const NiPoint3 UNIT_Z;
	static const NiPoint3 UNIT_ALL;
	static const NiPoint3 ZERO;

	NiPoint3& operator= (const NiPoint3& pt) {
		x = pt.x;
		y = pt.y;
		z = pt.z;
		return *this;
	};

	NiPoint3& operator= (const NiPoint3* pt) {
		x = pt->x;
		y = pt->y;
		z = pt->z;
		return *this;
	};

	NiPoint3 operator+ (const NiPoint3& pt) const { return NiPoint3(x + pt.x, y + pt.y, z + pt.z); };
	NiPoint3& operator+= (const NiPoint3& pt) {
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	};

	NiPoint3 operator- (const NiPoint3& pt) const { return NiPoint3(x - pt.x, y - pt.y, z - pt.z); };
	NiPoint3 operator- () const { return NiPoint3(-x, -y, -z); };
	NiPoint3& operator-= (const NiPoint3& pt) {
		x -= pt.x;
		y -= pt.y;
		z -= pt.z;
		return *this;
	};

	float operator* (const NiPoint3& pt) const { return x * pt.x + y * pt.y + z * pt.z; };

	NiPoint3 operator* (float fScalar) const { return NiPoint3(fScalar * x, fScalar * y, fScalar * z); };
	friend NiPoint3 operator* (float fScalar, const NiPoint3& pt) { return NiPoint3(fScalar * pt.x, fScalar * pt.y, fScalar * pt.z); };
	NiPoint3& operator*= (float fScalar) {
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		return *this;
	};



	NiPoint3 operator/ (float fScalar) const {
		float fInvScalar = 1.0f / fScalar;
		return NiPoint3(fInvScalar * x, fInvScalar * y, fInvScalar * z);
	};

	NiPoint3& operator/= (float fScalar) {
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;
		return *this;
	};
};

struct NiPoint4
{
	float x, y, z, r;
};

// 10 - always aligned?
struct NiQuaternion
{
	float	x, y, z, w;
};

// 24
struct NiMatrix33
{
	float	data[3][3];

	__forceinline float GetEntry(UInt32 uiRow, UInt32 uiCol) const {
		return data[uiRow][uiCol];
	}

	NiPoint3 operator*(const NiPoint3& pt) const {
		return NiPoint3(
			data[0][0] * pt.x + data[0][1] * pt.y + data[0][2] * pt.z,
			data[1][0] * pt.x + data[1][1] * pt.y + data[1][2] * pt.z,
			data[2][0] * pt.x + data[2][1] * pt.y + data[2][2] * pt.z);
	}

	NiMatrix33 operator* (const NiMatrix33& mat) const {
		NiMatrix33 result;
		result.data[0][0] =
			data[0][0] * mat.data[0][0] +
			data[0][1] * mat.data[1][0] +
			data[0][2] * mat.data[2][0];
		result.data[1][0] =
			data[1][0] * mat.data[0][0] +
			data[1][1] * mat.data[1][0] +
			data[1][2] * mat.data[2][0];
		result.data[2][0] =
			data[2][0] * mat.data[0][0] +
			data[2][1] * mat.data[1][0] +
			data[2][2] * mat.data[2][0];
		result.data[0][1] =
			data[0][0] * mat.data[0][1] +
			data[0][1] * mat.data[1][1] +
			data[0][2] * mat.data[2][1];
		result.data[1][1] =
			data[1][0] * mat.data[0][1] +
			data[1][1] * mat.data[1][1] +
			data[1][2] * mat.data[2][1];
		result.data[2][1] =
			data[2][0] * mat.data[0][1] +
			data[2][1] * mat.data[1][1] +
			data[2][2] * mat.data[2][1];
		result.data[0][2] =
			data[0][0] * mat.data[0][2] +
			data[0][1] * mat.data[1][2] +
			data[0][2] * mat.data[2][2];
		result.data[1][2] =
			data[1][0] * mat.data[0][2] +
			data[1][1] * mat.data[1][2] +
			data[1][2] * mat.data[2][2];
		result.data[2][2] =
			data[2][0] * mat.data[0][2] +
			data[2][1] * mat.data[1][2] +
			data[2][2] * mat.data[2][2];
		return result;
	}

	void MakeXRotation(float fAngle) {
		float sn = std::sin(fAngle);
		float cs = std::cos(fAngle);

		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = cs;
		data[1][2] = sn;
		data[2][0] = 0.0f;
		data[2][1] = -sn;
		data[2][2] = cs;
	}

	void MakeYRotation(float fAngle) {
		float sn = std::sin(fAngle);
		float cs = std::cos(fAngle);

		data[0][0] = cs;
		data[0][1] = 0.0f;
		data[0][2] = -sn;
		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[1][2] = 0.0f;
		data[2][0] = sn;
		data[2][1] = 0.0f;
		data[2][2] = cs;
	}

	void MakeZRotation(float fAngle) {
		float sn = std::sin(fAngle);
		float cs = std::cos(fAngle);

		data[0][0] = cs;
		data[0][1] = sn;
		data[0][2] = 0.0f;
		data[1][0] = -sn;
		data[1][1] = cs;
		data[1][2] = 0.0f;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = 1.0f;
	}

	void FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle) {
		NiMatrix33 kXRot, kYRot, kZRot;
		kXRot.MakeXRotation(fXAngle);
		kYRot.MakeYRotation(fYAngle);
		kZRot.MakeZRotation(fZAngle);
		*this = kXRot * (kYRot * kZRot);
	}
};

// 34
struct NiTransform
{
	NiMatrix33	rotate;		// 00
	NiPoint3	translate;	// 24
	float		scale;		// 30

	// 0x524C40
	inline NiPoint3 operator*(const NiPoint3& kPoint) const {
		return(((rotate * kPoint) * scale) + translate);
	};
};

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 1C
struct NiFrustum
{
	float	l;			// 00
	float	r;			// 04
	float	t;			// 08
	float	b;			// 0C
	float	n;			// 10
	float	f;			// 14
	UInt8	o;			// 18
	UInt8	pad19[3];	// 19
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// C
struct NiColor
{
	float	r;
	float	g;
	float	b;
};

// 10
struct NiColorAlpha
{
	float	r;
	float	g;
	float	b;
	float	a;

	NiColorAlpha(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};

	void Scale(float fScale) {
		r *= fScale;
		g *= fScale;
		b *= fScale;
		a *= fScale;
	};
};

// 10
struct NiPlane
{
	NiVector3	nrm;
	float		offset;
};

// 10
// NiTArrays are slightly weird: they can be sparse
// this implies that they can only be used with types that can be NULL?
// not sure on the above, but some code only works if this is true
// this can obviously lead to fragmentation, but the accessors don't seem to care
// weird stuff
template <typename T>
struct NiTArray
{
	void** _vtbl;		// 00
	T* data;			// 04
	UInt16	capacity;		// 08 - init'd to size of preallocation
	UInt16	firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16	numObjs;		// 0C - init'd to 0
	UInt16	growSize;		// 0E - init'd to size of preallocation

	T operator[](UInt32 idx) {
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T Get(UInt32 idx) { return (*this)[idx]; }

	UInt16 Length(void) { return firstFreeEntry; }
	void AddAtIndex(UInt32 index, T* item);	// no bounds checking
	void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed
};


#if RUNTIME

template <typename T> void NiTArray<T>::AddAtIndex(UInt32 index, T* item)
{
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	ThisCall(0x00869640, this, index, item);
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
	ThisCall(0x00869110, this, index, item);
#else
#error unsupported Oblivion version
#endif
}

template <typename T> void NiTArray<T>::SetCapacity(UInt16 newCapacity)
{
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	ThisCall(0x008696E0, this, newCapacity);
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
	ThisCall(0x00869190, this, newCapacity);
#else
#error unsupported Runtime version
#endif
}

#endif

// 18
// an NiTArray that can go above 0xFFFF, probably with all the same weirdness
// this implies that they make fragmentable arrays with 0x10000 elements, wtf
template <typename T>
class NiTLargeArray
{
public:
	NiTLargeArray();
	~NiTLargeArray();

	void	** _vtbl;		// 00
	T		* data;			// 04
	UInt32	capacity;		// 08 - init'd to size of preallocation
	UInt32	firstFreeEntry;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	numObjs;		// 10 - init'd to 0
	UInt32	growSize;		// 14 - init'd to size of preallocation

	T operator[](UInt32 idx) {
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T Get(UInt32 idx) { return (*this)[idx]; }

	UInt32 Length(void) { return firstFreeEntry; }
};

// 8
template <typename T>
struct NiTSet
{
	T		* data;		// 00
	UInt16	capacity;	// 04
	UInt16	length;		// 06
};

// 10
// this is a NiTPointerMap <UInt32, T_Data>
// todo: generalize key
template <typename T_Data>
class NiTPointerMap
{
public:
	NiTPointerMap();
	virtual ~NiTPointerMap();

	struct Entry
	{
		Entry	* next;
		UInt32	key;
		T_Data	* data;
	};

	// note: traverses in non-numerical order
	class Iterator
	{
		friend NiTPointerMap;

	public:
		Iterator(NiTPointerMap * table, Entry * entry = NULL, UInt32 bucket = 0)
			:m_table(table), m_entry(entry), m_bucket(bucket) { FindValid(); }
		~Iterator() { }

		T_Data *	Get(void);
		UInt32		GetKey(void);
		bool		Next(void);
		bool		Done(void);

	private:
		void		FindValid(void);

		NiTPointerMap	* m_table;
		Entry		* m_entry;
		UInt32		m_bucket;
	};

	virtual UInt32	KeyToHashIndex(UInt32 key);
	virtual bool	IsKeysEqual(UInt32 lhs, UInt32 rhs);
	virtual void	Fn_03(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// assign to entry
	virtual void	Fn_04(UInt32 arg);
	virtual void	Fn_05(void);	// locked operations
	virtual void	Fn_06(void);	// locked operations

	T_Data *	Lookup(UInt32 key);
	bool		Insert(Entry* nuEntry);

//	void	** _vtbl;		// 0
	UInt32	m_numBuckets;	// 4
	Entry	** m_buckets;	// 8
	UInt32	m_numItems;		// C

	NiTMapIterator GetFirstPos() const {
		for (UInt32 i = 0; i < m_numBuckets; i++) {
			if (m_buckets[i])
				return m_buckets[i];
		}
		return 0;
	}

	void GetNext(NiTMapIterator& pos, UInt32& key, T_Data*& val) {
		Entry* pItem = (Entry*)pos;

		key = pItem->key;
		val = pItem->data;

		if (pItem->next) {
			pos = pItem->next;
			return;
		}

		UInt32 i = KeyToHashIndex(pItem->key);
		for (++i; i < m_numBuckets; i++)
		{
			pItem = m_buckets[i];
			if (pItem)
			{
				pos = pItem;
				return;
			}
		}

		pos = 0;
	}
};

template <typename T_Data>
T_Data * NiTPointerMap <T_Data>::Lookup(UInt32 key)
{
	for(Entry * traverse = m_buckets[key % m_numBuckets]; traverse; traverse = traverse->next)
		if(traverse->key == key)
			return traverse->data;
	
	return NULL;
}

template <typename T_Data>
bool NiTPointerMap<T_Data>::Insert(Entry* nuEntry)
{
	// game code does not appear to care about ordering of entries in buckets
	UInt32 bucket = nuEntry->key % m_numBuckets;
	Entry* prev = NULL;
	for (Entry* cur = m_buckets[bucket]; cur; cur = cur->next) {
		if (cur->key == nuEntry->key) {
			return false;
		}
		else if (!cur->next) {
			prev = cur;
			break;
		}
	}

	if (prev) {
		prev->next = nuEntry;
	}
	else {
		m_buckets[bucket] = nuEntry;
	}

	m_numBuckets++;
	return true;
}

template <typename T_Data>
T_Data * NiTPointerMap <T_Data>::Iterator::Get(void)
{
	if(m_entry)
		return m_entry->data;

	return NULL;
}

template <typename T_Data>
UInt32 NiTPointerMap <T_Data>::Iterator::GetKey(void)
{
	if(m_entry)
		return m_entry->key;

	return 0;
}

template <typename T_Data>
bool NiTPointerMap <T_Data>::Iterator::Next(void)
{
	if(m_entry)
		m_entry = m_entry->next;

	while(!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_buckets[m_bucket];
	}

	return m_entry != NULL;
}

template <typename T_Data>
bool NiTPointerMap <T_Data>::Iterator::Done(void)
{
	return m_entry == NULL;
}

template <typename T_Data>
void NiTPointerMap <T_Data>::Iterator::FindValid(void)
{
	// validate bucket
	if(m_bucket >= m_table->m_numBuckets) return;

	// get bucket
	m_entry = m_table->m_buckets[m_bucket];

	// find non-empty bucket
	while(!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_buckets[m_bucket];
	}
}



// 10
// todo: NiTPointerMap should derive from this
// cleaning that up now could cause problems, so it will wait
template <typename T_Key, typename T_Data>
class NiTMapBase
{
public:
	NiTMapBase();
	~NiTMapBase();

	struct Entry
	{
		Entry	* next;	// 000
		T_Key	key;	// 004
		T_Data	data;	// 008
	};

	virtual NiTMapBase<T_Key, T_Data>*	Destructor(bool doFree);						// 000
	virtual UInt32						KeyToHashIndex(T_Key key);								// 001
	virtual void						IsKeysEqual(T_Key key1, T_Key key2);					// 002
	virtual void						SetValue(Entry entry, T_Key key, T_Data data);	// 003
	virtual	void						ClearValue(void * arg0);							// 004
	virtual	void						NewItem(void);									// 005
	virtual	void						DeleteItem();										// 006

	UInt32	numBuckets;	// 4
	Entry	** buckets;	// 8
	UInt32	numItems;	// C

	DEFINE_MEMBER_FN_LONG(NiTMapBase, Lookup, bool, _NiTMap_Lookup, T_Key key, T_Data * dataOut);

	NiTMapIterator GetFirstPos() const {
		for (UInt32 i = 0; i < numBuckets; i++) {
			if (buckets[i])
				return buckets[i];
		}
		return 0;
	}

	void GetNext(NiTMapIterator& pos, T_Key& key, T_Data& val) {
		Entry* pItem = (Entry*) pos;

		key = pItem->key;
		val = pItem->data;

		if (pItem->next) {
			pos = pItem->next;
			return;
		}

		UInt32 i = KeyToHashIndex(pItem->key);
		for (++i; i < numBuckets; i++)
		{
			pItem = buckets[i];
			if (pItem)
			{
				pos = pItem;
				return;
			}
		}

		pos = 0;
	}
};

// 14
template <typename T_Data>
class NiTStringPointerMap : public NiTPointerMap <T_Data>
{
public:
	NiTStringPointerMap();
	~NiTStringPointerMap();

	UInt32	unk010;
};

// not sure how much of this is in NiTListBase and how much is in NiTPointerListBase
// 10
template <typename T>
class NiTListBase
{
public:
	NiTListBase();
	~NiTListBase();

	struct Node
	{
		Node	* next;
		Node	* prev;
		T		* data;
	};

	virtual void	Destructor(void);
	virtual Node *	AllocateNode(void);
	virtual void	FreeNode(Node * node);

//	void	** _vtbl;	// 000
	Node	* start;	// 004
	Node	* end;		// 008
	UInt32	numItems;	// 00C
};

// 10
template <typename T>
class NiTPointerListBase : public NiTListBase <T>
{
public:
	NiTPointerListBase();
	~NiTPointerListBase();
};

// 10
template <typename T>
class NiTPointerList : public NiTPointerListBase <T>
{
public:
	NiTPointerList();
	~NiTPointerList();
};

// 4
template <typename T>
class NiPointer
{
public:
	NiPointer(T *init) : data(init)		{	}

	T	* data;

	const T&	operator *() const { return *data; }
	T&			operator *() { return *data; }

	T* operator->() const { return data; }

	operator const T*() const { return data; }
	operator T*() { return data; }

	__forceinline NiPointer<T>& operator =(const NiPointer& ptr) {
		if (data != ptr.data) {
			if (data)
				data->DecRefCount();
			data = ptr.data;
			if (data)
				data->IncRefCount();
		}
		return *this;
	}

	__forceinline NiPointer<T>& operator =(T* pObject) {
		if (data != pObject) {
			if (data)
				data->DecRefCount();
			data = pObject;
			if (data)
				data->IncRefCount();
		}
		return *this;
	}

	__forceinline bool operator==(T* apObject) const { return (data == apObject); }

	__forceinline bool operator==(const NiPointer& ptr) const { return (data == ptr.data); }

	__forceinline operator bool() const { return data != nullptr; }
};

// 14
template <typename T>
class BSTPersistentList
{
public:
	BSTPersistentList();
	~BSTPersistentList();

	virtual void	Destroy(bool destroy);

//	void	** _vtbl;	// 00
	UInt32	unk04;		// 04
	UInt32	unk08;		// 08
	UInt32	unk0C;		// 0C
	UInt32	unk10;		// 10
};
