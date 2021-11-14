#include <stdio.h>
#include <windows.h>
#include <conio.h>

//液晶屏尺寸参数
#define LCD_LINE_START          (0 + 5)
#define LCD_LINE_END            (3 + 5)
#define LCD_COLUMN_START        (0 + 5)
#define LCD_COLUMN_END          (15 + 5)

//底层液晶驱动
void LCD_OnxyPrint(char *buf, int startX, int startY)
{
    HANDLE hd;
    COORD pos;

    pos.X = startX;
    pos.Y = startY;
    hd = GetStdHandle(STD_OUTPUT_HANDLE);    /*获取标准输出的句柄*/
    SetConsoleCursorPosition(hd, pos);        /*设置控制台光标输出的位置*/
    printf("%s",buf );
}

void LCD_Clear()
{
    int i,j;
    for(i=LCD_COLUMN_START;i<=LCD_COLUMN_END;i++)
        for(j=LCD_LINE_START;j<=LCD_LINE_END;j++)
        {
            LCD_OnxyPrint(" ", i, j);
        }
}

//-------------菜单项-----------------------
typedef void *(*ItemFun_Typedef )(void *);

typedef struct ST_LCD_ITEM
{
    int ItemID;

    //菜单项在整个菜单中的位置
    int ItemLast;
    int ItemNext;
    int ItemFather;
    int ItemChildren;

    char *ItemDisplayStr;
    ItemFun_Typedef ItemFun;

}ST_LCD_ITEM;

//-------------菜单 -----------------------
typedef struct ST_LCD_MENU
{
    ST_LCD_ITEM* st_lcd_item_list;
    ST_LCD_ITEM* st_lcd_item_current;

}ST_LCD_MENU;

//-------------栈--------------------
typedef struct ST_STACK
{
    int* StackBufferStart;
    int* StackBufferEnd;
    int* StackTop;
}ST_STACK;

