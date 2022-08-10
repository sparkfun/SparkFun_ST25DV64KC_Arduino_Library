# API Reference for the SFE_QUAD_Menu class

## Brief Overview

The ```SFE_QUAD_Menu``` class allows the user to create a menu as a linked-list of ```SFE_QUAD_Menu_Item``` objects.

The ```SFE_QUAD_Menu``` ```_head``` points to the head (start) of the linked list.

```_next``` points to the next ```SFE_QUAD_Menu_Item``` in the list.

The ```_next``` of the final ```SFE_QUAD_Menu_Item``` in the list is ```NULL```.

```_head``` is initially ```NULL```. It is set to the address of the first ```SFE_QUAD_Menu_Item``` when ```addMenuItem``` is called.

There are several different types of menu item, as defined by the ```SFE_QUAD_Menu_Variable_Type_e``` enum:

- ```SFE_QUAD_MENU_VARIABLE_TYPE_NONE``` contains only the ```_itemName``` as text
    - The name of the menu and perhaps some underscoring can be defined using this type
    - This can be used to include 'empty' lines to separate parts of the menu
- ```SFE_QUAD_MENU_VARIABLE_TYPE_SUB_MENU_START``` is a special type which defines the start of a sub-menu
    - Only this line is printed when the parent menu is displayed
    - The menu items within the sub-menu are only printed when the sub-menu is opened
    - ```openMenu``` is called recursively to enter the sub-menus
- ```SFE_QUAD_MENU_VARIABLE_TYPE_SUB_MENU_END``` marks the end of a sub-menu
- ```SFE_QUAD_MENU_VARIABLE_TYPE_CODE``` contains a pointer to a method which is called when this menu item is selected
- ```SFE_QUAD_MENU_VARIABLE_TYPE_TEXT``` contains ASCII text
    - e.g. a WiFi SSID or password
- ```SFE_QUAD_MENU_VARIABLE_TYPE_BOOL``` is a ```bool``` which is toggled when the menu item is selected
- ```SFE_QUAD_MENU_VARIABLE_TYPE_FLOAT``` contains a ```float```
    - There is an equivalent type for ```double```
- ```SFE_QUAD_MENU_VARIABLE_TYPE_INT``` contains a signed integer ```int```
    - There are equivalent types for ```uint8_t```, ```uint16_t```, ```uint32_t```, ```unsigned long``` and ```long```

Menu items are added with ```addMenuItem```. An overloaded method allows ```CODE``` items to be added.

The menu variable can be read with ```getMenuItemVariable```. The value is returned in a ```SFE_QUAD_Menu_Every_Type_t```.
An overloaded method allows ```TEXT``` values to be read.

The menu variable can be set with ```setMenuItemVariable```, typically to set its default or initial value.
An overloaded method allows ```TEXT``` to be set with a ```const char *```.

Numeric menu items can have minimum and maximum values assigned using ```setMenuItemVariableMin``` and ```setMenuItemVariableMax```.
The built-in ```getValueDouble``` method will then only allow values between the min and max to be entered.

```setMenuPort``` defines which ```Stream``` (Serial port) will be used for the menu.

Debug messages can be displayed if required. ```setDebugPort``` sets the ```Stream``` (Serial port) for those.

The ```SFE_QUAD_Menu``` is not aware of what storage medium is being used by ```SFE_QUAD_Sensors```.
The menu variables can be extracted as CSV text using ```getMenuVariableAsCSV```. Higher methods are then
responsible for writing that text to storage. Likewise, higher methods will read the menu variables from
storage as CSV text. ```updateMenuVariableFromCSV``` will update the variable of the appropriate menu item using that text.

## Initialization / Configuration

### setMenuPort()

This method sets the ```Stream``` (Serial port) for the menu.

The ```supportsBackspace``` parameter can be set to ```true``` if the user is using a terminal emulator, like Tera Term,
which supports backspace. Existing ```TEXT``` items can then be _edited_ instead of having to be entered in full each time.

```c++
void setMenuPort(Stream &port, bool supportsBackspace)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `port` | `Stream &` | The address of the ```Stream``` (Serial port) |
| `supportsBackspace` | `bool` | Default is ```false```. If ```true```, existing ```TEXT``` items can be _edited_ |

### setDebugPort()

This method sets the ```Stream``` (Serial port) for menu debug messages - if desired.

```c++
void setDebugPort(Stream &port)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `port` | `Stream &` | The address of the ```Stream``` (Serial port) |

### addMenuItem()

This method adds a menu item of the specified type to the linked-list.

