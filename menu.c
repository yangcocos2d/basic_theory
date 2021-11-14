#include <stdio.h>
#include <windows.h>
#include <conio.h>

//Һ�����ߴ����
#define LCD_LINE_START          (0 + 5)
#define LCD_LINE_END            (3 + 5)
#define LCD_COLUMN_START        (0 + 5)
#define LCD_COLUMN_END          (15 + 5)

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

//-------------�˵���-----------------------
typedef void *(*ItemFun_Typedef )(void *);

typedef struct ST_LCD_ITEM
{
    int ItemID;

    //�˵����������˵��е�λ��
    int ItemLast;
    int ItemNext;
    int ItemFather;
    int ItemChildren;

    char *ItemDisplayStr;
    ItemFun_Typedef ItemFun;

}ST_LCD_ITEM;

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

extern void    MENU_ItemInitSet(ST_LCD_MENU* st_lcd_menu,ST_LCD_ITEM st_lcd_item[ ]);
extern ST_LCD_ITEM* MENU_Enter(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Exit(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Next(ST_LCD_MENU* st_lcd_menu);
extern ST_LCD_ITEM* MENU_Last(ST_LCD_MENU* st_lcd_menu);

/*-------------------------------------------------------------------*/

void PrintScore(ST_LCD_MENU* st_lcd_menu)
{
    LCD_Clear();
    LCD_OnxyPrint("���ķ���:100", LCD_LINE_START, LCD_COLUMN_START);
    while(getch()!='a');
}

void PrintAward(ST_LCD_MENU* st_lcd_menu)
{
    LCD_Clear();
    LCD_OnxyPrint("�����ǹ�Ŷ", LCD_LINE_START, LCD_COLUMN_START);
    while(getch()!='a');
}

/*
�涨���˵�����Ϊ12λ���֡�
ǰ4λΪ�˵������
��4λΪ���˵������б��еĴ���(�޸��˵�����Ϊ0)��
��4λΪ���˵������б��еĴ���
*/
ST_LCD_ITEM st_lcd_item[ ] =
{
    {100100000001,100000000000,100000000000,100000000000,100200010001,"��ӭ",       MENU_Enter},
    {100200010001,100000000000,100200010002,100100000001,100300010001,"1.Ӣ��",     MENU_Enter},

    {100300010001,100000000000,100300010002,100200010001,100000000000,"1.����",     PrintScore},
    {100300010002,100300010001,100300010003,100200010001,100400020001,"2.����",     MENU_Enter},
    {100400020001,100000000000,100400020002,100300010002,100000000000,"1.����",     NULL},
    {100400020002,100400020001,100000000000,100300010002,100000000000,"2.����",     NULL},

    {100300010003,100300010002,100300010004,100200010001,100000000000,"3.��λ",     NULL},
    {100300010004,100300010003,100300010005,100200010001,100000000000,"4.�Ƿ񼰸�",   NULL},
    {100300010005,100300010004,100300010006,100200010001,100000000000,"5.�Ƿ�����",   NULL},
    {100300010006,100300010005,100000000000,100200010001,100000000000,"6.�Ƿ���",   PrintAward},

    {100200010002,100200010001,100200010003,100100000001,100300020001,"2.��ѧ",     MENU_Enter},
    {100300020001,100000000000,100300020002,100200010002,100000000000,"1.����",     NULL},
    {100300020002,100300020001,100000000000,100200010002,100000000000,"2.����",     NULL},

    {100200010003,100200010002,100200010004,100100000001,100000000000,"3.����",     NULL},

    {100200010004,100200010003,100200010005,100100000001,100000000000,"4.����",     NULL},
    {100200010005,100200010004,100200010006,100100000001,100000000000,"5.����",     NULL},
    {100200010006,100200010005,100200010007,100100000001,100000000000,"6.��ѧ",     NULL},
    {100200010007,100200010006,100200010008,100100000001,100300010001,"7.����",     MENU_Enter},

    {100200010008,100200010007,100200010009,100100000001,100000000000,"8.����",     NULL},
    {100200010009,100200010008,100000000000,100100000001,100000000000,"9.˼��",     NULL},

    {000000000000,000000000000,000000000000,000000000000,000000000000,"NULL",   NULL},


};

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

#define STACK_SIZE        256

void main()
{
    char key=0;
    int i=0;
    int ArrowLine = LCD_LINE_START;
    ST_LCD_ITEM* p;

    //��ͷλ��ջ
    int arrow_stack_buffer[STACK_SIZE];
    ST_STACK arrow_stack;

    //�˵�·��ջ
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

            //�Ѳ˵�ҳ����ʾ����Ļ��
            LCD_Clear();

            menu_Dis = menu;
            LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_START - 1);
            LCD_OnxyPrint("----------------",LCD_COLUMN_START - 4, LCD_LINE_END + 1);

            LCD_OnxyPrint("��",LCD_COLUMN_START, ArrowLine);
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

            //չʾ·��
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
