class DZLPlayerSpawnListener {
    ref DZLConfig config;

    void DZLPlayerSpawnListener() {
        config = DZLConfig.Get();
        GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
    }

    void ~DZLPlayerSpawnListener() {
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if(rpc_type == DAY_Z_LIFE_NEW_SPAWN) {
            autoptr Param2<string, string> param;
            if(ctx.Read(param) && param.param1 && param.param2) {
                PlayerBase player = PlayerBase.Cast(target);
                if(!player) return;
                player.RemoveAllItems();
                player.GetDZLPlayer().LoosPlayerInventoryMoney();

                DZLJobSpawnPoints points = config.GetJobSpawnPointsByJobId(param.param2);
                DZLSpawnPoint point = points.FindSpawnById(param.param1);

                DZLPlayer dzlPlayer = player.GetDZLPlayer();
                dzlPlayer.UpdateActiveJob(param.param2);

                if(point) {
                    foreach(string item: point.items) {
                        player.GetInventory().CreateInInventory(item);
                    }
                }

                player.SetPosition(point.point);
                player.SetOrientation(point.orientation);

                player.GetDZLPlayer().GetFractionMember();
                GetGame().RPCSingleParam(player, DAY_Z_LIFE_PLAYER_DATA_RESPONSE, new Param1<ref DZLPlayer>(player.GetDZLPlayer()), true, sender);
                GetGame().RPCSingleParam(null, DAY_Z_LIFE_NEW_SPAWN_RESPONSE, null, true, sender);
            }
        }
    }
}
