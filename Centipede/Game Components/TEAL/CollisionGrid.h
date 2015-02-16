// CollisionGrid.h
// Andre Berthiaume, June 2012
//
// Processed collision tests and event

#ifndef _CollisionGrid
#define _CollisionGrid

//#include "GameObject.h"
#include "Tools.h"
#include <stddef.h>
#include <math.h>
#include <limits>
#include "CollisionGridBase.h"
#include <list>

template <class T> 
class CollisionGrid : public CollisionGridBase
{
private:
	static CollisionGrid<T>* ColMgrInstance;															// Per-scene singleton

	CollisionGrid<T>()																					// Private constructor
	{ 
		candidateCount = 0; 
		candidates.clear();
		Colliders.clear();
		Cellgrid.clear();
		NeedsUpdating = false;

		totalCandidates = totalAABB = totalPositive = 0;

		// Add these lines
		minx = miny = std::numeric_limits<float>::max();		
		maxx = maxy = std::numeric_limits<float>::min();
	}	

	CollisionGrid<T>(const CollisionGrid<T>&);															// Prevent copy-construction
	CollisionGrid<T>& operator=(const CollisionGrid<T>&);												// Prevent assignment
	
	virtual void ClearInstance(){ColMgrInstance = NULL;}
	virtual void SetInstance( CollisionGridBase* instanceptr ){ ColMgrInstance = static_cast<CollisionGrid<T>*>(instanceptr); }
	virtual void DestroyInstance(){ delete ColMgrInstance; ColMgrInstance = NULL; }

	typedef std::list<T*> ColliderList;
	ColliderList Colliders; // List of objects to consider when processing pairwise collisions

	typedef Tools::NonClearingVector<T*> Cell;

	typedef std::vector< Cell > Hashgrid;
	typedef std::pair<T*,T*> Reflexivepair;

	Hashgrid Cellgrid;	
	unsigned int HashgridSize; 
	int sidegridcount;
	float minx, miny, maxx, maxy;
	bool NeedsUpdating; 

	template <class OT>
	static void CandidateTest( OT &target )
	{
		if ( CollisionGrid<T>::Instance().Colliders.size() < 1 ) return; // nothing to do with no colliding entities

		int n=0;
		CollisionGrid<T>::Instance().GetCollidingCandidate( target.getCollider()->getAABB(), CollisionGrid<T>::Instance().candidates, n);

		bool showbox = false;

		int i=0;
		for(Candidate::iterator it = CollisionGrid<T>::Instance().candidates.begin(); i < n; it++)
		{			
			if ( *it != NULL) 
			{
				showbox = true;
				//TEALShow::Line( target.getCollider()->getPosition(), (*it)->getCollider()->getPosition(), sf::Color(255,255,0,100) );

				SinglePairTest<OT>(&target, *it );
				CollisionGrid<T>::Instance().totalCandidates++;
			}

			i++;
		}

		//if (showbox) TEALShow::Box( target.getCollider()->getAABB(), sf::Color(255,255,0,100)  );
	}

	typedef std::list<T*> Candidate; 	
	Candidate candidates;
	unsigned int candidateCount;

	void GetCollidingCandidate( sf::FloatRect &r, Candidate &candidates, int &n)
	{
		// Early exit
		if ( CollisionGrid<T>::Instance().Colliders.size() == 0 ||
			 r.left + r.width < minx || maxx < r.left || r.top + r.height < miny || maxy < r.top) return;

		int cellminx, cellmaxx, cellminy, cellmaxy, a, b;

		cellminx = Clamp( Hash(minx, maxx, r.left, sidegridcount ), 0, sidegridcount - 1);
		cellmaxx = Clamp( Hash(minx, maxx, r.left + r.width, sidegridcount ), 0, sidegridcount - 1);
		cellminy = Clamp( Hash(miny, maxy, r.top, sidegridcount ), 0, sidegridcount - 1);
		cellmaxy = Clamp( Hash(miny, maxy, r.top + r.height, sidegridcount ), 0, sidegridcount - 1);

		Cell *cell;
		Candidate::iterator it = candidates.begin();
		n=0;
		for( a = cellminx; a <= cellmaxx; a++)
		{
			for( b = cellminy; b <= cellmaxy; b++)
			{
				cell = &Cellgrid[sidegridcount*a + b];
				for( unsigned int k=0 ; k <  cell->count ; k++) 
				{
					*it = cell->vector[k]; 
					n++;

					// We need to keep and eye on how full the list is
					if (n == candidates.size()) candidates.resize(candidates.size() + HashgridSize);
					it++;
				}
			}
		}

		// clean up duplicates
		it = candidates.begin();
  		for(int i = 0; i < n; i++)
		{
			(*it)->getCollider()->Marked = false; // resetting the marking flags
			it++;
		}

		it = candidates.begin(); 
		int i=0;
		while (i < n )
		{
			if ( (*it)->getCollider()->Marked )
				*it = NULL;
			else
				(*it)->getCollider()->Marked = true;
			it++;
			i++;
		}
	}

