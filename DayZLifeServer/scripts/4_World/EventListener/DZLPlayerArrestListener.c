class DZLPlayerArrestListener {
    private DZLArrestConfig arrestConfig;
    private ref Timer timerArrest;
    private ref array<ref DZLEscapedPlayer> escapeePlayers = new array<ref DZLEscapedPlayer>;
    private ref array<ref DZLOpenTicketPlayer> openTicketPlayers = new array<ref DZLOpenTicketPlayer>;
    private int copCount = 0;
    private int transportCount = 0;
    private int civCount = 0;
    private int medicCount = 0;
    private int armyCont = 0;

    void DZLPlayerArrestListener() {
        GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
        arrestConfig = DZLConfig.Get().jobConfig.arrestConfig;

        timerArrest = new Timer;
        timerArrest.Run(60, this, "CheckPrisoners", null, true);
    }

    void ~DZLPlayerArrestListener() {
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if(rpc_type == DAY_Z_LIFE_ARREST_PLAYER) {
            autoptr Param3<PlayerBase, int, string> paramArrestPlayer;
            if(ctx.Read(paramArrestPlayer)) {
                PlayerBase cop = PlayerBase.Cast(target);
                PlayerBase prisoner = paramArrestPlayer.param1;
                int arrestTime = paramArrestPlayer.param2;
                string arrestReason = paramArrestPlayer.param3;

                DZLPlayer copDzl = cop.GetDZLPlayer();
                DZLPlayer prisonerDzl = prisoner.GetDZLPlayer();

                if(copDzl.IsActiveAsMedic() || copDzl.IsActiveAsCivil() || copDzl.IsActiveAsTransport()) return;
                if(copDzl.arrestTimeInMinutes != 0) return;
                if(true == prisonerDzl.IsActiveAsCop() && true == copDzl.IsActiveAsCop()) return;
                if(true == prisonerDzl.IsActiveAsArmy() && true == copDzl.IsActiveAsArmy()) return;

                prisonerDzl.ArrestPlayer(arrestReason, arrestTime);

                ChangeItems(prisoner, arrestConfig.prisonerItems, arrestConfig.shouldDeleteAllItemsOnPrissoner);

                if(arrestConfig.teleportArrestedIntoJail) {
                    prisoner.SetPosition(arrestConfig.teleportPosition.ToVector());
                }

                GetGame().RPCSingleParam(null, DAY_Z_LIFE_EVENT_CLIENT_SHOULD_REQUEST_PLAYER_BASE, null, true, prisoner.GetIdentity());
                GetGame().RPCSingleParam(null, DAY_Z_LIFE_ARREST_PLAYER_RESPONSE, null, true, sender);
                DZLSendMessage(prisoner.GetIdentity(), "#you_got_arrest_in_minutes: " + arrestTime.ToString());
                DZLSendMessage(cop.GetIdentity(), "#you_set_arrest_to_player_in_minutes: " + arrestTime.ToString());
                DZLLogArrest(prisoner.GetPlayerId(), "got arrest", arrestTime);
            }
        } else if(rpc_type == DAY_Z_LIFE_GET_ESCAPED_PLAYERS) {
            GetGame().RPCSingleParam(null, DAY_Z_LIFE_GET_ESCAPED_PLAYERS_RESPONSE, new Param5<ref array<ref DZLEscapedPlayer>, int, int, int, int>(escapeePlayers, copCount, medicCount, civCount, armyCont), true, sender);
        } else if(rpc_type == DAY_Z_LIFE_GET_OPEN_TICKET_PLAYERS) {
            GetGame().RPCSingleParam(null, DAY_Z_LIFE_GET_OPEN_TICKET_PLAYERS_RESPONSE, new Param1<ref array<ref DZLOpenTicketPlayer>>(openTicketPlayers), true, sender);
        } else if(rpc_type == DAY_Z_LIFE_GET_MEDIC_COUNT) {
            GetGame().RPCSingleParam(null, DAY_Z_LIFE_GET_MEDIC_COUNT_RESPONSE, new Param1<int>(medicCount), true, sender);
        }
    }

    void CheckPrisoners() {
        array<Man> allPlayers = new array<Man>;
        GetGame().GetPlayers(allPlayers);
        escapeePlayers.Clear();
        openTicketPlayers.Clear();
        civCount = 0;
        copCount = 0;
        transportCount = 0;
        medicCount = 0;
        armyCont = 0;

        foreach(Man playerMan: allPlayers) {
            PlayerBase player = PlayerBase.Cast(playerMan);
            DZLPlayer dzlPlayer = player.GetDZLPlayer();

            if(dzlPlayer.HasTickets()) {
                openTicketPlayers.Insert(new DZLOpenTicketPlayer(player));
            }

            if(dzlPlayer.IsActiveAsCop()) {
                copCount ++;
            }

            if(dzlPlayer.IsActiveAsTransport()) {
                transportCount ++;
            }

            if(dzlPlayer.IsActiveAsArmy()) {
                armyCont ++;
            }

            if(dzlPlayer.IsActiveAsCivil()) {
                civCount ++;
            }

            if(dzlPlayer.IsActiveAsMedic()) {
                medicCount ++;
            }

            if(dzlPlayer.IsPlayerInArrest()) {
                vector playerPosition = player.GetPosition();

                bool isInPrison = false;
                int prisonArea = -1;
                foreach(int index, vector position: arrestConfig.arrestAreas) {
                    if(vector.Distance(position, playerPosition) < arrestConfig.arrestAreaRadius) {
                        dzlPlayer.ArrestCountDown();
                        GetGame().RPCSingleParam(null, DAY_Z_LIFE_EVENT_CLIENT_SHOULD_REQUEST_PLAYER_BASE, null, true, player.GetIdentity());
                        isInPrison = true;
                        prisonArea = index;
                        break;
                    }
                }
                if(!isInPrison) {
                    escapeePlayers.Insert(new DZLEscapedPlayer(player));
                    continue;
                }

                if(isInPrison && !dzlPlayer.IsPlayerInArrest()) {
                    ChangeItems(PlayerBase.Cast(player), arrestConfig.exPrisonerItems, arrestConfig.shouldDeleteAllItemsOnExPrissoner);


                    vector randPosition = arrestConfig.exPrisonerAreas.Get(prisonArea);

                    if(randPosition) {
                        player.SetPosition(randPosition);
                    }
                }
            }
        }
        DZLDatabaseLayer.Get().SetCopCount(copCount).SetMedicCount(medicCount).SetCivCount(civCount).SetArmyCount(armyCont).SetTransportCount(transportCount);

        GetGame().RPCSingleParam(null, DAY_Z_LIFE_GET_MEDIC_COUNT_RESPONSE, new Param1<int>(medicCount), true);
    }

    private void ChangeItems(PlayerBase prisoner, array<string> items, bool shouldDeleteAllItems) {

        if(shouldDeleteAllItems) {
            prisoner.RemoveAllItems();
        }

        foreach(string type: items) {
            EntityAI item;
            InventoryLocation inventoryLocation = new InventoryLocation;
            if(prisoner.GetInventory().FindFirstFreeLocationForNewEntity(type, FindInventoryLocationType.ANY, inventoryLocation)) {
                item = prisoner.GetHumanInventory().CreateInInventory(type);
            } else if(!prisoner.GetHumanInventory().GetEntityInHands()) {
                item = prisoner.GetHumanInventory().CreateInHands(type);
            }

            if(!item) {
                item = prisoner.SpawnEntityOnGroundPos(type, prisoner.GetPosition());
            }
        }

    }
}
