class DZLBaseMenu: UIScriptedMenu {
    protected ref DZLUIItemCreator creator;
    protected ref DZLConfig config;
    protected string layoutPath;
    protected ButtonWidget closeButton;
    protected PlayerBase player;
    protected DZLPlayer dzlPlayer;
    protected bool hasCloseButton = true;
    protected bool hideCourser = true;
    protected bool showCourser = true;
    protected bool canClose = true;
    protected bool showHud = true;
    protected bool showQuickBar = true;


    void DZLBaseMenu() {
        if(GetGame().IsClient()) {
            GetDayZGame().Event_OnRPC.Insert(HandleEventsDZL);
        }
    }

    void ~DZLBaseMenu() {
        OnHide();
        GetDayZGame().Event_OnRPC.Remove(HandleEventsDZL);
    }

    void SetPlayer(PlayerBase _player) {
        this.player = _player;
        this.dzlPlayer = _player.GetDZLPlayer();
        config = DZLConfig.Get();
    }

    void UpdatePlayer(PlayerBase _player) {
        SetPlayer(_player);
        UpdateGUI();
    }

    void UpdateGUI(string message = "") {
        if(message) {
            player.DisplayMessage(message);
        }
    }

    void HandleEventsDZL(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {}

    override Widget Init() {
        creator = new DZLUIItemCreator(layoutPath);

        if(hasCloseButton) {
            closeButton = creator.GetButtonWidget("Button_Closed");
            closeButton.Show(true);
        }

        layoutRoot = creator.GetLayoutRoot();

        layoutRoot.Show(false);

        return layoutRoot;
    }

    override bool OnClick(Widget w, int x, int y, int button) {
        if(super.OnClick(w, x, y, button)) return true;

        switch(w) {
            case closeButton:
                OnHide();
                return true;
            default:
                break;
        }
        return false;
    }

    override void OnShow() {
        super.OnShow();

        GetGame().GetUIManager().ShowCursor(true);

        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);

        GetGame().GetMission().GetHud().ShowHud(showHud);
        GetGame().GetMission().GetHud().ShowQuickBar(showQuickBar);
    }

    override void Update(float timeslice) {
        super.Update(timeslice);
        if(canClose && GetUApi() && GetUApi().GetInputByName("UAUIBack").LocalPress()) {
            GetGame().GetUIManager().HideScriptedMenu(this);
        }
    }

    override void OnHide() {
        super.OnHide();

        GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(true);
        GetGame().GetMission().GetHud().ShowHud(true);
        GetGame().GetMission().GetHud().ShowQuickBar(true);
        Close();
    }
}