```c++
bool addMenuItem(const char *itemName, SFE_QUAD_Menu_Variable_Type_e variableType)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item - the text that will be printed when this menu item is shown |
| `variableType` | `SFE_QUAD_Menu_Variable_Type_e` | The variable type associated with this menu item |
| return value | `bool` | ```true``` if the item is added successfully, otherwise ```false``` |

### addMenuItem()

This method allows ```CODE``` menu items to be added.

```c++
bool addMenuItem(const char *itemName, void (*codePointer)())
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item - the text that will be printed when this menu item is shown |
| `codePointer` | `void (*)()` | The address of the method to be called when this menu item is selected |
| return value | `bool` | ```true``` if the item is added successfully, otherwise ```false``` |

### getMenuItemVariableType()

This method returns the variable type of this menu item.

```c++
SFE_QUAD_Menu_Variable_Type_e getMenuItemVariableType(const char *itemName)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| return value | `SFE_QUAD_Menu_Variable_Type_e` | The enum value representing the type |

### getMenuItemVariable()

This method returns the value of the menu item variable.

```c++
bool getMenuItemVariable(const char *itemName, SFE_QUAD_Menu_Every_Type_t *theValue)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `theValue` | `SFE_QUAD_Menu_Every_Type_t *` | A pointer to the struct that will hold the value |
| return value | `bool` | ```true``` if the item exists and the value is retrieved successfully, otherwise ```false``` |

### getMenuItemVariable()

This method returns the value of a ```TEXT``` menu item variable.

The text is copied into ```theValue```. ```maxLen``` should be set to the size of ```theValue```.
The text is only copied if ```theValue``` is large enough to hold it.

```c++
bool getMenuItemVariable(const char *itemName, char *theValue, size_t maxLen)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `theValue` | `char *` | A pointer to the char array that will hold the text |
| `maxLen` | `size_t` | Defines how many characters ```theValue``` can hold |
| return value | `bool` | ```true``` if the item exists and the value is retrieved successfully, otherwise ```false``` |

### setMenuItemVariable()

This method sets the menu item variable in the linked-list with the value passed in ```theValue```.

```c++
bool setMenuItemVariable(const char *itemName, const SFE_QUAD_Menu_Every_Type_t *theValue)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `theValue` | `const SFE_QUAD_Menu_Every_Type_t *` | A pointer to the struct holding the value |
| return value | `bool` | ```true``` if the item exists and the value is updated successfully, otherwise ```false``` |

### setMenuItemVariable()

This method sets the menu item variable in the linked-list with the ```TEXT``` passed in ```theValue```.

```c++
bool setMenuItemVariable(const char *itemName, const char *theValue)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `theValue` | `const char *` | A pointer to the text to be copied into the value |
| return value | `bool` | ```true``` if the item exists and the text is copied successfully, otherwise ```false``` |

### setMenuItemVariableMin()

This method sets the minimum permissible value for the menu item variable. This is used by the menu to check the value is _in range_.

```c++
bool setMenuItemVariableMin(const char *itemName, const SFE_QUAD_Menu_Every_Type_t *minVal)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `minVal` | `const SFE_QUAD_Menu_Every_Type_t *` | A pointer to the struct holding the minimum value |
| return value | `bool` | ```true``` if the item exists and the minimum value is updated successfully, otherwise ```false``` |

### setMenuItemVariableMax()

This method sets the maximum permissible value for the menu item variable. This is used by the menu to check the value is _in range_.

```c++
bool setMenuItemVariableMax(const char *itemName, const SFE_QUAD_Menu_Every_Type_t *maxVal)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The name of the menu item |
| `maxVal` | `const SFE_QUAD_Menu_Every_Type_t *` | A pointer to the struct holding the maximum value |
| return value | `bool` | ```true``` if the item exists and the maximum value is updated successfully, otherwise ```false``` |

## The Menu

### openMenu()

This method opens the menu on the chosen ```Stream``` (Serial port).

This method calls itself recursively when entering a sub-menu. ```start``` points to the address of the ```SUB_MENU_START``` menu item.
If start is ```NULL``` (the default), the menu starts at ```_head```.

```c++
bool openMenu(SFE_QUAD_Menu_Item *start)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `start` | `SFE_QUAD_Menu_Item *` | The address of the menu item to be shown first |
| return value | `bool` | ```true``` if the menu item exists and the menu closes normally, otherwise ```false``` |

## Menu Item Variable Storage

### getNumMenuVariables()

The method returns the number of menu item variables which should be written to storage. The return value dictates how many
time ```getMenuVariableAsCSV``` is called.

