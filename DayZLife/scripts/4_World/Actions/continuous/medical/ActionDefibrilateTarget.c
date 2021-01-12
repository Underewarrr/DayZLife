modded class ActionDefibrilateTarget
{

	override void OnFinishProgressServer(ActionData action_data){
		super.OnFinishProgressServer(action_data);
		DZLEmergencies emergencies = DZLDatabaseLayer.Get().GetEmergencies();
		PlayerBase target = PlayerBase.Cast(action_data.m_Target.GetObject());
		PlayerIdentity targetIdent = target.GetIdentity();
		string targetId = targetIdent.GetId();

        HealByMedic(target);

		if(target && emergencies.HasEmergency(targetId)) {
		    emergencies.Remove(targetId);
		    DZLDatabaseLayer.Get().GetPlayer(targetId).AddMoneyToPlayerBank(DZLConfig.Get().medicConfig.priceMedicHeal * -1);
		    DZLDatabaseLayer.Get().GetPlayer(action_data.m_Player.GetIdentity().GetId()).AddMoneyToPlayerBank(DZLConfig.Get().medicConfig.priceMedicHeal);
		    GetGame().RPCSingleParam(target, DAY_Z_LIFE_ALL_WAS_HEALED_RESPONSE, null, true, targetIdent);
		}
	}

	private void HealByMedic(PlayerBase player) {
        player.healByMedic = true;
        player.healByHospital = false;
        player.noHealthDecrease = 10;
    }
}
