class ActionRobMoney: ActionInteractBase
{
    void ActionRobMoney() {
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREFB_COME;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
        m_HUDCursorIcon = CursorIcons.None;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);;
        m_ConditionItem = new CCINone;
    }

    override string GetText()
    {
        return "#rob_money";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
		if (!target.GetObject()) return false;
        if (!EntityAI.Cast(target.GetObject()).IsPlayer()) return false;

        PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());

        return targetPlayer.IsRestrained() || targetPlayer.IsUnconscious();
    }

    override void OnEndServer(ActionData action_data) {
        PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
        PlayerBase player = action_data.m_Player;
        DZLPlayer dzlPlayer = new DZLPlayer(player.GetIdentity().GetId());

        if (targetPlayer.IsRestrained() || targetPlayer.IsUnconscious()) {
            DZLPlayer dzlTargetPlayer = new DZLPlayer(targetPlayer.GetIdentity().GetId());
            if (dzlTargetPlayer && dzlTargetPlayer.money > 0) {
                dzlTargetPlayer.TransferFromPlayerToOtherPlayer(dzlPlayer);

                GetGame().RPCSingleParam(player, DAY_Z_LIFE_PLAYER_DATA_RESPONSE, new Param1<ref DZLPlayer>(dzlPlayer), true, player.GetIdentity());
                GetGame().RPCSingleParam(targetPlayer, DAY_Z_LIFE_PLAYER_DATA_RESPONSE, new Param1<ref DZLPlayer>(dzlTargetPlayer), true, targetPlayer.GetIdentity());
            }
        }
    }
}