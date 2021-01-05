modded class PlayerBase
{
    ref DZLHouseMenu houseMenu;
    ref DZLBankingMenu bankingMenu;
    ref DZLTraderMenu traderMenu;
    ref DZLAlmanacMenu almanacMenu;
    ref DZLConfig config;
	ref DZLPlayerHouse house;
	ref DZLPlayer dzlPlayer;
	ref DZLBank dzlBank;
	ref DZLLicenceMenu licenceMenu;
	ref DZLLicenceProgressBar progressBarLicence;
	ref DZLDoorRaidProgressBar progressBarRaid;
	ref DZLCarRaidProgressBar progressBarRaidCar;
	ref DZLHarvestProgressBar progressBarHarvest;
	ref DZLMessageMenu messageMenu;
	ref DZLPlayerMoneyTransferMenu moneyTransferMenu;
	ref DZLSpawnPositionMenu spawnPositionMenu;
	ref DZLLoadOutMenu loadOutMenu;
	ref DZLPlayerArrestMenu arrestMenu;
	ref DZLCarMenu carMenu;
	ref DZLCarStorageMenu carStorageMenu;
	ref DZLMedicHelpMenu healMenu;

	bool IsDZLBank = false;
	bool IsLicencePoint = false;
	bool IsTrader = false;
	bool IsLoadOut = false;
	bool IsGarage = false;
	int moneyPlayerIsDead = 0;
	bool IsRealPlayerDZL = false;
	bool isOnHarvest = false;
	bool isPolice = false;
	bool wasHit = false;
	bool hasShock = false;
	bool willDie = false;
	bool willHealByMedic = false;
	bool willHealByHospital = false;
	bool willHospital = false;

	int timeAskForTraderConfig = 0;
	bool hasTraderConfig = false;
	int timeAskForBankingConfig = 0;
	bool hasBankingConfig = false;

	override void Init() {
        super.Init();
        RegisterNetSyncVariableBool("IsDZLBank");
        RegisterNetSyncVariableBool("IsRealPlayerDZL");
        RegisterNetSyncVariableBool("IsLicencePoint");
        RegisterNetSyncVariableBool("IsTrader");
        RegisterNetSyncVariableBool("IsLoadOut");
        RegisterNetSyncVariableBool("isPolice");
        RegisterNetSyncVariableBool("IsGarage");
        RegisterNetSyncVariableBool("wasHit");
        RegisterNetSyncVariableBool("willHealByMedic");
        RegisterNetSyncVariableBool("willHealByHospital");
        RegisterNetSyncVariableBool("willDie");
        RegisterNetSyncVariableBool("willHospital");
        RegisterNetSyncVariableInt("moneyPlayerIsDead", 0, 99999999999);
		SetCanBeDestroyed(false);
	}


	bool IsDZLPlayer() {
	    return !IsDZLBank && !IsLicencePoint && !IsTrader && !IsLoadOut && !IsGarage;
	}

    override void SetActions(out TInputActionMap InputActionMap) {
        super.SetActions(InputActionMap);

        AddAction(ActionOpenHouseMenu, InputActionMap);
        AddAction(ActionOpenTraderMenu, InputActionMap);
        AddAction(ActionOpenBankingMenu, InputActionMap);
        AddAction(ActionRobMoney, InputActionMap);
        AddAction(ActionRobMoneyFromDead, InputActionMap);
        AddAction(ActionRobBank, InputActionMap);
        AddAction(DZLActionHarvestItem, InputActionMap);
        AddAction(ActionOpenLicenseMenu, InputActionMap);
        AddAction(ActionLicenceCrafting, InputActionMap);
        AddAction(DZLActionLockDoors, InputActionMap);
        AddAction(DZLActionRaidDoors, InputActionMap);
        AddAction(DZLActionUnLockDoors, InputActionMap);
        AddAction(DZLActionTransferMoney, InputActionMap);
        AddAction(ActionOpenLoadOutMenu, InputActionMap);
        AddAction(ActionOpenArrestMenu, InputActionMap);
        AddAction(ActionOpenCarStorageMenu, InputActionMap);

        InitDZLPlayer();
    }

    void InitDZLPlayer() {
        if (GetGame().IsClient() && IsDZLPlayer()) {
            Param1<PlayerBase> paramGetConfig = new Param1<PlayerBase>(this);
            config = new DZLConfig();
            GetGame().RPCSingleParam(paramGetConfig.param1, DAY_Z_LIFE_EVENT_GET_CONFIG, paramGetConfig, true);
            GetGame().RPCSingleParam(paramGetConfig.param1, DAY_Z_LIFE_GET_PLAYER_BUILDING, paramGetConfig, true);
            GetGame().RPCSingleParam(paramGetConfig.param1, DAY_Z_LIFE_PLAYER_DATA, paramGetConfig, true);
            GetGame().RPCSingleParam(paramGetConfig.param1, DAY_Z_LIFE_PLAYER_BANK_DATA, paramGetConfig, true);
        }
    }

    void DisplayMessage(string message) {
        GetMessageMenu();
        messageMenu.SetText(message);
        messageMenu.Init();
        messageMenu.Show();
    }

    void UpdatePlayerAtDependencies() {
        if (houseMenu && houseMenu.IsVisible()) {
            houseMenu.UpdatePlayer(this);
        } else if (bankingMenu && bankingMenu.IsVisible()) {
            bankingMenu.UpdatePlayer(this);
        } else if (traderMenu && traderMenu.IsVisible()) {
            traderMenu.UpdatePlayer(this);
        } else if (licenceMenu && licenceMenu.IsVisible()) {
            licenceMenu.UpdatePlayer(this);
        } else if (progressBarLicence && progressBarLicence.IsVisible()) {
            progressBarLicence.UpdatePlayer(this);
        } else if (loadOutMenu && loadOutMenu.IsVisible()) {
            loadOutMenu.UpdatePlayer(this);
        } else if (arrestMenu && arrestMenu.IsVisible()) {
            arrestMenu.UpdatePlayer(this);
        } else if (carMenu && carMenu.IsVisible()) {
            carMenu.UpdatePlayer(this);
        } else if (carStorageMenu && carStorageMenu.IsVisible()) {
            carStorageMenu.UpdatePlayer(this);
        } else if (healMenu && healMenu.IsVisible()) {
            healMenu.UpdatePlayer(this);
        }
    }

    DZLHouseMenu GetHouseMenu(DZLHouseDefinition definition, Building target) {
        houseMenu = new DZLHouseMenu;
        InitMenu(houseMenu);

		houseMenu.SetHouseDefinition(definition);
		houseMenu.SetTarget(target);

        return houseMenu;
    }
	
    DZLCarMenu GetCarMenu(CarScript car) {
        carMenu = new DZLCarMenu;
        InitMenu(carMenu);
		carMenu.SetCar(car);
		
        return carMenu;
    }

    DZLMedicHelpMenu GetMedicHealMenu() {
        healMenu = new DZLMedicHelpMenu;
        InitMenu(healMenu);

        return healMenu;
    }

    DZLCarStorageMenu GetCarStorageMenu() {
        carStorageMenu = new DZLCarStorageMenu;
        InitMenu(carStorageMenu);

        return carStorageMenu;
    }
	
    private void InitMenu(DZLBaseMenu menu) {
        menu.SetConfig(config);
        menu.SetPlayer(this);
    }

	DZLMessageMenu GetMessageMenu() {
		messageMenu = new DZLMessageMenu();
		return messageMenu;
	}

	DZLAlmanacMenu GetAlmanacMenu() {
		almanacMenu = new DZLAlmanacMenu();
		InitMenu(almanacMenu);
		return almanacMenu;
	}
	
	DZLTraderMenu GetTraderMenu() {
		traderMenu = new DZLTraderMenu();
		InitMenu(traderMenu);
		return traderMenu;
	}

	DZLBankingMenu GetBankingMenu() {
		bankingMenu = new DZLBankingMenu();
		InitMenu(bankingMenu);
		return bankingMenu;
	}

	DZLLicenceMenu GetLicenceMenu() {
		licenceMenu = new DZLLicenceMenu();
		InitMenu(licenceMenu);
		return licenceMenu;
	}

	DZLLoadOutMenu GetLoadOutMenu() {
		loadOutMenu = new DZLLoadOutMenu();
		InitMenu(loadOutMenu);
		return loadOutMenu;
	}

	DZLPlayerArrestMenu GetArrestMenu() {
		arrestMenu = new DZLPlayerArrestMenu();
		InitMenu(arrestMenu);
		return arrestMenu;
	}

	DZLPlayerMoneyTransferMenu GetPlayerMoneyTransferMenu() {
		moneyTransferMenu = new DZLPlayerMoneyTransferMenu();
		InitMenu(moneyTransferMenu);
		return moneyTransferMenu;
	}

    DZLLicenceProgressBar GetLicenceProgressBar() {
        progressBarLicence = new DZLLicenceProgressBar();
        progressBarLicence.SetPlayer(this);
        return progressBarLicence;
    }

    DZLHarvestProgressBar GetHarvestProgressBar() {
        progressBarHarvest = new DZLHarvestProgressBar();
        progressBarHarvest.SetPlayer(this);
        return progressBarHarvest;
    }

    DZLDoorRaidProgressBar GetRaidProgressBar() {
        progressBarRaid = new DZLDoorRaidProgressBar();
        progressBarRaid.SetPlayer(this);
        return progressBarRaid;
    }

    DZLCarRaidProgressBar GetRaidCarProgressBar() {
        progressBarRaidCar = new DZLCarRaidProgressBar();
        progressBarRaidCar.SetPlayer(this);
        return progressBarRaidCar;
    }
	
	DZLSpawnPositionMenu GetSpawnPositionMenu() {
		spawnPositionMenu = new DZLSpawnPositionMenu();
		InitMenu(spawnPositionMenu);
		return spawnPositionMenu;
	}

	void CloseMenu() {
		if (houseMenu && houseMenu.IsVisible()) {
			houseMenu.OnHide();
		} else if (bankingMenu && bankingMenu.IsVisible()) {
			bankingMenu.OnHide();
		} else if (traderMenu && traderMenu.IsVisible()) {
			traderMenu.OnHide();
		} else if (licenceMenu && licenceMenu.IsVisible()) {
			licenceMenu.OnHide();
		} else if (loadOutMenu && loadOutMenu.IsVisible()) {
			loadOutMenu.OnHide();
		} else if (arrestMenu && arrestMenu.IsVisible()) {
			arrestMenu.OnHide();
		} else if (progressBarLicence && progressBarLicence.IsVisible()) {
			progressBarLicence.OnHide();
		} else if (almanacMenu && almanacMenu.IsVisible()) {
			almanacMenu.OnHide();
		} else if (progressBarRaid && progressBarRaid.IsVisible()) {
			progressBarRaid.OnHide();
		} else if (progressBarRaidCar && progressBarRaidCar.IsVisible()) {
			progressBarRaidCar.OnHide();
		} else if (moneyTransferMenu && moneyTransferMenu.IsVisible()) {
			moneyTransferMenu.OnHide();
		} else if (carMenu && carMenu.IsVisible()) {
			carMenu.OnHide();
		} else if (healMenu && healMenu.IsVisible()) {
			healMenu.OnHide();
		} else if (carStorageMenu && carStorageMenu.IsVisible()) {
			carStorageMenu.OnHide();
		}
	}

	DZLHouseDefinition FindHouseDefinition(Building building) {
		array<ref DZLHouseDefinition> houseConfigs = config.GetHouseDefinitions();
		foreach(DZLHouseDefinition definition: houseConfigs) {
                if (definition.houseType == building.GetType()) {
					return definition;
                }
            }
		return null;
	}

    void TransferFromDeadPlayer(DZLPlayer playerTarget) {
        playerTarget.AddMoneyToPlayer(moneyPlayerIsDead);
        moneyPlayerIsDead = 0;
    }

    float GetMoneyPlayerIsDead() {
        return moneyPlayerIsDead;
    }

    void SetMoneyPlayerIsDead(float money) {
        IsRealPlayerDZL = false;
        moneyPlayerIsDead = money;
        money = 0;
    }
	
	DZLLicence GetLicenceByPosition() {
		
		if (GetGame().IsServer()) {
			dzlPlayer = DZLDatabaseLayer.Get().GetPlayer(GetIdentity().GetId());
			config = DZLConfig.Get();
		}

	    if(!dzlPlayer) return null;

        vector playerPosition = GetPosition();
        if (!playerPosition) {
            return null;
        }
		
        foreach(string licenceId: dzlPlayer.licenceIds) {
			DZLLicence licence = config.licenceConfig.licences.FindById(licenceId);
			
			if(!licence) continue;
			
            if (vector.Distance(licence.position, playerPosition) <= licence.range){
                return licence;
            }
        }
        return null;
    }

    DZLTraderPosition GetTraderByPosition(int distance = 2) {
		vector playerPosition = GetPosition();
        if (!playerPosition) {
            return null;
        }
		
		array<ref DZLTraderPosition> positions = config.traderConfig.positions.positions;
		
		foreach(DZLTraderPosition position: positions) {
			float distanceToPos = vector.Distance(position.position, playerPosition);
			if (distanceToPos <= distance){
                return position;
            }
		}
		
		return null;
    }

	string CanUseLicence(notnull DZLLicence licence) {
        array<EntityAI> items = GetPlayerItems();
        map<string, int> craft = new map<string, int>;
        map<string, int> tools = new map<string, int>;
		string message = "";

        foreach(EntityAI item: items) {
            string itemType = item.GetType();
            itemType.ToLower();
			int quantity = item.GetQuantity();
			if (quantity == 0) {
                quantity = 1;
            }
			
			bool isCraft = false;

			foreach(DZLLicenceCraftItem craftItem: licence.craftItems.collection) {
                if(IsNeededItem(craftItem, item, itemType)) {
					int countCraft = 0;
					if (craft.Find(itemType, countCraft)) {
						craft.Set(itemType, countCraft + quantity);
					} else {
						craft.Insert(itemType, quantity);
					}
					isCraft = true;
					break;
				}
			}
			
			if(isCraft) continue;
			
			foreach(DZLLicenceToolItem toolItem: licence.toolItems.collection) {
                if(IsNeededItem(toolItem, item, itemType)) {
					int countTools = 0;
					if (tools.Find(itemType, countTools)) {
						tools.Set(itemType, countTools + quantity);
					} else {
						tools.Insert(itemType, quantity);
					}
					break;
				}
			}
        }
		
		map<string, int> craftMap = licence.craftItems.GetTypeCountMap();
		map<string, int> toolMap = licence.toolItems.GetTypeCountMap();

		if (craft.Count() >= craftMap.Count() && tools.Count() >= toolMap.Count()) {
		    foreach(string type, int count: craftMap) {
				int countFound = 0;
				if (craft.Find(type, countFound)) {
					if (countFound < count) {
						return "#not_enough_items_to_craft";
					}
				} else {
					return "#missing_craft_item";
				}
			}
			
			foreach(string typeTool, int countTool: toolMap) {
                int countFoundTool = 0;
                if (tools.Find(typeTool, countFoundTool)) {
                    if (countFoundTool < countTool) {
                        return "#not_enough_tools_to_craft";
                    }
                } else {
                    return "#missing_tool_item";
                }
            }
		} else {
			message = "#has_not_found_all_items_that_is_needed_to_craft";
		}
		
        return message;
	}

	void UseLicence(DZLLicence licence) {
        array<EntityAI> items = GetPlayerItems();
        map<string, int> craftMap = licence.craftItems.GetTypeCountMap();
        map<string, int> toolMap = licence.toolItems.GetTypeCountMap();
		
		array<DZLLicenceToolItem> tools = new array<DZLLicenceToolItem>;
		
		foreach(DZLLicenceToolItem tool: licence.toolItems.collection) {
			tools.Insert(tool);
		}
		
        foreach(EntityAI item: items) {
			if (craftMap.Count() == 0 && toolMap.Count() == 0) break;
			
            string itemType = item.GetType();
            itemType.ToLower();
            int quantity = item.GetQuantity();
            if (quantity == 0) {
                quantity = 1;
            }

			bool isCraft = false;
			foreach(DZLLicenceCraftItem craftItem: licence.craftItems.collection) {
				if (craftMap.Count() == 0) break;
				
                if(IsNeededItem(craftItem, item, itemType)) {
					int countFoundCraft = 0;
					if(craftMap.Find(itemType, countFoundCraft)) {
						if (quantity == 1) {
							GetGame().ObjectDelete(item);
							countFoundCraft -= 1;
							craftMap.Set(itemType, countFoundCraft);
						} else if (quantity > countFoundCraft) {
							ItemBase.Cast(item).SetQuantity(quantity - countFoundCraft);
							countFoundCraft = 0;
						} else {
							countFoundCraft -= quantity;
							GetGame().ObjectDelete(item);
							craftMap.Set(itemType, countFoundCraft);
						}

						if (0 == countFoundCraft) {
                            craftMap.Remove(itemType);
						}

						isCraft = true;
						break;
					}
				}
			}

			if(isCraft) continue;

			foreach(int index, DZLLicenceToolItem toolItem: tools) {
				if (toolMap.Count() == 0) break;
				
                if(IsNeededItem(toolItem, item, itemType)) {
					int countFoundTool = 0;

					if(toolMap.Find(itemType, countFoundTool)) {
					    int health = item.GetHealth();
						
						if (health >= toolItem.health) {
						    if (quantity == 1) {
						        craftMap.Remove(itemType);
						    } else {
						        craftMap.Set(itemType, craftMap.Get(itemType) - 1);
						    }

							item.SetHealth(health - toolItem.health);
						}
						tools.Remove(index);
						break;
					}
				}
			}
        }

		
        InventoryLocation inventoryLocation = new InventoryLocation;
        EntityAI itemSpawn;
		DZLLicenceCraftedItem itemToCraft = licence.craftedItem;
		
        if (GetInventory().FindFirstFreeLocationForNewEntity(itemToCraft.type, FindInventoryLocationType.ANY, inventoryLocation)) {
            itemSpawn = GetHumanInventory().CreateInInventory(itemToCraft.type);
        } else if (GetHumanInventory().GetEntityInHands()) {
            itemSpawn = GetHumanInventory().CreateInHands(itemToCraft.type);
        } else {
            itemSpawn = SpawnEntityOnGroundPos(itemToCraft.type, GetPosition());
        }
		
		if (itemSpawn) {
			itemSpawn.SetHealth(itemToCraft.health);
			ItemBase.Cast(itemSpawn).SetQuantity(itemToCraft.quantity);
		}
		
	}

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        if (GetGame().IsServer()) {
			if(GetHealth() < 5) {
				SetHealth(5);
				wasHit = true;
			}

			if (GetHealth("", "Shock") < 5) {
				SetHealth("", "Shock", 5);
				hasShock = true;
			}
		}
    }

    bool HasChooseMedicAction() {
        return willDie || willHealByMedic || willHospital;
    }
	
	override void OnScheduledTick(float deltaTime) {
		if (GetGame().IsServer()) {
			if(!willDie) {
			    if (GetHealth("", "Blood") < 5) {
				    SetHealth("", "Blood", 5);
			    }
			
			    if (hasShock) {
				    SetHealth(5);
				    SetHealth("", "Shock", 5);
			    }
            } else if (willHealByMedic) {
                SetHealth(50);
                SetHealth("", "Shock", 50);
                SetHealth("", "Blood", 2500);
                hasShock = false;
                willHealByMedic = false;
            } else if (willHealByHospital) {
                SetHealth(100);
                SetHealth("", "Shock", 100);
                SetHealth("", "Blood", 5000);
                hasShock = false;
                willHealByHospital = false;
                DZLBaseSpawnPoint point = DZLConfig.Get().medicConfig.hospitalSpawnPoints.GetRandomElement();
				
				SetPosition(point.point);
				SetOrientation(point.orientation);
            } else if(willDie) {
                SetCanBeDestroyed(true);
                SetHealth(0);
            }
		} else {
		    if (!HasChooseMedicAction() && g_Game.GetUIManager().GetMenu() == NULL && !healMenu && GetHealth() < 5 && wasHit){
		        GetGame().GetUIManager().ShowScriptedMenu(GetMedicHealMenu(), NULL);
            }
		}

        super.OnScheduledTick(deltaTime);
	}

	override void EEKilled(Object killer) {
	    if (killer == this) {
	        SetCanBeDestroyed(true);
	    }

	    super.EEKilled(killer);
	}

	private bool IsNeededItem(DZLLicenceCraftItem item, EntityAI itemSearch, string ItemSearchType) {
        if(item.GetLowerCaseType() == ItemSearchType) {
            if(GetGame().IsServer()) {
                if (itemSearch.GetHealth() >= item.health){
					return true;
                }
            } else {
                return true;
            }
        }

        return false;
	}
		
	array<EntityAI> GetPlayerItems() {
        array<EntityAI> itemsArray = new array<EntityAI>;
        GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, itemsArray);

        return itemsArray;
    }
}
