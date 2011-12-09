#include "actors.hpp"
#include <OgreSceneNode.h>
#include <components/nifogre/ogre_nif_loader.hpp>
#include "../mwworld/world.hpp"

using namespace Ogre;
using namespace MWRender;
using namespace NifOgre;

void Actors::setMwRoot(Ogre::SceneNode* root){
    mMwRoot = root;
}
Ogre::Entity* Actors::insertBoundedPart(const std::string &mesh, std::string bonename, Ogre::Entity* base){
    NIFLoader::load(mesh);
    Entity* ent = mRend.getScene()->createEntity(mesh);
	 
    base->attachObjectToBone(bonename, ent); 
    return ent;
				
}
void Actors::insertNPC(const MWWorld::Ptr& ptr){
    ESMS::LiveCellRef<ESM::NPC, MWWorld::RefData> *ref =
            ptr.get<ESM::NPC>();
        assert (ref->base != NULL);
		
        insertBegin(ptr, true, true);
		
		//Part selection on last character of the file string
		//  " Tri Chest
		//  * Tri Tail
		//  : Tri Left Foot
		//  < Tri Right Foot
		//  > Tri Left Hand
		//  ? Tri Right Hand
		//  | Normal

		//Mirroring Parts on second to last character
		//suffix == '*'
		//	vector = Ogre::Vector3(-1,1,1);
		//  suffix == '?'
		//	vector = Ogre::Vector3(1,-1,1);
		//  suffix == '<'
		//	vector = Ogre::Vector3(1,1,-1);



		std::string hairID = ref->base->hair;
        std::string headID = ref->base->head;
		std::string npcName = ref->base->name;

         std::string bodyRaceID = headID.substr(0, headID.find_last_of("head_") - 4);
		char secondtolast = bodyRaceID.at(bodyRaceID.length() - 2);
		bool female = tolower(secondtolast) == 'f';
		bool beast = bodyRaceID == "b_n_khajiit_m_" || bodyRaceID == "b_n_khajiit_f_" || bodyRaceID == "b_n_argonian_m_" || bodyRaceID == "b_n_argonian_f_";


        std::string smodel = "meshes\\base_anim.nif";
		if(beast)
			smodel = "meshes\\base_animkna.nif";

         Ogre::SceneNode* insert = ptr.getRefData().getBaseNode();
         assert(insert);

         NifOgre::NIFLoader::load(smodel);
    Entity *base = mRend.getScene()->createEntity(smodel);
    insert->attachObject(base);
        
        std::string headModel = "meshes\\" +
            mEnvironment.mWorld->getStore().bodyParts.find(headID)->model;

		std::string hairModel = "meshes\\" +
            mEnvironment.mWorld->getStore().bodyParts.find(hairID)->model;

        const ESM::BodyPart *upperleg = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "upper leg");
		const ESM::BodyPart *groin = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "groin");
		const ESM::BodyPart *arml = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "upper arm");  //We need two
		const ESM::BodyPart *neck = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "neck");
		const ESM::BodyPart *knee = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "knee");
		const ESM::BodyPart *ankle = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "ankle");
		const ESM::BodyPart *foot = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "foot");
		const ESM::BodyPart *feet = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "feet");
		const ESM::BodyPart *tail = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "tail");
		const ESM::BodyPart *wristl = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "wrist");  //We need two
		const ESM::BodyPart *forearml = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "forearm");  //We need two
		const ESM::BodyPart *handl = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "hand");   //We need two
		const ESM::BodyPart *hair = mEnvironment.mWorld->getStore().bodyParts.search(hairID);
		const ESM::BodyPart *head = mEnvironment.mWorld->getStore().bodyParts.search(headID);
		if(!handl)
			handl = mEnvironment.mWorld->getStore().bodyParts.search (bodyRaceID + "hands");
		//const ESM::BodyPart* claviclel = environment.mWorld->getStore().bodyParts.search (bodyRaceID + "clavicle");
		//const ESM::BodyPart* clavicler = claviclel;
		const ESM::BodyPart* handr = handl;
		const ESM::BodyPart* forearmr = forearml;
		const ESM::BodyPart* wristr = wristl;
		const ESM::BodyPart* armr = arml;
        if(upperleg){
			insertBoundedPart("meshes\\" + upperleg->model + "*|", "Left Upper Leg", base);
			insertBoundedPart("meshes\\" + upperleg->model, "Right Upper Leg", base);
			
		}
        if(foot){
			if(bodyRaceID.compare("b_n_khajiit_m_") == 0)
			{
				feet = foot;
			}
			else
			{
				insertBoundedPart("meshes\\" + foot->model, "Right Foot", base);
				insertBoundedPart("meshes\\" + foot->model + "*|", "Left Foot", base);
			}
		}
        if(groin){
			insertBoundedPart("meshes\\" + groin->model, "Groin", base);
		}
        if(knee)
		{
			insertBoundedPart("meshes\\" + knee->model + "*|", "Left Knee", base);  //e
			insertBoundedPart("meshes\\" + knee->model, "Right Knee", base);   //e
			
		}
		if(ankle){
			
			insertBoundedPart("meshes\\" + ankle->model + "*|", "Left Ankle", base); //Ogre::Quaternion(Ogre::Radian(3.14 / 4), Ogre::Vector3(1, 0, 0)),blank); //1,0,0, blank);
			insertBoundedPart("meshes\\" + ankle->model, "Right Ankle", base);
		}
        if (armr){
			insertBoundedPart("meshes\\" + armr->model, "Right Upper Arm", base);
		}
		if(arml){
			insertBoundedPart("meshes\\" + arml->model + "*|", "Left Upper Arm", base);
		}

		if (forearmr)
		{
				insertBoundedPart("meshes\\" + forearmr->model, "Right Forearm", base);
		}
		if(forearml)
			insertBoundedPart("meshes\\" + forearml->model + "*|", "Left Forearm", base);

		if (wristr)
		{
			insertBoundedPart("meshes\\" + wristr->model, "Right Wrist", base);
		}

		if(wristl)
				insertBoundedPart("meshes\\" + wristl->model + "*|", "Left Wrist", base);
		

	
		

		/*if(claviclel)
			insertBoundedPart("meshes\\" + claviclel->model + "*|", "Left Clavicle", base);
		if(clavicler)
			insertBoundedPart("meshes\\" + clavicler->model , "Right Clavicle", base);*/
	
	
		if(neck)
		{
			insertBoundedPart("meshes\\" + neck->model, "Neck", base);
		}
		if(head)
			insertBoundedPart("meshes\\" + head->model, "Head", base);
		if(hair)
			insertBoundedPart("meshes\\" + hair->model, "Head", base);
}
void Actors::insertBegin (const MWWorld::Ptr& ptr, bool enabled, bool static_){
    Ogre::SceneNode* cellnode;
    if(mCellSceneNodes.find(ptr.getCell()) == mCellSceneNodes.end())
    {
        //Create the scenenode and put it in the map
        cellnode = mMwRoot->createChildSceneNode();
        mCellSceneNodes[ptr.getCell()] = cellnode;
    }
    else
    {
        cellnode = mCellSceneNodes[ptr.getCell()];
    }

    Ogre::SceneNode* insert = cellnode->createChildSceneNode();
    const float *f = ptr.getRefData().getPosition().pos;
    insert->setPosition(f[0], f[1], f[2]);
    insert->setScale(ptr.getCellRef().scale, ptr.getCellRef().scale, ptr.getCellRef().scale);

    // Convert MW rotation to a quaternion:
    f = ptr.getCellRef().pos.rot;

    // Rotate around X axis
    Quaternion xr(Radian(-f[0]), Vector3::UNIT_X);

    // Rotate around Y axis
    Quaternion yr(Radian(-f[1]), Vector3::UNIT_Y);

    // Rotate around Z axis
    Quaternion zr(Radian(-f[2]), Vector3::UNIT_Z);

   // Rotates first around z, then y, then x
    insert->setOrientation(xr*yr*zr);
    if (!enabled)
         insert->setVisible (false);
    ptr.getRefData().setBaseNode(insert);


}
void Actors::insertFreePart(const MWWorld::Ptr& ptr, const std::string& mesh){
    MeshPtr meshp = NIFLoader::load(mesh);
	Entity *ent = mRend.getScene()->createEntity(mesh);
	NIFLoader::getSingletonPtr()->addInMesh(ent->getMesh().getPointer());
}
void Actors::insertMesh (const MWWorld::Ptr& ptr, const std::string& mesh){
    Ogre::SceneNode* insert = ptr.getRefData().getBaseNode();
    assert(insert);

    NifOgre::NIFLoader::load(mesh);
    Entity *ent = mRend.getScene()->createEntity(mesh);
    insert->attachObject(ent);
}

bool Actors::deleteObject (const MWWorld::Ptr& ptr)
{
    if (Ogre::SceneNode *base = ptr.getRefData().getBaseNode())
    {
        Ogre::SceneNode *parent = base->getParentSceneNode();

        for (std::map<MWWorld::Ptr::CellStore *, Ogre::SceneNode *>::const_iterator iter (
            mCellSceneNodes.begin()); iter!=mCellSceneNodes.end(); ++iter)
            if (iter->second==parent)
            {
                base->removeAndDestroyAllChildren();
                mRend.getScene()->destroySceneNode (base);
                ptr.getRefData().setBaseNode (0);
                return true;
            }

        return false;
    }

    return true;
}

void Actors::removeCell(MWWorld::Ptr::CellStore* store){
    if(mCellSceneNodes.find(store) != mCellSceneNodes.end())
    {
        Ogre::SceneNode* base = mCellSceneNodes[store];
        base->removeAndDestroyAllChildren();
        mCellSceneNodes.erase(store);
        mRend.getScene()->destroySceneNode(base);
        base = 0;
    }
}