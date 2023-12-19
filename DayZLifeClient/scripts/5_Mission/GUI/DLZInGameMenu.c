modded class InGameMenu {
    override Widget Init() {
        layoutRoot = super.Init();

        WrapSpacerWidget top = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("top"));
        top.AddChildAfter(m_OptionsButton, m_RestartButton);

        ImageWidget dayz_logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
        dayz_logo.LoadImageFile(0, "DayZLifeClient/layout/MainMenu/DayZLifeLogo.edds");
        dayz_logo.SetImage(0);
        dayz_logo.SetFlags(dayz_logo.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);

        m_ModdedWarning.Show(false);

        return layoutRoot;
    }
};
