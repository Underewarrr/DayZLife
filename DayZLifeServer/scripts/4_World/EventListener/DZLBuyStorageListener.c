class DZLBuyStorageListener
{
    private ref DZLPlayerInventory inventory;
    private ref DZLHouseFinder houseFinder;
	private ref DZLConfig config;

    void DZLBuyStorageListener() {
        GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
        inventory = new DZLPlayerInventory;
        houseFinder = new DZLHouseFinder;
        config = new DZLConfig;

        houseFinder.SetConfig(config);
    }

    void ~DZLBuyStorageListener() {
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if (rpc_type == DAY_Z_LIFE_BUY_STORAGE) {
			autoptr  Param3<PlayerBase, ref Building, ref DZLStorageType> paramBuyStorage;
            if (ctx.Read(paramBuyStorage)){
				
				DZLBuilding dzlBuilding = new DZLBuilding(paramBuyStorage.param2);
                DZLHouseDefinition actualHouseDef = houseFinder.GetHouseDefinitionByBuilding(paramBuyStorage.param2);

				DZLStorageType storageType;
				array<ref DZLStorageType> storageTypes = config.GetStorageTypes();
				foreach(DZLStorageType _storageType: storageTypes) {
					if (_storageType.type == paramBuyStorage.param3.type) {
						storageType = _storageType;
						break;
					}
				}
			
				if (storageType) {
	                string message = "#error_buying_storage";
					int buyPriceBuy =  storageType.price * (actualHouseDef.storageBuyFactor * (dzlBuilding.GetStorage().Count() + 1));
					vector posToSpawnRelavtiv = dzlBuilding.GetNextFreeStoragePosition(actualHouseDef);
					
					bool canNotSpawn = posToSpawnRelavtiv == "0 0 0";
					
	                if (!canNotSpawn && actualHouseDef.GetMaxStorage() > dzlBuilding.GetStorage().Count() && inventory.PlayerHasEnoughMoney(paramBuyStorage.param1, buyPriceBuy) && dzlBuilding.IsOwner(paramBuyStorage.param1)) {
	                   vector posToSpawn = paramBuyStorage.param2.ModelToWorld(posToSpawnRelavtiv);
						bool hasSpawned = DZLSpawnHelper.SpawnContainer(posToSpawn, paramBuyStorage.param2.GetOrientation(), storageType.type);
						
						if (hasSpawned) {
							inventory.AddMoneyToPlayer(paramBuyStorage.param1, buyPriceBuy * -1);
							dzlBuilding.BuyStorageOnServer(new DZLStorageTypeBought(storageType, posToSpawn, buyPriceBuy, posToSpawnRelavtiv));
							message = "#successfully_buy_storage";
						}

					}
					
					GetGame().RPCSingleParam(paramBuyStorage.param1, DAY_Z_LIFE_BUY_STORAGE_RESPONSE, new Param2<ref DZLBuilding, string>(dzlBuilding, message), true, sender);
	                GetGame().RPCSingleParam(paramBuyStorage.param1, DAY_Z_LIFE_GET_PLAYER_BUILDING_RESPONSE, new Param1<ref DZLPlayerHouse>(new DZLPlayerHouse(sender)), true, sender);
				}
            }
        } else if (rpc_type == DAY_Z_LIFE_SELL_STORAGE) {
            autoptr Param3<PlayerBase, ref Building, vector> paramSellStorage;
            if (ctx.Read(paramSellStorage)){
                DZLBuilding dzlBuildingSell = new DZLBuilding(paramSellStorage.param2);
                DZLHouseDefinition actualHouseDefSell = houseFinder.GetHouseDefinitionByBuilding(paramSellStorage.param2);

                string messageSell = "#error_sell_house";

                if (actualHouseDefSell && dzlBuildingSell && dzlBuildingSell.IsOwner(paramSellStorage.param1)) {
                    DZLStorageTypeBought positionToSell = dzlBuildingSell.FindStorageByPosition(paramSellStorage.param3);
                    if (positionToSell) {
                        Container_Base itemToDestroy = houseFinder.objectFinder.GetContainerAt(positionToSell.position, positionToSell.position, positionToSell.type, paramSellStorage.param2);

                        if (itemToDestroy) {
                            inventory.AddMoneyToPlayer(paramSellStorage.param1, positionToSell.sellPrice);
                            dzlBuildingSell.SellStorageOnServer(positionToSell);

                            GetGame().ObjectDelete(itemToDestroy);
                            messageSell = "#successfully_sell_house";
                        }
                    }
                }
                GetGame().RPCSingleParam(paramSellStorage.param1, DAY_Z_LIFE_SELL_STORAGE_RESPONSE, new Param2<ref DZLBuilding, string>(dzlBuildingSell, messageSell), true, sender);
                GetGame().RPCSingleParam(paramSellStorage.param1, DAY_Z_LIFE_GET_PLAYER_BUILDING_RESPONSE, new Param1<ref DZLPlayerHouse>(new DZLPlayerHouse(sender)), true, sender);
            }
        }
    }
}