	void ReflexiveTest()
	{
		if ( CollisionGrid<T>::Instance().Colliders.size() < 1 ) return; // nothing to do with no collising entities

		// collect all the pairs in a set (to eliminate duplicates);
		std::set<Reflexivepair> CandidatePairs;
		std::vector<T*>::iterator member, othermember;

		for(unsigned int i=0; i <  CollisionGrid<T>::Instance().HashgridSize; i++)
		{
			if ( Cellgrid[i].count >= 2)	// no collisions otherwise
			{
				member = Cellgrid[i].vector.begin();
				for( unsigned int j = 0; j < Cellgrid[i].count - 1; j++) // stop one short of the last element
				{
					othermember = member;
					othermember++ ; // note: we start othermember one step ahead of member
					for( unsigned int k=j+1 ; k <  Cellgrid[i].count ; k++) 
					{	
						Reflexivepair p;
						if ( *member < *othermember) p = Reflexivepair(*member, *othermember);	// standardize pair ordering to catch
						else p = Reflexivepair(*othermember, *member);							// <a,b> versus <b,a>

						CandidatePairs.insert(p);
						othermember++;
					}
					member++;
				}
			}
		}

		totalCandidates += CandidatePairs.size();

		// now we process these pairs normally
		for ( std::set<Reflexivepair>::iterator it =  CandidatePairs.begin(); it != CandidatePairs.end(); it++)
		{
			SinglePairTest<T>((*it).first, (*it).second);

			//TEALShow::Line( (*it).first->getCollider()->getPosition(), (*it).second->getCollider()->getPosition(), sf::Color(255,255,255,80) );
		}
	}

	template<class OT>
	static void SinglePairTest(OT *target, T *member)
	{
		if ( target->getCollider()->getAABB().intersects( member->getCollider()->getAABB() ) ) // are the AABBs close enough?
		{
			CollisionGrid<T>::Instance().totalAABB++;
			//TEALShow::Box( target->getCollider()->getAABB(), sf::Color(255,255,255,40)  );
			//TEALShow::Box( member->getCollider()->getAABB(), sf::Color(255,255,255,40)  );

			if ( member->getCollider()->PairwiseCollisionTest( target->getCollider() ) )	// determines collision based on precision request of member and othermember
			{																				// If a collision takes place, perform both callbacks
				CollisionGrid<T>::Instance().totalPositive++;
				target->Collision( member ); 
				member->Collision( target );
			}	
		}
	}

	static int Hash(double min, float max, float val, int numcells){ return (int) ( (val-min)/(max-min)*numcells ); }
	static int Clamp(int v, int min, int max){ return std::max(std::min(max, v), 0);  }

public:
	static CollisionGrid<T>& Instance()																
	{
		if (! ColMgrInstance) 
		{
			ColMgrInstance = new CollisionGrid<T>;		
		}
		return *ColMgrInstance;
	};
	
	static void Register( T &go)
	{
		ColliderList *collist = &CollisionGrid<T>::Instance().Colliders;

		ColliderList::iterator it = std::find(collist->begin(), collist->end(), &go);
		if ( it == collist->end() ) 
		{
			collist->push_front( &go );
			CollisionGrid<T>::Instance().NeedsUpdating = true;		

			//Tools::insertSort( collist->begin(), collist->end());	// Insertion sort is faster for already sorted lists
		}
	}

	static void Deregister( T &go )
	{
		ColliderList *collist = &CollisionGrid<T>::Instance().Colliders;

		ColliderList::iterator it = std::find(collist->begin(), collist->end(), &go);
		if ( it != collist->end() ) 
		{
			collist->erase(it);
			CollisionGrid<T>::Instance().NeedsUpdating = true;
		}
	}

	sf::FloatRect GetGridRectangle(){ return sf::FloatRect(minx, miny, maxx-minx, maxy-miny); }

