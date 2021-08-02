#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectExtrasMenu *ExtrasMenu;

void ExtrasMenu_Update(void) {}

void ExtrasMenu_LateUpdate(void) {}

void ExtrasMenu_StaticUpdate(void) {}

void ExtrasMenu_Draw(void) {}

void ExtrasMenu_Create(void *data) {}

void ExtrasMenu_StageLoad(void) {}

void ExtrasMenu_Initialize(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    LogHelpers_Print("ManiaModeMenu_Initialize()");

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Extras");
        if (RSDK.StringCompare(&info, &control->tag, false))
            ExtrasMenu->extrasControl = (Entity *)control;
    }
}

void ExtrasMenu_Unknown2(void)
{
    EntityUIControl *control = (EntityUIControl *)ExtrasMenu->extrasControl;

    EntityUIButton *button = control->entities[0];
    int unlock             = SaveGame_CheckUnlock(8);
    button->disabled       = !unlock;
    if (button->disabled)
        UIButton_Unknown1(button);

    button           = control->entities[1];
    unlock           = SaveGame_CheckUnlock(6);
    button->disabled = !unlock;
    if (button->disabled)
        UIButton_Unknown1(button);

    button           = control->entities[2];
    unlock           = SaveGame_CheckUnlock(7);
    button->disabled = !unlock && !globals->medallionDebug;
}

void ExtrasMenu_Unknown3(void)
{
    EntityUIControl *control      = (EntityUIControl *)ExtrasMenu->extrasControl;
    control->processButtonInputCB = ExtrasMenu_ProcessInputs;

    foreach_all(UIButton, button)
    {
        if (button->listID == 7) {
            if (button->frameID == 8) {
                button->options2  = ExtrasMenu_Callback_Credits;
                button->field_150 = 1;
            }
            if (button->listID == 7 && button->frameID == 4) {
                button->options2  = ExtrasMenu_Callback_DAGarden;
                button->field_150 = 1;
            }
        }
    }

    foreach_all(UIChoice, choice)
    {
        if (choice->listID == 7) {
            if (choice->frameID == 2) {
                choice->options2  = ExtrasMenu_Callback_Puyo_vsAI;
                choice->field_148 = 1;
            }
            if (choice->listID == 7) {
                if (choice->frameID == 3) {
                    choice->options2  = ExtrasMenu_Callback_Puyo_vsAI;
                    choice->field_148 = 1;
                }
                if (choice->listID == 7) {
                    if (choice->frameID == 6) {
                        choice->options2  = ExtrasMenu_Callback_BSS_3K;
                        choice->field_148 = 1;
                    }
                    if (choice->listID == 7 && choice->frameID == 7) {
                        choice->options2  = ExtrasMenu_Callback_BSS_Mania;
                        choice->field_148 = 1;
                    }
                }
            }
        }
    }
}

void ExtrasMenu_ProcessMedallionCheat(void)
{
    int key = 0;
    if (UIControl->keyLeft) {
        key = 1;
    }
    else if (UIControl->keyRight) {
        key = 2;
    }

    if (key) {
        for (int i = 0; i < 7; ++i) {
            ExtrasMenu->cheatCode[i] = ExtrasMenu->cheatCode[i + 1];
        }
        ExtrasMenu->cheatCode[7] = key;
    }
}

bool32 ExtrasMenu_CheckMedallionCheat(void)
{
    return ExtrasMenu->cheatCode[0] == 1 && ExtrasMenu->cheatCode[1] == 1 && ExtrasMenu->cheatCode[2] == 2 && ExtrasMenu->cheatCode[3] == 2
           && ExtrasMenu->cheatCode[4] == 1 && ExtrasMenu->cheatCode[5] == 2 && ExtrasMenu->cheatCode[6] == 1 && ExtrasMenu->cheatCode[7] == 2;
}

void ExtrasMenu_ProcessInputs(void)
{
    RSDK_THIS(UIControl);
    if (entity->activeEntityID == 2) {
        ExtrasMenu_ProcessMedallionCheat();
        if (ExtrasMenu_CheckMedallionCheat()) {
            if (!globals->medallionDebug) {
                RSDK.PlaySFX(UIWidgets->sfx_Event, false, 255);
                globals->medallionDebug = true;
                ExtrasMenu_Unknown2();
            }
        }
    }
    UIControl_ProcessButtonInput();
}

void ExtrasMenu_Start_Puyo_vsAI(void)
{
    TimeAttackData_ClearOptions();
    ((byte *)&globals->menuParam)[89] = 1;
    char *tag                         = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 1;
    RSDK.LoadScene("Extras", "Puyo Puyo");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_Puyo_vsAI(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_Puyo_vsAI;
}

void ExtrasMenu_Start_Puyo_vs2P(void)
{
    TimeAttackData_ClearOptions();
    ((byte *)&globals->menuParam)[89] = 2;
    char *tag                         = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 1;
    RSDK.LoadScene("Extras", "Puyo Puyo");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_Puyo_vs2P(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_Puyo_vs2P;
}

void ExtrasMenu_Start_Credits(void)
{
    TimeAttackData_ClearOptions();
    ((byte *)&globals->menuParam)[89] = 1;
    char *tag                         = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 3;
    globals->menuParam[106] = 1;
    RSDK.LoadScene("Presentation", "Credits");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_Credits(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_Credits;
}

void ExtrasMenu_Start_DAGarden(void)
{
    TimeAttackData_ClearOptions();
    char *tag = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 2;
#if RETRO_USE_PLUS
    if (API.CheckDLC(DLC_PLUS))
        RSDK.LoadScene("Extras", "D.A. Garden Plus");
    else
#endif
        RSDK.LoadScene("Extras", "D.A. Garden");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_DAGarden(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_DAGarden;
}

void ExtrasMenu_Start_BSS_3K(void)
{
    TimeAttackData_ClearOptions();
    ((byte *)&globals->menuParam)[89] = 1;
    char *tag                         = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 0;
    RSDK.LoadScene("Blue Spheres", "Random");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_BSS_3K(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_BSS_3K;
}

void ExtrasMenu_Start_BSS_Mania(void)
{
    TimeAttackData_ClearOptions();
    ((byte *)&globals->menuParam)[89] = 1;
    char *tag                         = (char *)&((char *)&globals->menuParam)[90];
    strcpy(tag, "Extras");
    globals->menuParam[MP_MenuSelection] = 0;
    RSDK.LoadScene("Blue Spheres", "Random 2");
    RSDK.InitSceneLoad();
}

void ExtrasMenu_Callback_BSS_Mania(void)
{
    EntityMenuSetup *menuSetup = CREATE_ENTITY(MenuSetup, NULL, -0x100000, -0x100000);
    menuSetup->active          = ACTIVE_ALWAYS;
    menuSetup->fadeColour      = 0x000000;
    menuSetup->field_68        = 5;
    menuSetup->timeOut         = 32;
    menuSetup->state           = MenuSetup_Unknown13;
    menuSetup->timedState      = ExtrasMenu_Start_BSS_Mania;
}

void ExtrasMenu_EditorDraw(void) {}

void ExtrasMenu_EditorLoad(void) {}

void ExtrasMenu_Serialize(void) {}
#endif
