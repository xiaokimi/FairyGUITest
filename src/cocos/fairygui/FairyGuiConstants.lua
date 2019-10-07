if nil == fgui then
    return
end

fgui.RelationType =
{
    Left_Left 			= 0,
    Left_Center 		= 1,
    Left_Right 			= 2,
    Center_Center		= 3,
    Right_Left 			= 4,
    Right_Center 		= 5,
    Right_Right 		= 6,

    Top_Top				= 7,
    Top_Middle			= 8,
    Top_Bottom			= 9,
    Middle_Middle		= 10,
    Bottom_Top			= 11,
    Bottom_Middle		= 12,
    Bottom_Bottom		= 13,

    Width				= 14,
    Height				= 15,

    LeftExt_Left		= 16,
    LeftExt_Right		= 17,
    RightExt_Left		= 18,
    RightExt_Right		= 19,
    TopExt_Top			= 20,
    TopExt_Bottom		= 21,
    BottomExt_Top		= 22,
    BottomExt_Bottom 	= 23,

    Size 				= 24
}

fgui.UIEventType = 
{
    Enter           = 0;
    Exit            = 1;
    Changed         = 2;
    Submit          = 3;

    TouchBegin      = 10;
    TouchMove       = 11;
    TouchEnd        = 12;
    Click           = 13;
    RollOver        = 14;
    RollOut         = 15;
    MouseWheel      = 16;
    RightClick      = 17;
    MiddleClick     = 18;

    PositionChange  = 20;
    SizeChange      = 21;

    KeyDown         = 30;
    KeyUp           = 31;

    Scroll          = 40;
    ScrollEnd       = 41;
    PullDownRelease = 42;
    PullUpRelease   = 43;

    ClickItem       = 50;
    ClickLink       = 51;
    ClickMenu       = 52;
    RightClickItem  = 53;

    DragStart       = 60;
    DragMove        = 61;
    DragEnd         = 62;
    Drop            = 63;

    GearStop        = 70;
}