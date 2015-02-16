// CollisionManager
// Andre Berthiaume, July 2012
//

#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
	CollisionGridCollection.clear();
	PairwiseTestFncs.clear();
}

CollisionManager::~CollisionManager()
{
	// Clean up the various collision grid instances
	for( ColMgrSet::iterator it = CollisionGridCollection.begin(); it != CollisionGridCollection.end(); it++)
		(*it)->DestroyInstance();
}

void CollisionManager::ProcessCollisionEvents()
{
	// Partition space
	for( ColMgrSet::iterator it = CollisionGridCollection.begin(); it != CollisionGridCollection.end(); it++)
		(*it)->PartitionSpace();
	
	// Trigger the necessary events
	for( CollisionFncs::iterator it = PairwiseTestFncs.begin(); it != PairwiseTestFncs.end(); it++)
		(*it)();
}

void  CollisionManager::ClearCollisionGridInstances()
{
	for( ColMgrSet::iterator it = CollisionGridCollection.begin(); it != CollisionGridCollection.end(); it++)
		(*it)->ClearInstance();
}

void  CollisionManager::ReconnectCollisionGridInstances()
{
	for( ColMgrSet::iterator it = CollisionGridCollection.begin(); it != CollisionGridCollection.end(); it++)
		(*it)->SetInstance(*it);
}


