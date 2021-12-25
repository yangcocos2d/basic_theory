#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "menu.h"

//�ײ�Һ������
void LCD_OnxyPrint(char *buf, int startX, int startY)
{
    HANDLE hd;
    COORD pos;

    pos.X = startX;
    pos.Y = startY;
    hd = GetStdHandle(STD_OUTPUT_HANDLE);    /*��ȡ��׼����ľ��*/
    SetConsoleCursorPosition(hd, pos);        /*���ÿ���̨��������λ��*/
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

//-------------�˵� -----------------------
typedef struct ST_LCD_MENU
{
    ST_LCD_ITEM* st_lcd_item_list;
    ST_LCD_ITEM* st_lcd_item_current;

}ST_LCD_MENU;

//-------------ջ--------------------
typedef struct ST_STACK
{
    int* StackBufferStart;
    int* StackBufferEnd;
    int* StackTop;
}ST_STACK;

/*
�涨���˵�����Ϊ12λ���֡�
ǰ4λΪ�˵������
��4λΪ���˵������б��еĴ���(�޸��˵�����Ϊ0)��
��4λΪ���˵������б��еĴ���
*/

ST_LCD_ITEM st_lcd_item[LCD_ITEM_SIZE];

//ջ
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

//�˵�
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

//ȫ�ֱ���
static ST_LCD_ITEM* p;
static int g_arrow_line = LCD_LINE_START;
//��ͷλ��ջ
static int g_arrow_stack_buffer[LCD_STACK_SIZE];
static ST_STACK g_arrow_stack;
//�˵�·��ջ
static int g_itemID_stack_buffer[LCD_STACK_SIZE];
static ST_STACK g_itemID_stack;
//�˵�
static ST_LCD_MENU g_menu;
static ST_LCD_MENU g_menu_dis;

void MENU_app_set_item(struct ST_LCD_ITEM *item,int item_index)
{
    if(item_index >= sizeof(st_lcd_item) / sizeof(st_lcd_item[0]))
    {
        return;
    }
    st_lcd_item[item_index] = *item;
}

void MENU_appInit()
{
    int i = 0;
    for(i=0;i<sizeof(st_lcd_item) / sizeof(st_lcd_item[0]);i++)
    {
        st_lcd_item[i].ItemFun = MENU_Enter;
    }
    MENU_ItemInitSet(&g_menu,st_lcd_item );
    MENU_ItemInitSet(&g_menu_dis,st_lcd_item );

    ST_STACK_Init(&g_arrow_stack,&g_arrow_stack_buffer[0],&g_arrow_stack_buffer[LCD_STACK_SIZE-1]);
    ST_STACK_Init(&g_itemID_stack,&g_itemID_stack_buffer[0],&g_itemID_stack_buffer[LCD_STACK_SIZE-1]);

}

void MENU_appLast()
{
    if(NULL != MENU_Last(&g_menu))
    {
        if(g_arrow_line>LCD_LINE_START) g_arrow_line--;
    }
}

void MENU_appNext()
{
    if(NULL!=MENU_Next(&g_menu))
    {
        if(g_arrow_line<LCD_LINE_END) g_arrow_line++;
    }
}

void MENU_appExit()
{
     MENU_Back(&g_menu,&g_itemID_stack);
     g_arrow_line = ST_STACK_DataOut(&g_arrow_stack );
}

void MENU_appEnter()
{
    //MENU_Enter(&g_menu);
    ST_LCD_MENU menu_pre = g_menu;
    if(g_menu.st_lcd_item_current->ItemFun == MENU_Enter)
    {
        if(NULL!=g_menu.st_lcd_item_current->ItemFun(&g_menu))
        {
            ST_STACK_DataIn(&g_itemID_stack,menu_pre.st_lcd_item_current->ItemID);
            ST_STACK_DataIn(&g_arrow_stack,g_arrow_line);
            g_arrow_line = LCD_LINE_START;
        }
    }
    else if(g_menu.st_lcd_item_current->ItemFun!=NULL)
    {
        g_menu.st_lcd_item_current->ItemFun(&g_menu);
    }
}

void MENU_appUpdate()
{
    int i=0;
     //�Ѳ˵�ҳ����ʾ����Ļ��
    LCD_Clear();

    g_menu_dis = g_menu;
    LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_START - 1);
    LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_END + 1);

    LCD_OnxyPrint("��",LCD_COLUMN_START, g_arrow_line);
    LCD_OnxyPrint(g_menu_dis.st_lcd_item_current->ItemDisplayStr,LCD_COLUMN_START+2, g_arrow_line);

    for(i=g_arrow_line-1;i>=LCD_LINE_START;i--)
    {
        p=MENU_Last(&g_menu_dis);
        if(p!=NULL)
        {    LCD_OnxyPrint("  ",LCD_COLUMN_START, i);
            LCD_OnxyPrint(p->ItemDisplayStr,LCD_COLUMN_START+2, i);
        }
    }
    g_menu_dis = g_menu;
    for(i=g_arrow_line+1;i<=LCD_LINE_END;i++)
    {
        p = MENU_Next(&g_menu_dis);
        if(p!=NULL)
        {    LCD_OnxyPrint("  ",LCD_COLUMN_START, i);
            LCD_OnxyPrint(p->ItemDisplayStr,LCD_COLUMN_START+2, i);
        }
    }

    //չʾ·��
    {
        int *i,x=5;
        LCD_OnxyPrint("path:                                                   ",0,10);
        for(i = g_itemID_stack.StackBufferStart;i < g_itemID_stack.StackTop;i++)
        {
            ST_LCD_ITEM* item =  MENU_ItemSearch(&g_menu,*i);
            LCD_OnxyPrint(item->ItemDisplayStr,x,10);
            x+= strlen(item->ItemDisplayStr);
            LCD_OnxyPrint(">",x,10);
            x+=1;
        }
    }
}

/*
void main()
{
    char key=0;
    MENU_appInit();

    while(1)
    {
        if(kbhit() != 0)
        {
            key = getch();

            if(key == 'w')
            {
                MENU_appLast();
            }
            if(key == 's')
            {
                MENU_appNext();
            }
            if(key == 'a')
            {
                MENU_appExit();
            }
            if(key == 'd')
            {
                MENU_appEnter();
            }

            MENU_appUpdate();

        }
    }
}
*/
