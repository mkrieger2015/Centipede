// CollisionGridBase
// Andre Berthiaume, July 2012

#ifndef _CollisionGridBase
#define _CollisionGridBase

class CollisionGridBase
{
protected:
	bool ReflexiveCollision;
	bool FixedType;

public:
	CollisionGridBase(){ ReflexiveCollision = false; FixedType = false; }

	virtual void ClearInstance(){};
	virtual void SetInstance( CollisionGridBase* instanceptr ){};
	virtual void DestroyInstance() =0;

	void SetReflexiveCollision(bool b){ ReflexiveCollision = b;  }
	void SetFixedType(bool b){ FixedType = b;  }

	virtual void PartitionSpace() =0;
};

#endif _CollisionGridBase