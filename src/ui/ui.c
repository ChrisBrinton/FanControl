// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: FanControl

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_TempScreen
void ui_TempScreen_screen_init(void);
lv_obj_t *ui_TempScreen;
lv_obj_t *ui_PresetPanel;
lv_obj_t *ui_Label4;
void ui_event_Preset1Btn( lv_event_t * e);
lv_obj_t *ui_Preset1Btn;
lv_obj_t *ui_Label1;
void ui_event_Preset2Btn( lv_event_t * e);
lv_obj_t *ui_Preset2Btn;
lv_obj_t *ui_Label2;
void ui_event_Preset3Btn( lv_event_t * e);
lv_obj_t *ui_Preset3Btn;
lv_obj_t *ui_Label3;
lv_obj_t *ui_TempPanel;
lv_obj_t *ui_CurrentTempPanel;
lv_obj_t *ui_Panel5;
lv_obj_t *ui_CurrentTempLabel;
lv_obj_t *ui_CurrentTemp;
lv_obj_t *ui_Panel6;
lv_obj_t *ui_FanRPMLabel;
lv_obj_t *ui_FanRPM;
lv_obj_t *ui_FanRPMBar;
lv_obj_t *ui_TargetTempPanel;
void ui_event_TargetTempDnBtn( lv_event_t * e);
lv_obj_t *ui_TargetTempDnBtn;
lv_obj_t *ui_Label9;
void ui_event_TargetTempUpBtn( lv_event_t * e);
lv_obj_t *ui_TargetTempUpBtn;
lv_obj_t *ui_Label10;
lv_obj_t *ui_Panel4;
lv_obj_t *ui_TargetTempLabel;
lv_obj_t *ui_TargetTemp;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Preset1Btn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_state_modify( ui_Preset2Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_Preset3Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
}
}
void ui_event_Preset2Btn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_state_modify( ui_Preset1Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_Preset3Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
}
}
void ui_event_Preset3Btn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_state_modify( ui_Preset1Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_Preset2Btn, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
}
}
void ui_event_TargetTempDnBtn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      targetTempDn( e );
}
}
void ui_event_TargetTempUpBtn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      targetTempUp( e );
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_TempScreen_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_TempScreen);
}
