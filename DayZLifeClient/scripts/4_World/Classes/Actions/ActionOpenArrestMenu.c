class ActionOpenArrestMenu: ActionInteractBase {
    void ActionOpenArrestMenu() {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override void CreateConditionComponents() {
        m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT, false);
        m_ConditionItem = new CCINone;
    }

    override string GetText() {
        return "#give_arrest";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {
        if(!target.GetObject()) return false;
        if(!EntityAI.Cast(target.GetObject()).IsPlayer()) return false;
        DZLPlayer dzlPlayerCop = player.GetDZLPlayer();
        if(dzlPlayerCop.IsActiveJob(DAY_Z_LIFE_JOB_MEDIC) || dzlPlayerCop.IsActiveAsCivil()) return false;
        if(dzlPlayerCop.arrestTimeInMinutes != 0) return false;

        PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());
        if(!targetPlayer.IsDZLPlayer()) return false;

        if(GetGame().IsServer()) {
            if(!targetPlayer || !targetPlayer.GetIdentity()) return false;

            DZLPlayer dzlPlayerPrisoner = targetPlayer.GetDZLPlayer();

            if(true == dzlPlayerPrisoner.IsActiveJob(DAY_Z_LIFE_JOB_COP) && true == dzlPlayerCop.IsActiveJob(DAY_Z_LIFE_JOB_COP)) return false;
            if(true == dzlPlayerPrisoner.IsActiveJob(DAY_Z_LIFE_JOB_ARMY) && true == dzlPlayerCop.IsActiveJob(DAY_Z_LIFE_JOB_ARMY)) return false;
        }

        return true;
    }

    override void OnStartClient(ActionData action_data) {
        if(g_Game.GetUIManager().GetMenu() != NULL) return;
        PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
        PlayerBase player = action_data.m_Player;
        DZLPlayerArrestMenu menu = player.GetArrestMenu();
        menu.SetReceiver(targetPlayer);
        GetGame().GetUIManager().ShowScriptedMenu(menu, NULL);
    }
}
