#ifndef __MENU_H__
#define __MENU_H__

//-------------菜单项-----------------------
typedef void *(*ItemFun_Typedef )(void *);

typedef struct ST_LCD_ITEM
{
    int ItemID;

    //菜单项在整个菜单中的位置
    long int ItemLast;
    long int ItemNext;
    long int ItemFather;
    long int ItemChildren;

    char *ItemDisplayStr;
    ItemFun_Typedef ItemFun;

}ST_LCD_ITEM;

extern void MENU_app_set_item(struct ST_LCD_ITEM *item,int item_index);
extern void MENU_appInit();
extern void MENU_appLast();
extern void MENU_appNext();
extern void MENU_appExit();
extern void MENU_appEnter();
extern void MENU_appUpdate();

#define LCD_ITEM_SIZE               100
#define LCD_STACK_SIZE              256
//液晶屏尺寸参数
#define LCD_LINE_START          (0 + 5)
#define LCD_LINE_END            (3 + 5)
#define LCD_COLUMN_START        (0 + 5)
#define LCD_COLUMN_END          (15 + 5)

#endif
