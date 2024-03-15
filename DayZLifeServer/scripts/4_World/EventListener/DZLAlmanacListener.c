class DZLAlmanacListener {
    ref DZLConfig config;

    void DZLAlmanacListener() {
        config = DZLConfig.Get();

        GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
    }

    void ~DZLAlmanacListener() {
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if(rpc_type == DZL_RPC.PLAYER_SERVER_RESET_AT_PLAYER_BASE) {
            PlayerBase.Cast(target).ResetDZLPlayer();
        } else if(rpc_type == DZL_RPC.ALL_PLAYER_ONLINE_PLAYERS) {
            Param1<string> paramJobPlayer;
            if(!ctx.Read(paramJobPlayer)) return;
            SendUpdateListJob(paramJobPlayer.param1, PlayerBase.Cast(target));
        } else if(rpc_type == DZL_RPC.GET_ALL_PLAYERS) {
            SendAllPlayerList(sender);
        } else if(rpc_type == DZL_RPC.DELETE_PLAYER) {
            if(!config.adminIds.HasAccess(DAY_Z_LIFE_ACCESS_PLAYERS, sender.GetId())) return;
            Param1<string> paramDeletePlayer;
            if(ctx.Read(paramDeletePlayer)) {
                string identString = paramDeletePlayer.param1;
                if(DZLDatabaseLayer.Get().HasPlayer(identString)) {
                    DZLPlayer dzlPlayer = DZLDatabaseLayer.Get().GetPlayer(identString);

                    DZLDatabaseLayer.Get().RemovePlayer(identString);
                    DZLDatabaseLayer.Get().RemovePlayerCars(identString);
                    DZLDatabaseLayer.Get().GetPlayerIds().RemovePlayer(identString);
                }

                array<Man> _players = new array<Man>;
                GetGame().GetPlayers(_players);

                if(_players) {
                    foreach(Man _player: _players) {
                        if(_player.GetIdentity().GetId() == identString) {
                            PlayerBase.Cast(_player).ResetDZLPlayer();
                            break;
                        }
                    }
                }

                DZLSendMessage(sender, "#player_data_was_deleted");
                SendAllPlayerList(sender);
            }
        } else if(rpc_type == DZL_RPC.ALL_PLAYER_UPDATE_JOB_PLAYERS) {
            Param2<string, ref array<DZLOnlinePlayer>> paramUpdateJobs;
            if(!ctx.Read(paramUpdateJobs)) return;

            if(!config.adminIds.HasAccess(paramUpdateJobs.param1, sender.GetId())) return;

            DZLPlayerIdentities dzlPlayerIdentities = DZLDatabaseLayer.Get().GetPlayerIds();
            dzlPlayerIdentities.UpdateJob(paramUpdateJobs.param1, paramUpdateJobs.param2);
            DZLSendMessage(sender, "#update_successful");
        } else if(rpc_type == DZL_RPC.MONEY_TRANSFER_ADMIN) {
            if(!config.adminIds.HasAccess(DAY_Z_LIFE_ACCESS_PLAYERS, sender.GetId())) return;
            autoptr Param3<string, int, bool> paramDepositAdminPlayer;
            string messageDepositPP = "";
            if(ctx.Read(paramDepositAdminPlayer)) {
                PlayerIdentity identMoney = sender;
                if(!config.adminIds.HasAccess(DAY_Z_LIFE_ACCESS_PLAYERS, identMoney.GetId())) return;

                DZLPlayer dzlPlayerReciverPP = DZLDatabaseLayer.Get().GetPlayer(paramDepositAdminPlayer.param1);

                if(!paramDepositAdminPlayer.param3) {
                    dzlPlayerReciverPP.AddMoneyToPlayer(paramDepositAdminPlayer.param2);
                } else {
                    DZLDatabaseLayer.Get().GetBank().AddMoney(paramDepositAdminPlayer.param2);
                    dzlPlayerReciverPP.AddMoneyToPlayerBank(paramDepositAdminPlayer.param2);
                }
                DZLSendMessage(identMoney, "#money_transfer_successful");
            }
        }
    }

    void SendUpdateListJob(string job, PlayerBase player) {
        if(!config.adminIds.HasAccess(job, player.GetPlayerId())) return;

        array<Man> _players = new array<Man>;
        GetGame().GetPlayers(_players);
        array<ref DZLOnlinePlayer> collection = new array<ref DZLOnlinePlayer>;

        DZLPlayerIdentities dzlPlayerIdentities = DZLDatabaseLayer.Get().GetPlayerIds();
        array<ref DZLOnlinePlayer> copIdents = dzlPlayerIdentities.GetJobPlayerCollection(job);

        if(_players) {
            foreach(Man _player: _players) {
                string ident = _player.GetIdentity().GetId();
                DZLPlayer dzlPlayer = PlayerBase.Cast(_player).GetDZLPlayer();

                if(!dzlPlayer.CanUseJob(job)) {
                    collection.Insert(new DZLOnlinePlayer(ident, _player.GetIdentity().GetName(), dzlPlayer.GetLastJobRank(job)));
                }
            }
        }

        GetGame().RPCSingleParam(null, DZL_RPC.ALL_PLAYER_ONLINE_PLAYERS_RESPONSE, new Param3<string, ref array<ref DZLOnlinePlayer>, ref array<ref DZLOnlinePlayer>>(job, collection, copIdents), true, player.GetIdentity());
    }





    void SendAllPlayerList(PlayerIdentity player) {
        if(!config.adminIds.HasAccess(DAY_Z_LIFE_ACCESS_PLAYERS, player.GetId())) return;

        array<ref DZLPlayer> collection = new array<ref DZLPlayer>;
        DZLPlayerIdentities dzlPlayerIdentities = DZLDatabaseLayer.Get().GetPlayerIds();

        array<string> allPlayer = dzlPlayerIdentities.playerIdentities;
        foreach(string ident: allPlayer) {
            DZLPlayer _player = DZLDatabaseLayer.Get().GetPlayer(ident);
            collection.Insert(_player);
        }

        GetGame().RPCSingleParam(null, DZL_RPC.GET_ALL_PLAYERS_RESPONSE, new Param1<ref array<ref DZLPlayer>>(collection), true, player);
    }
}
