class DZLBankingConfig
{
    ref array<ref DZLBankingPosition> positionOfBankingPoints;
	bool showSumOfStoredCashInBank = true;
	int raidTimeBankInSeconds = 5;
	ref array<string> itemsCanUsedToRaidBank;
    int percentOfMoneyWhenRaid = 50;
	int raidCoolDownTimeInSeconds = 60;
	int maximumRaidDistanceToBank = 5;
	int startCapital = 1000;

    void DZLBankingConfig() {
        if(!Load()) {
            positionOfBankingPoints = new array<ref DZLBankingPosition>;
            itemsCanUsedToRaidBank = new array<string>;

            array<string> attachments = new array<string>;
            attachments.Insert("ManSuit_Black");
            attachments.Insert("SlacksPants_Black");
            attachments.Insert("ThickFramesGlasses");
            attachments.Insert("DressShoes_Black");

            if (DAY_Z_LIFE_DEBUG) {
                 // first Bank
                positionOfBankingPoints.Insert(new DZLBankingPosition("11053.101563 226.815567 12388.920898", "0 0 0", "SurvivorM_Boris", attachments));
                // second bank
                positionOfBankingPoints.Insert(new DZLBankingPosition("12326.892578 140.493500 12659.409180", "0 0 0", "SurvivorM_Rolf", attachments));

                itemsCanUsedToRaidBank.Insert("M4A1");
            } else {
                //TODO normal base config add
            }

            Save();
        }
		
		if (percentOfMoneyWhenRaid > 100) {
			percentOfMoneyWhenRaid = 100;
			
			Save();
		}
    }

    private bool Load(){
        if (GetGame().IsServer() && FileExist(DAY_Z_LIFE_SERVER_FOLDER_CONFIG + "banking.json")) {
            JsonFileLoader<DZLBankingConfig>.JsonLoadFile(DAY_Z_LIFE_SERVER_FOLDER_CONFIG + "banking.json", this);
            return true;
        }
        return false;
    }

    private void Save(){
        if (GetGame().IsServer()) {
            CheckDZLConfigPath();
            JsonFileLoader<DZLBankingConfig>.JsonSaveFile(DAY_Z_LIFE_SERVER_FOLDER_CONFIG + "banking.json", this);
        }
    }
}