class DZLBankRaidTimer {
    private static ref DZLBankRaidTimer bankRaidTimer;
    private ref Timer raidTimer;
    private ref DZLBank bank;

    static DZLBankRaidTimer Get(ref DZLBank _bank) {
        if(!bankRaidTimer) {
            bankRaidTimer = new DZLBankRaidTimer(_bank);
        }

        return bankRaidTimer;
    }

    void DZLBankRaidTimer(ref DZLBank _bank) {
        bank = _bank;

        raidTimer = new Timer;
        if(bank.RaidRuns()) {
            raidTimer.Run(1, bank, "CheckRaid", null, true);
        }
    }

    void Stop() {
        raidTimer.Stop();
        raidTimer = null;
    }

    void Start() {
        raidTimer = new Timer;
        raidTimer.Run(1, bank, "CheckRaid", null, true);
    }

}