	virtual void PartitionSpace()
	{
		if ( !FixedType || NeedsUpdating)  // fixed objects only need to recalculate the partition on registration/deregistration
		{
			ColliderList *collist = &CollisionGrid<T>::Instance().Colliders;

			if (collist->size() < 1 ) return; // nothing to do with no collising entities

			// preparing the hasgrid for this iteration
			HashgridSize = collist->size();
			sidegridcount = (int) sqrt( (float) HashgridSize );	// this value could be adjusted based on circumstances, but it's a good starting point	
			Cellgrid.resize( HashgridSize ); // reserve?
			candidates.resize( HashgridSize );

			// Clean out data from the previous frame
			for(unsigned int i=0; i < HashgridSize; i++)
			{
				Cellgrid[i].vector.resize(HashgridSize);
				Cellgrid[i].count = 0;				// rather than using clear(). Overwriting the entries should avoid triggering the garbage collector.
			}

			// Compute the box that contains all members
			minx = miny = std::numeric_limits<float>::max();
			maxx = maxy = std::numeric_limits<float>::min();
			sf::FloatRect aabb;
			ColliderList::iterator it;
			for ( it = collist->begin(); it != collist->end(); it++)
			{
				aabb = (*it)->getCollider()->getAABB();
				minx = (aabb.left < minx) ? aabb.left : minx;
				miny = (aabb.top) < miny  ? aabb.top  : miny;
				maxx = (maxx < aabb.left + aabb.width) ? aabb.left + aabb.width  : maxx;
				maxy = (maxy < aabb.top + aabb.height) ? aabb.top + aabb.height : maxy;
			}
			minx -= 1; 	miny -= 1; maxx += 1; 	maxy += 1;		// Bumping out the overall limits to avoid hashing out of bounds later

			/* grid visualizing
			for (int i = 0; i <= sidegridcount; i++)
				TEALShow::Line( sf::Vector2f(minx + i*(maxx-minx)/sidegridcount, miny), sf::Vector2f(minx + i*(maxx-minx)/sidegridcount, maxy), sf::Color(255,255,255,80) );
			for (int i = 0; i <= sidegridcount; i++)
				TEALShow::Line( sf::Vector2f(minx, miny + i*(maxy-miny)/sidegridcount), sf::Vector2f(maxx, miny + i*(maxy-miny)/sidegridcount), sf::Color(255,255,255,80) );
			//TEALShow::Text( "Totals: Candidates: " + Tools::ToString(totalCandidates) + "\tAABB: " + Tools::ToString(totalAABB) + "\tPositive: " + Tools::ToString(totalPositive) );
			//totalCandidates = totalAABB = totalPositive = 0;
			//*/

			// Fill the hashgrid's cell with the gameobjects is contains
			int cellminx, cellmaxx, cellminy, cellmaxy, a, b, num = 0;
			for (it = collist->begin(); it != collist->end(); it++)
			{
				// one gameobject needs to be added to any cells it overlaps
				aabb = (*it)->getCollider()->getAABB();
				cellminx = Hash(minx, maxx, aabb.left, sidegridcount );
				cellmaxx = Hash(minx, maxx, aabb.left + aabb.width, sidegridcount );
				cellminy = Hash(miny, maxy, aabb.top, sidegridcount );
				cellmaxy = Hash(miny, maxy, aabb.top + aabb.height, sidegridcount );

				for( a = cellminx; a <= cellmaxx; a++)
					for( b = cellminy; b <= cellmaxy; b++)
						Cellgrid[sidegridcount*a + b].insert( *it ); 
			}

			NeedsUpdating = false;
		}

		if (ReflexiveCollision) ReflexiveTest();
	}

	static Candidate* GetCollidingCandidates( sf::FloatRect &r, int &n)
	{ 
		Instance().GetCollidingCandidate(r, Instance().candidates, n); 

		return &Instance().candidates;
	}

	template < class OT >
	static void CollisionTest()
	{
		  std::list<OT*> *others;
		  int othercount = 0;

		  others = CollisionGrid<OT>::Instance().GetCollidingCandidates( Instance().GetGridRectangle(), othercount);
		  
		  int i=0;
		  for( std::list<OT*>::iterator it = others->begin(); i < othercount; it++)
		  {
			  if ( *it != NULL ) CandidateTest<OT>( **it );
			  i++;
		  }
	}

	// Used for visualizing collision work
	int totalCandidates, totalAABB, totalPositive;

};

template <class T>
CollisionGrid<T>* CollisionGrid<T>::ColMgrInstance = NULL;

#endif _CollisionGrid