```c++
uint16_t getNumMenuVariables(void)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `uint16_t` | The number of menu item variables requiring storage |

### getMenuVariableAsCSV()

This method reads the menu item variable with index ```num``` and writes its name, type and value into ```var``` as CSV-format text.
Higher methods then write that CSV text to storage.

```maxLen``` defines how many characters ```var``` can hold. The text is not written if ```var``` is too small.

```c++
bool getMenuVariableAsCSV(uint16_t num, char *var, size_t maxLen)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `num` | `uint16_t` | The menu item index |
| `var` | `char *` | A pointer to a char array that the CSV text will be written to |
| `maxLen` | `size_t` | The maximum number of characters ```var``` can hold |
| return value | `bool` | ```true``` if the item exists and is copied successfully, otherwise ```false``` |

### updateMenuVariableFromCSV()

```line``` contains a menu variable in CSV format which has been read from storage.
This method updates the appropriate menu item variable with the value contained in the CSV text.

```c++
bool updateMenuVariableFromCSV(char *line)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `line` | `char *` | A pointer to a char array that contains the CSV |
| return value | `bool` | ```true``` if the menu item exists and is updated successfully, otherwise ```false``` |

## Helper Methods

### getMenuChoice()

Ask the user to select a menu item.

```c++
uint32_t getMenuChoice(unsigned long timeout)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `timeout` | `unsigned long` | The timeout in milliseconds |
| return value | `uint32_t` | The menu item selected, 0 if none or if a timeout occurred |

### getValueDouble()

Ask the user to enter an integer or floating point value. Exponent-format values are also acceptable.

```c++
bool getValueDouble(double *value, unsigned long timeout)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `timeout` | `unsigned long` | The timeout in milliseconds |
| `value` | `double *` | A pointer to the double which will hold the value |
| return value | `uint32_t` | ```false``` if nothing is entered or a timeout occurred, otherwise ```true``` |

### getValueText()

Ask the user to enter a ```TEXT``` entry, such as a WiFi password.

```getValueText``` will automatically create a new char array to hold the text and returns a pointer to it.
The old char array is automatically deleted (freed).

```c++
bool getValueText(char **value, unsigned long timeout)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `timeout` | `unsigned long` | The timeout in milliseconds |
| `value` | `char * *` | A pointer to a pointer to the dynamic char array that holds the text |
| return value | `uint32_t` | ```true``` if text is entered within the timeout, otherwise ```false``` |

### getMenuItemNameMaxLen()

Returns the maximum length of all of the menu item names. Used to format the menus correctly (space padding).

```c++
size_t getMenuItemNameMaxLen(void)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `size_t` | The maximum name length |

### getMenuVariablesMaxLen()

Returns the likely maximum combined length of a full menu item (including min and max values if present).
This is used to determine how much memory should be allocated to hold a menu item in CSV format.

```c++
size_t getMenuVariablesMaxLen(void)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| return value | `size_t` | The likely maximum length |

### menuItemExists()

Returns a pointer to the menu item if it exists, otherwise returns ```NULL```.

```c++
SFE_QUAD_Menu_Item *menuItemExists(const char *itemName)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `itemName` | `const char *` | The item name |
| return value | `SFE_QUAD_Menu_Item *` | A pointer to the menu item (if it exists), otherwise ```NULL``` |

### setMenuTimeout()

This method sets the menu timeout.

```c++
void setMenuTimeout(unsigned long newTimeout)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `newTimeout` | `unsigned long` | The new menu timeout in milliseconds |

### setMaxTextChars()

This method sets the maximum number of characters that can be entered into a ```TEXT``` value.

```c++
void setMaxTextChars(uint16_t newMax)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `newMax` | `uint16_t` | The new maximum length |

### setSupportsBackspace()

This method defines if the ```_menuPort``` supports backspace for ```TEXT``` entry.

```c++
void setSupportsBackspace(bool support)
```

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `support` | `bool` | ```true``` in the port supports backspace, otherwise ```false``` |

## Member Variables

| Parameter | Type | Description |
| :-------- | :--- | :---------- |
| `_head` | `SFE_QUAD_Menu_Item *` | A pointer to the first menu item in the linked-list, initially ```NULL``` |
| `_menuPort` | `Stream *` | The address of the ```Stream``` (Serial port) to be used by the menu |
| `_debugPort` | `Stream *` | The address of the ```Stream``` (Serial port) to be used for debug messages (if desired) |
| `_menuTimeout` | `unsigned long` | The default menu timeout in milliseconds. Default is 10000 |
| `_maxTextChars` | `uint16_t` | The maximum length of ```TEXT``` value entries. Default is 32 |
| `_supportsBackspace` | `bool` | ```true``` if the port supports backspace, otherwise ```false```. Default is ```false``` |
