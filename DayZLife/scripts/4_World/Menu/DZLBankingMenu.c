class DZLBankingMenu : DZLBaseMenu
{
	TextListboxWidget playerListbox;

    ButtonWidget payInButton;
    ButtonWidget payOutButton;
    ButtonWidget cashTransferButton;
	
    TextWidget playerBankBalanceTextWidget;
    TextWidget balanceTextLabelWidget;
    TextWidget playerBalanceTextWidget;
    TextWidget bankBalanceTextWidget;
	
	EditBoxWidget inputDeposit;
	
    void DZLBankingMenu() {
        layoutPath = "DayZLife/layout/Banking/DZLBanking.layout";
        Construct();
    }

    void ~DZLBankingMenu() {
        Destruct();
    }

    override void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if (rpc_type == DAY_Z_LIFE_PLAYER_DEPOSIT_AT_BANK_DATA_RESPONSE || rpc_type == DAY_Z_LIFE_PLAYER_DEPOSIT_TO_PLAYER_RESPONSE) {
           autoptr Param3<ref DZLPlayer, ref DZLBank, string> paramGetResponse;
           if (ctx.Read(paramGetResponse)){
                bankBalanceTextWidget.SetText(paramGetResponse.param2.moneyAtBank.ToString());
                playerBalanceTextWidget.SetText(paramGetResponse.param1.money.ToString());
                playerBankBalanceTextWidget.SetText(paramGetResponse.param1.bank.ToString());
                errorMessageTextWidget.SetText(paramGetResponse.param3);
           }
       	} else if(rpc_type == DAY_Z_LIFE_ALL_PLAYER_IDENT_DATA_RESPONSE) {
			autoptr Param1<ref array<ref DZLPlayerBankInfo>> paramGetPlayerResponse;
           	if (ctx.Read(paramGetPlayerResponse)){
				playerListbox.ClearItems();
				array<ref DZLPlayerBankInfo> players = paramGetPlayerResponse.param1;
				
				foreach(DZLPlayerBankInfo reciver: players) {
					playerListbox.AddItem(reciver.name, reciver, 0);
				}
			
			}
		}
    }

    override Widget Init() {
        super.Init();

        payOutButton = creator.GetButtonWidget("Button_Pay_out");
        payOutButton.Show(true);
		payInButton = creator.GetButtonWidget("Button_Deposit");
		payInButton.Show(true);
		cashTransferButton = creator.GetButtonWidget("Button_Cash_Transfer");
		cashTransferButton.Show(true);

        playerBankBalanceTextWidget = creator.GetTextWidget("DZLBank");
        playerBankBalanceTextWidget.Show(true);
        playerBalanceTextWidget = creator.GetTextWidget("DZLCash");
        playerBalanceTextWidget.Show(true);
        bankBalanceTextWidget = creator.GetTextWidget("DZLBank_Cash");
        balanceTextLabelWidget = creator.GetTextWidget("Cash_at_Bank");
		
		inputDeposit = creator.GetEditBoxWidget("Input_Deposit");
		
		playerListbox = creator.GetTextListboxWidget("Player_list");
        
		GetGame().RPCSingleParam(player, DAY_Z_LIFE_ALL_PLAYER_IDENT_DATA, new Param1<string>(""), true);

        return layoutRoot;
    }
	
	override void OnShow() {
        if (config) {
            super.OnShow();

            playerBalanceTextWidget.SetText(dzlPlayer.money.ToString());
            playerBankBalanceTextWidget.SetText(dzlPlayer.bank.ToString());

			balanceTextLabelWidget.Show(config.bankConfig.showSumOfStoredCashInBank);
			bankBalanceTextWidget.Show(config.bankConfig.showSumOfStoredCashInBank);

			if (player.dzlBank) bankBalanceTextWidget.SetText(player.dzlBank.moneyAtBank.ToString());
			
        } else {
            OnHide();
        }
    }

    override bool OnClick(Widget w, int x, int y, int button) {
        if (super.OnClick(w, x, y, button)) return true;
        switch(w){
            case payInButton:
                SendDeposit(-1);
				return true;
            case payOutButton:
                SendDeposit(1);
                return true;
			case cashTransferButton:
			 	SendToPlayer();
				return true;
			default:
				break;
        }
        return false;
    }
	
	private void SendToPlayer() {
		int deposit = inputDeposit.GetText().ToInt();
		
		if (deposit < 1) {
			errorMessageTextWidget.SetText("#error_deposit_must_be_positiv");
			return;
		}
		
		int selected = playerListbox.GetSelectedRow();
		
		if (-1 == selected) {
			errorMessageTextWidget.SetText("#error_no_player_was_selected");
			return;
		}
		
		DZLPlayerBankInfo playerBankInfo;

		playerListbox.GetItemData(selected, 0, playerBankInfo);
		
		if (!playerBankInfo) {
			errorMessageTextWidget.SetText("#error_no_player_was_selected");
			return;
		}
		
		if (playerBankInfo.id == player.GetIdentity().GetId()) {
			errorMessageTextWidget.SetText("#error_you_cant_send_money_to_yourself");
			return;
		}
		
		if (deposit != 0) {
            if (deposit <= dzlPlayer.money + dzlPlayer.bank) {
                GetGame().RPCSingleParam(player, DAY_Z_LIFE_PLAYER_DEPOSIT_TO_PLAYER, new Param3<PlayerBase, DZLPlayerBankInfo, int>(player, playerBankInfo, deposit), true);
                errorMessageTextWidget.SetText("");
                inputDeposit.SetText("");
            } else {
                errorMessageTextWidget.SetText("#error_not_enough_money_to_transfer");
            }
        } else {
            errorMessageTextWidget.SetText("#error_deposit_is_not_a_int");
        }
	}


    private void SendDeposit(int factor) {
        if (inputDeposit.GetText().ToInt() >= 1) {
			int deposit = factor * inputDeposit.GetText().ToInt();
            if (deposit >= dzlPlayer.money || deposit <= dzlPlayer.bank) {
                GetGame().RPCSingleParam(player, DAY_Z_LIFE_PLAYER_DEPOSIT_AT_BANK_DATA, new Param2<PlayerBase, int>(player, deposit), true);
                errorMessageTextWidget.SetText("");
                inputDeposit.SetText("");
            } else {
                errorMessageTextWidget.SetText("#error_not_enough_money_to_transfer");
            }
        } else if(inputDeposit.GetText().ToInt() <= 1){
			errorMessageTextWidget.SetText("#error_deposit_must_be_positiv");
		} else {
            errorMessageTextWidget.SetText("#error_deposit_is_not_a_int");
        }
	}

}