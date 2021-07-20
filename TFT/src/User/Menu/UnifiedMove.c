#include "UnifiedMove.h"
#include "includes.h"

void deltaCalibration(void)
{
  mustStoreCmd("G33\n");
}

void menuUnifiedMove(void)
{
  MENUITEMS UnifiedMoveItems = {
    // title
    LABEL_UNIFIEDMOVE,
    // icon                          label
    {
      {ICON_HOME,                    LABEL_HOME},
      {ICON_MOVE,                    LABEL_MOVE},
      {ICON_EXTRUDE,                 LABEL_EXTRUDE},
      {ICON_DISABLE_STEPPERS,        LABEL_DISABLE_STEPPERS},
      {ICON_BABYSTEP,                LABEL_BABYSTEP},
      #if DELTA_PRINTER
        {ICON_CALIBRATION,             LABEL_CALIBRATION},
      #else
        {ICON_MANUAL_LEVEL,            LABEL_LEVELING},
      #endif
      {ICON_BACKGROUND,              LABEL_BACKGROUND},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  if (infoMachineSettings.leveling != BL_DISABLED)
  {
    UnifiedMoveItems.items[6].icon = ICON_LEVELING;
    UnifiedMoveItems.items[6].label.index = LABEL_BED_LEVELING;
  }

  menuDrawPage(&UnifiedMoveItems);

  while (infoMenu.menu[infoMenu.cur] == menuUnifiedMove)
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:
        infoMenu.menu[++infoMenu.cur] = menuHome;
        break;

      case KEY_ICON_1:
        infoMenu.menu[++infoMenu.cur] = menuMove;
        break;

      case KEY_ICON_2:
        infoMenu.menu[++infoMenu.cur] = menuExtrude;
        break;

      case KEY_ICON_3:
        storeCmd("M84\n");
        break;

      case KEY_ICON_4:
        infoMenu.menu[++infoMenu.cur] = menuBabystep;
        break;

      case KEY_ICON_5:
        #if DELTA_PRINTER
          #if REMOVABLE_PROBE
            setDialogText(LABEL_WARNING, LABEL_CONNECT_PROBE, LABEL_CONTINUE, LABEL_CANCEL);
            showDialog(DIALOG_TYPE_ALERT, deltaCalibration, NULL, NULL);
          #else
            deltaCalibration();
          #endif
        #else
          infoMenu.menu[++infoMenu.cur] = menuManualLeveling;
        #endif
        break;

      case KEY_ICON_6:
        if (infoMachineSettings.leveling != BL_DISABLED)
          infoMenu.menu[++infoMenu.cur] = menuBedLeveling;
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        break;

      default:
        break;
    }

    loopProcess();
  }
}
