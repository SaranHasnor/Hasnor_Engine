#include "interface_utils.h"

void interface_pushBlock(placement_t placement);
void interface_popBlock();

int interface_newSideMenu(int x, int y, void (*onEnter)(uint), void (*onExit)(uint));
void interface_closeSideMenu();

int interface_staticLabel(char *text, placement_t placement, anchor_t anchor);
int interface_dynamicLabel(char **text, placement_t placement, anchor_t anchor);
int interface_staticButton(char *text, placement_t placement, void (*onClick)(void));
int interface_textField(placement_t placement, fieldType_t type);
int interface_list(placement_t placement, anchor_t textAnchor, void (*onSelect)(uint));
int interface_slider(placement_t placement);
int interface_checkBox(placement_t placement);
int interface_radioButton(placement_t placement, uint groupID);

void interface_staticListEntry(uint listID, char *entry);
void interface_clearList(uint listID);
void interface_setSliderValue(uint sliderID, float min, float max, float *value);
void interface_setTextFieldValue(uint textFieldID, float min, float max, void **value, bool editable);
void interface_setCheckBoxValue(uint id, bool *value);
void interface_setRadioButtonValue(uint id, bool *value);

uint interface_getListSelectedIndex(uint id);
void interface_saveListSelectedIndex(uint id);
void interface_loadListSelectedIndex(uint id);

void interface_deleteObject(uint objectID);
void interface_deleteAllObjects();
void interface_clearSideMenu(uint menuID);

void interface_setRadioGroupSelectionCallback(uint groupID, void (*onSelectionChanged)(uint index));

void interface_updateLayout();
