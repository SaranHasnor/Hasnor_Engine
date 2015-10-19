#include "interface_utils.h"

typedef struct {
	void (*pushBlock)(placement_t placement);
	void (*popBlock)();

	int (*newSideMenu)(int x, int y, void (*onEnter)(uint), void (*onExit)(uint));
	void (*closeSideMenu)();

	int (*staticLabel)(char *text, placement_t placement, anchor_t anchor);
	int (*dynamicLabel)(char **text, placement_t placement, anchor_t anchor);
	int (*staticButton)(char *text, placement_t placement, void (*onClick)(void));
	int (*textField)(placement_t placement, fieldType_t type);
	int (*list)(placement_t placement, anchor_t textAnchor, void (*onSelect)(uint));
	int (*slider)(placement_t placement);
	int (*checkBox)(placement_t placement);
	int (*radioButton)(placement_t placement, uint groupID);

	void (*staticListEntry)(uint listID, char *entry);
	void (*clearList)(uint listID);
	void (*setSliderValue)(uint sliderID, float min, float max, float *value);
	void (*setTextFieldValue)(uint textFieldID, float min, float max, void **value, bool editable);
	void (*setCheckBoxValue)(uint id, bool *value);
	void (*setRadioButtonValue)(uint id, bool *value);

	uint (*getListSelectedIndex)(uint id);
	void (*saveListSelectedIndex)(uint id);
	void (*loadListSelectedIndex)(uint id);

	void (*deleteObject)(uint objectID);
	void (*deleteAllObjects)();
	void (*clearSideMenu)(uint menuID);

	void (*setRadioGroupSelectionCallback)(uint groupID, void (*onSelectionChanged)(uint index));

	void (*updateLayout)();
} _interface_functions;

#ifdef HASNOR_INIT
void initInterfaceFunctions(_interface_functions *UI);
#endif