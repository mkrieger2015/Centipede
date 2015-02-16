// Scene.h
// Andre Berthiaume, Juen 2012
//
// Base scene class

#ifndef _Scene
#define _Scene

#include "AlarmManager.h"
#include "InputEventManager.h"
#include "CollisionManager.h"

// Forward declarations
class GameObject;

class Scene
{
private:
	// GameObject management
	typedef std::list<GameObject*> GameObjectList;
	typedef std::map<int, GameObjectList > MapOfGameObjectLists;
	MapOfGameObjectLists updateListMap;
	MapOfGameObjectLists drawListMap;
	MapOfGameObjectLists GameObjectstoBeRemoved;
	void OperateOnAllGameObjects( MapOfGameObjectLists& map, void (*f)(GameObject*) );

	// Alarm and input event management
	AlarmManager MyAlarmManager;
	InputEventManager MyInputEventMgr;

	CollisionManager MyCollisionMgr;

	// these are required because lambdas are borked in VS 2010 according to https://connect.microsoft.com/VisualStudio/feedback/details/572138
	static void UpdateOneGameObject( GameObject* go);
	static void DrawOneGameObject( GameObject* go);
	static void DeleteOneGameObject( GameObject* go);
	static void DeleteOneMarkedGameObject( GameObject* go);
	static void FinalDeleteOrRelease( GameObject* go );

	void UpdateAllObjects();
	void DrawAllObjects();
	void DeleteMarkedObjects();

public:
	typedef void (*DeleteOperation)(GameObject*);

private:

	/// \brief Deletes the game object described by go.
	/// Default delete operation for a unmanaged GameObject being removed from a scene
	static void DeleteGameObject(GameObject* go);	

friend class GameObject; // needed to give access to the methods below
	void RegisterGameObject( GameObject* );
	void DeregisterGameObject( GameObject* go);
	AlarmManager& Alarms();
	InputEventManager& InputEvents();

friend class SceneManager; // needed to give access to the methods below
	void ProcessOneFrame();
	
public:
	Scene(){};
	~Scene();

	/// \brief Initializes the Scene with the new GameObjects and prepares the collision system
	/// \ingroup SCENE
	/// 
	/// This method is implemented in the derived classe. It will be called once when the scene is created and will create all 
	/// the necessary GameObject in the Scene as well as preparing the collision system.
	///  
	///  \b Example: the following code shows how to create 512 Bugs and 25 Blocks in the Scene where Collisions Between two Bugs and
	///  between a Bug and Block will be processed.
	///		\code
	///		void CrazyBugLevel::Initialize()
	///		{
	///			// Set up the needed game entities
	///			for(int i=0; i < 25; i++)
	///				new Block; // Each Block will chose a random position
	///
	///			for(int i=0; i< 512; i++)
	///				new Bug(); // Each Bug	will chose a random start position
	///
	///			// These are the collisions we care about
	///			CollisionTestPair<Bug,Block>();
	///			CollisionTestPair<Bug,Bug>();
	///		};
	///		\endcode
	virtual void Initialize();

	/// \brief Notifies the collision system that collision betweens these two types of GameObject should be tested.
	/// \ingroup SCENE 
	/// 
	/// <b>This is a _template function_ with no input parameters! The template parameters themselve _are_ the input! </b>
	/// 
	/// In order to optimize the collision processing, you specify exactly which pairwise test your game will need. 
	/// 
	/// \tparam GO1 Derived from GameObject.
	/// \tparam GO2 Derived from GameObject.
	/// 			
	///  \b Example: the following code shows how to create 512 Bugs and 25 Blocks in the Scene where Collisions Between two Bugs and
	///  between a Bug and Block will be processed.
	///		\code
	///		void CrazyBugLevel::Initialize()
	///		{
	///			// Set up the needed game entities
	///			for(int i=0; i < 25; i++)
	///				new Block; // Each Block will chose a random position
	///
	///			for(int i=0; i< 512; i++)
	///				new Bug(); // Each Bug	will chose a random start position
	///
	///			// These are the collisions we care about
	///			CollisionTestPair<Bug,Block>();
	///			CollisionTestPair<Bug,Bug>();
	///		};
	///		\endcode
	template < class GO1, class GO2 >
	void CollisionTestPair(){ MyCollisionMgr.CollisionPair<GO1,GO2>(); }

	void ChangeUpdateOrder(GameObject* go, int neworder );
	void ChangeDrawOrder(GameObject* go, int neworder );
};


#endif _Scene