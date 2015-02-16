// CollisionManager
// Andre Berthiaume, July 2012
//

#ifndef _CollisionManager
#define _CollisionManager

#include <set>
#include "CollisionGrid.h"

class CollisionManager
{

public:
	typedef std::set<CollisionGridBase*> ColMgrSet;
	
	typedef void (*ColTestFnc)();

	typedef std::pair<CollisionGridBase*, ColTestFnc> CollisionPairs;
	typedef std::set<ColTestFnc> CollisionFncs;

private:
	ColMgrSet CollisionGridCollection;
	CollisionFncs PairwiseTestFncs;

public:

	CollisionManager();	
	~CollisionManager();

	void ClearCollisionGridInstances();
	void ReconnectCollisionGridInstances();

	void ProcessCollisionEvents();

	template <class GO>
	void SetAsFixed( bool b = true ){ CollisionGrid<GO>::Instance().SetFixedType(b); }

	template < class GO1, class GO2 >
	void CollisionPair()
	{
		CollisionGrid<GO1> *Gridptr1 = &CollisionGrid<GO1>::Instance();
		CollisionGrid<GO2> *Gridptr2 = &CollisionGrid<GO2>::Instance();

		// insert the grid (if not already present) and get a iterator pointing to it
		std::pair<ColMgrSet::iterator, bool> id1 = CollisionGridCollection.insert( Gridptr1 );
		std::pair<ColMgrSet::iterator, bool> id2 = CollisionGridCollection.insert( Gridptr2 );

		if ( *(id1.first) == *(id2.first) ) // collisions against your own kind are processed seperatly
			(*(id1.first))->SetReflexiveCollision(true);
		else 
		{	
			// Test that we haven't already added this pairwise test
			if ( PairwiseTestFncs.find( &CollisionGrid<GO1>::CollisionTest<GO2> ) != PairwiseTestFncs.end() )
				ConsoleMsg::ShowError("Pairwise test already added." );
			else if ( PairwiseTestFncs.find( &CollisionGrid<GO2>::CollisionTest<GO1> ) != PairwiseTestFncs.end() )
				ConsoleMsg::ShowError("Pairwise test already added in reverse order." );
			else
				PairwiseTestFncs.insert( &CollisionGrid<GO1>::CollisionTest<GO2> );
		}

	}

};

#endif _CollisionManager