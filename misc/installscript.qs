function Controller() 
{
    installer.autoRejectMessageBoxes();
    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    });
    var page = gui.pageWidgetByObjectName("WelcomePage");
    page.completeChanged.connect(function() { gui.clickButton(buttons.NextButton); });
    gui.setSilent(true);
}

Controller.prototype.WelcomePageCallback = function() 
{
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.CredentialsPageCallback = function() 
{
    var widget = gui.currentPageWidget();
    widget.loginWidget.EmailLineEdit.setText("");
    widget.loginWidget.PasswordLineEdit.setText("");
    gui.clickButton(buttons.CommitButton);
}

Controller.prototype.IntroductionPageCallback = function() 
{
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function()
{
    var widget = gui.currentPageWidget();
    widget.TargetDirectoryLineEdit.setText(installer.value("targetdir"));
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function() 
{
    var widget = gui.currentPageWidget();

    widget.deselectAll();
    // Use the following first to get the list of components
    if (installer.value("getcomponents")) {
        var components = installer.components();
        console.log("Available components: " + components.length);
        for (var i = 0 ; i < components.length ;i++)
           console.log(components[i].name);
        gui.clickButton(buttons.CancelButton);
        gui.rejectWithoutPrompt();
        return;
    }

    // Then you can specify the ones you want to select in here
    widget.selectComponent("qt.511.clang_64");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.LicenseAgreementPageCallback = function()
{
    gui.currentPageWidget().AcceptLicenseRadioButton.setChecked(true);
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function() 
{
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function()
{
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function() 
{
    var checkBoxForm = gui.currentPageWidget().LaunchQtCreatorCheckBoxForm
    if (checkBoxForm && checkBoxForm.launchQtCreatorCheckBox)
        checkBoxForm.launchQtCreatorCheckBox.checked = false;
    gui.clickButton(buttons.FinishButton);
}
