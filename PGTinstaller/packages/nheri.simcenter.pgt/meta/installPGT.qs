function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows")
    {
        component.addOperation("CreateShortcut", "@TargetDir@/PileGroupTool.exe", "@StartMenuDir@/Pile Group Tool.lnk",
            "workingDirectory=@TargetDir@", "description=Start SimCenter Pile Group Tool");
    }
}
