class CfgPatches
{
	class Cocain
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Container_Base;
	class Paper;
	class DZL_Coca_Plant: HouseNoDestruct
	{
		scope=1;
		model="DZ\gear\cultivation\Cannabis_plant.p3d";
		physLayer="item_small";
		displayName="#Cocaine_plant";
		descriptionShort="#Cocaine_plant";
		carveNavmesh=0;
		hiddenSelections[]=
		{
			"plantstage_03",
			"plantstage_02",
			"plantstage_04",
			"plantstage_05",
			"plantstage_01",
			"infestedparts"
		};
		hiddenSelectionsTextures[]=
		{
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa",
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa",
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa",
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa",
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa",
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Plant.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\gear\cultivation\data\Cannabis_plant.rvmat"
		};
	};
	class DZL_Cocain_Brick: Container_Base
	{	
		scope=2;
		displayName="#Cocain_Brick";
		descriptionShort="#Cocain_Brick_desc";
		model="\dz\gear\containers\FirstAidKit.p3d";
		rotationFlags=17;
		lootCategory="Crafted";
		weight=500;
		itemSize[]={3,3};
		absorbency=0.2;
		isMeleeWeapon=1;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"DZL_Misc\Drugs\Cocain\Data\Cocain_Brick.paa"
		};
	};
	class DZL_Cocain: Paper
	{	
		scope=2;
		displayName="#raw_Cocaine";
		descriptionShort="#raw_Cocaine_desc";
		model="\dz\gear\consumables\Paper.p3d";
		rotationFlags=17;
		lootCategory="Crafted";
		weight=20;
		itemSize[]={2,2};
		absorbency=0.2;
		isMeleeWeapon=1;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"DZL_Misc\Drugs\Cocain\Data\Cocain.paa"
		};
	};
};
