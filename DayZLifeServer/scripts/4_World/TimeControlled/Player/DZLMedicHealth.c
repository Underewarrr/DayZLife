class DZLMedicHealth
{
	static void CheckHealth(PlayerBase player, PlayerIdentity playerIdentity) {
	    DZLPlayer dzlPlayer = player.GetDZLPlayer();
	    if (!dzlPlayer.HasNoDieState() && !dzlPlayer.HasBetweenState()) return;
	    bool showMedicHelpMenu = false;
        if (100 > player.GetHealth("GlobalHealth", "Blood")) {
            player.SetHealth("GlobalHealth", "Blood", 50);
            player.SetHealth("GlobalHealth", "Shock", 0);
            showMedicHelpMenu = true;
        }

        if (7 > player.GetHealth("GlobalHealth", "Health")) {
            player.SetHealth("GlobalHealth", "Health", 5);
            player.SetHealth("GlobalHealth", "Shock", 0);
            showMedicHelpMenu = true;
        }

        if (showMedicHelpMenu == true) {
            if (!dzlPlayer.HasBetweenState()) {
                dzlPlayer.SetBetweenState();
                GetGame().RPCSingleParam(player, DAY_Z_LIFE_EVENT_MEDIC_SYNC_PLAYER, null, true, playerIdentity);
            }
        } else if (showMedicHelpMenu == false && dzlPlayer.HasBetweenState()) {
            dzlPlayer.ResetDeadState();
        }
	}

}