extern void    MENU_ItemInitSet(ST_LCD_MENU* st_lcd_menu,ST_LCD_ITEM st_lcd_item[ ]);
extern ST_LCD_ITEM* MENU_Enter(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Exit(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Next(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Last(ST_LCD_MENU* st_lcd_menu);

/*-------------------------------------------------------------------*/

void PrintScore(ST_LCD_MENU* st_lcd_menu)
{
    LCD_Clear();
    LCD_OnxyPrint("您的分数:100", LCD_LINE_START, LCD_COLUMN_START);
    while(getch()!='a');
}

void PrintAward(ST_LCD_MENU* st_lcd_menu)
{
    LCD_Clear();
    LCD_OnxyPrint("奖励糖果哦", LCD_LINE_START, LCD_COLUMN_START);
    while(getch()!='a');
}

/*
规定，菜单项编号为12位数字。
前4位为菜单项级数，
中4位为父菜单项在列表中的次序(无父菜单项则为0)，
后4位为本菜单项在列表中的次序
*/
ST_LCD_ITEM st_lcd_item[ ] =
{
    {100100000001,100000000000,100000000000,100000000000,100200010001,"欢迎",       MENU_Enter},
    {100200010001,100000000000,100200010002,100100000001,100300010001,"1.英语",     MENU_Enter},

    {100300010001,100000000000,100300010002,100200010001,100000000000,"1.分数",     PrintScore},
    {100300010002,100300010001,100300010003,100200010001,100400020001,"2.排名",     MENU_Enter},
    {100400020001,100000000000,100400020002,100300010002,100000000000,"1.升序",     NULL},
    {100400020002,100400020001,100000000000,100300010002,100000000000,"2.降序",     NULL},

    {100300010003,100300010002,100300010004,100200010001,100000000000,"3.档位",     NULL},
    {100300010004,100300010003,100300010005,100200010001,100000000000,"4.是否及格",   NULL},
    {100300010005,100300010004,100300010006,100200010001,100000000000,"5.是否优秀",   NULL},
    {100300010006,100300010005,100000000000,100200010001,100000000000,"6.是否奖励",   PrintAward},

    {100200010002,100200010001,100200010003,100100000001,100300020001,"2.数学",     MENU_Enter},
    {100300020001,100000000000,100300020002,100200010002,100000000000,"1.分数",     NULL},
    {100300020002,100300020001,100000000000,100200010002,100000000000,"2.排名",     NULL},

    {100200010003,100200010002,100200010004,100100000001,100000000000,"3.语文",     NULL},

    {100200010004,100200010003,100200010005,100100000001,100000000000,"4.物理",     NULL},
    {100200010005,100200010004,100200010006,100100000001,100000000000,"5.生物",     NULL},
    {100200010006,100200010005,100200010007,100100000001,100000000000,"6.化学",     NULL},
    {100200010007,100200010006,100200010008,100100000001,100300010001,"7.地理",     MENU_Enter},

    {100200010008,100200010007,100200010009,100100000001,100000000000,"8.体育",     NULL},
    {100200010009,100200010008,100000000000,100100000001,100000000000,"9.思修",     NULL},

    {000000000000,000000000000,000000000000,000000000000,000000000000,"NULL",   NULL},


};

//栈
void ST_STACK_Init(ST_STACK* stack,int* buffer_start,int* buffer_end)
{
    stack->StackBufferStart = buffer_start;
    stack->StackBufferEnd = buffer_end;
    stack->StackTop = buffer_start;
}

void ST_STACK_DataIn(ST_STACK* stack,int Data)
{
    if(stack->StackTop<= stack->StackBufferEnd)
    {
        *(stack->StackTop) = Data;
        (stack->StackTop)++;
    }
}

int ST_STACK_DataOut(ST_STACK* stack )
{
    if(stack->StackTop > stack->StackBufferStart)
    {
        (stack->StackTop)--;
        return    *(stack->StackTop) ;
    }
    else
    {
        return  LCD_LINE_START;
    }
}

//菜单
void MENU_ItemInitSet(ST_LCD_MENU* st_lcd_menu,ST_LCD_ITEM st_lcd_item[ ] )
{
    st_lcd_menu->st_lcd_item_list = &st_lcd_item[ 0];
    st_lcd_menu->st_lcd_item_current = &st_lcd_item[ 0];

}

ST_LCD_ITEM* MENU_ItemSearch(ST_LCD_MENU* st_lcd_menu,int ItemID)
{
    int i = 0;
    ST_LCD_ITEM* pItem = NULL;

    if(ItemID == 0)
    {
        return pItem;
    }

    while(st_lcd_menu->st_lcd_item_list[ i ].ItemID != 0)
    {
        if(st_lcd_menu->st_lcd_item_list[ i ].ItemID == ItemID)
        {
            pItem = &st_lcd_menu->st_lcd_item_list[ i ];
            return pItem;
        }
        i++;

    }
    return pItem;

}

ST_LCD_ITEM* MENU_Trace(ST_LCD_MENU* st_lcd_menu,int item_id)
{
    ST_LCD_ITEM* pItem = NULL;

    pItem= MENU_ItemSearch( st_lcd_menu,item_id);

    if(pItem!=NULL )
    {
        st_lcd_menu->st_lcd_item_current = pItem;
    }
    return pItem;

}

ST_LCD_ITEM *MENU_Back(ST_LCD_MENU* st_lcd_menu,ST_STACK *stack)
{
    ST_LCD_ITEM* pItem = NULL;
    int item_id = ST_STACK_DataOut(stack);

    pItem= MENU_ItemSearch( st_lcd_menu,item_id);

    if(pItem!=NULL )
    {
        st_lcd_menu->st_lcd_item_current = pItem;
    }
    return pItem;

}

ST_LCD_ITEM* MENU_Enter(ST_LCD_MENU* st_lcd_menu)
{
    return MENU_Trace(st_lcd_menu,st_lcd_menu->st_lcd_item_current->ItemChildren);
}

ST_LCD_ITEM* MENU_Exit(ST_LCD_MENU* st_lcd_menu)
{
    return MENU_Trace(st_lcd_menu,st_lcd_menu->st_lcd_item_current->ItemFather);
}

ST_LCD_ITEM* MENU_Next(ST_LCD_MENU* st_lcd_menu)
{
    return MENU_Trace(st_lcd_menu,st_lcd_menu->st_lcd_item_current->ItemNext);
}

ST_LCD_ITEM* MENU_Last(ST_LCD_MENU* st_lcd_menu)
{
    return MENU_Trace(st_lcd_menu,st_lcd_menu->st_lcd_item_current->ItemLast);
}

#define STACK_SIZE        256

void main()
{
    char key=0;
    int i=0;
    int ArrowLine = LCD_LINE_START;
    ST_LCD_ITEM* p;

    //箭头位置栈
    int arrow_stack_buffer[STACK_SIZE];
    ST_STACK arrow_stack;

    //菜单路径栈
    int itemID_stack_buffer[STACK_SIZE];
    ST_STACK itemID_stack;

    ST_LCD_MENU menu;
    ST_LCD_MENU menu_Dis;
    MENU_ItemInitSet(&menu,st_lcd_item );
    MENU_ItemInitSet(&menu_Dis,st_lcd_item );

    ST_STACK_Init(&arrow_stack,&arrow_stack_buffer[0],&arrow_stack_buffer[STACK_SIZE-1]);
    ST_STACK_Init(&itemID_stack,&itemID_stack_buffer[0],&itemID_stack_buffer[STACK_SIZE-1]);

    while(1)
    {
        if(kbhit() != 0)
        {
            key = getch();

            if(key == 'w')
            {
                if(NULL != MENU_Last(&menu))
                {
                    if(ArrowLine>LCD_LINE_START) ArrowLine--;

                }
            }
            if(key == 's')
            {
                if(NULL!=MENU_Next(&menu))
                {
                    if(ArrowLine<LCD_LINE_END) ArrowLine++;
                }
            }
            if(key == 'a')
            {
                MENU_Back(&menu,&itemID_stack);
                ArrowLine = ST_STACK_DataOut(&arrow_stack );

            }
            if(key == 'd')
            {
                //MENU_Enter(&menu);
                ST_LCD_MENU menu_pre = menu;
                if(menu.st_lcd_item_current->ItemFun == MENU_Enter)
                {
                    if(NULL!=menu.st_lcd_item_current->ItemFun(&menu))
                    {
                        ST_STACK_DataIn(&itemID_stack,menu_pre.st_lcd_item_current->ItemID);
                        ST_STACK_DataIn(&arrow_stack,ArrowLine);
                        ArrowLine = LCD_LINE_START;
                    }
                }
                else if(menu.st_lcd_item_current->ItemFun!=NULL)
                {
                    menu.st_lcd_item_current->ItemFun(&menu);
                }
            }

            //把菜单页面显示在屏幕上
            LCD_Clear();

            menu_Dis = menu;
            LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_START - 1);
            LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_END + 1);

            LCD_OnxyPrint("◆",LCD_COLUMN_START, ArrowLine);
            LCD_OnxyPrint(menu_Dis.st_lcd_item_current->ItemDisplayStr,LCD_COLUMN_START+2, ArrowLine);

            for(i=ArrowLine-1;i>=LCD_LINE_START;i--)
            {
                p=MENU_Last(&menu_Dis);
                if(p!=NULL)
                {    LCD_OnxyPrint("  ",LCD_COLUMN_START, i);
                    LCD_OnxyPrint(p->ItemDisplayStr,LCD_COLUMN_START+2, i);
                }
            }
            menu_Dis = menu;
            for(i=ArrowLine+1;i<=LCD_LINE_END;i++)
            {
                p = MENU_Next(&menu_Dis);
                if(p!=NULL)
                {    LCD_OnxyPrint("  ",LCD_COLUMN_START, i);
                    LCD_OnxyPrint(p->ItemDisplayStr,LCD_COLUMN_START+2, i);
                }
            }

            //展示路径
            {
                int *i,x=5;
                LCD_OnxyPrint("path:                                                   ",0,10);
                for(i = itemID_stack.StackBufferStart;i < itemID_stack.StackTop;i++)
                {
                    ST_LCD_ITEM* item =  MENU_ItemSearch(&menu,*i);
                    LCD_OnxyPrint(item->ItemDisplayStr,x,10);
                    x+= strlen(item->ItemDisplayStr);
                    LCD_OnxyPrint(">",x,10);
                    x+=1;
                }
            }
        }
    }
}
