```C
typedef struct _AMI_POST_MANAGER_PROTOCOL
{
    AMI_POST_MANAGER_HANDSHAKE                  Handshake;
    AMI_POST_MANAGER_DISPLAY_MESSAGE            DisplayPostMessage;
    AMI_POST_MANAGER_DISPLAY_MESSAGE_EX         DisplayPostMessageEx;
    AMI_POST_MANAGER_DISPLAY_QUIETBOOT_MESSAGE  DisplayQuietBootMessage;
    AMI_POST_MANAGER_DISPLAY_MSG_BOX            DisplayMsgBox;
    AMI_POST_MANAGER_SWITCH_TO_POST_SCREEN      SwitchToPostScreen;
    AMI_POST_MANAGER_SET_CURSOR_POSITION        SetCurPos;
    AMI_POST_MANAGER_GET_CURSOR_POSITION        GetCurPos;
    AMI_POST_MANAGER_INIT_PROGRESSBAR           InitProgressBar;
    AMI_POST_MANAGER_SET_PROGRESSBAR_POSITION   SetProgressBarPosition;
    AMI_POST_MANAGER_POST_STATUS                GetPostStatus;
    AMI_POST_MANAGER_DISPLAY_INFO_BOX           DisplayInfoBox;
    AMI_POST_MANAGER_SET_ATTRIBUTE      SetAttribute;
    AMI_POST_MANAGER_DISPLAY_MENU       DisplayPostMenu;
    AMI_POST_MANAGER_DISPLAY_MSG_BOX_EX     DisplayMsgBoxEx;
    AMI_POST_MANAGER_DRAW_PROGRESS_BOX      DisplayProgress;
    AMI_POST_MANAGER_GET_ATTRIBUTE      GetAttribute;  
    AMI_POST_MANAGER_DISPLAY_TEXT_BOX       DisplayTextBox;
}
AMI_POST_MANAGER_PROTOCOL;
```

---

```C
AMI_POST_MANAGER_DISPLAY_MESSAGE DisplayPostMessage;
AMI_POST_MANAGER_DISPLAY_MESSAGE_EX DisplayPostMessageEx;
```
这两个成员都是向屏幕输出字符串，调用的核心函数都是`PrintPostMessage`
```C
/**
    function to Print the post massages
    @param message BOOLEAN bAdvanceLine
    @retval status
**/
EFI_STATUS PrintPostMessage( CHAR16 *message, BOOLEAN bAdvanceLine )
{
    UINTN LineCount;
    RUNTIME_DEBUG(L"printpostmsg");
    if(bAdvanceLine && (stNextLine > EndRow))
    {
        ScrollPostScreenLine();
        stNextLine = EndRow;
        stColNum = StartCol;
    }
    LineCount = DrawPostStringWithAttribute(stColNum,stNextLine,message, gPostMgrAttribute,bAdvanceLine);
    if(stPostScreenActive)
    {
        FlushLines(stStartLine,stNextLine+LineCount);
        DoRealFlushLines();
        MouseRefresh();
    }
    if(bAdvanceLine)
    {
        stNextLine += LineCount;
        stColNum = StartCol;
        stNextLine++;
        if(stNextLine > gMaxRows)
            stNextLine = gMaxRows;
    }
    else {
        // Update the Line number based on the string displayed, to not to overwrite the existing line in case of a big string.
        if(stColNum > gMaxCols)
        {
            stColNum = (stColNum%gMaxCols);
        }
        stNextLine = stNextLine+LineCount;
        if(stNextLine > EndRow)
        {
            ScrollPostScreenLine();
            stNextLine = EndRow;
        }
    }
    return EFI_SUCCESS;

}
```

```C
/**
    function to write a multiline string in the post screen.
    @param Col UINTN Row,
    @param line UINT8 Attrib
    @retval UINTN
**/
UINTN DrawPostStringWithAttribute( UINTN Col, UINTN Row,CHAR16 *line, UINT8 Attrib, BOOLEAN AdvanceLine )
{
    CHAR16 * text;
    UINTN i=0;
    CHAR16 * String, *temp;
    UINTN Offset;
    UINTN mxCol=gMaxCols;
    temp = StrDup( line );
    String = temp;
    while(1)
    {
        CHAR16 save;
        text = String;
        if ( *String == L'\0' )
            break;

        while ( ( *String != L'\n' ) &&( *String != L'\r' ) && ( *String != L'\0' ) )
            String++;
        save = *String;
        *String = L'\0';
        if(AdvanceLine)
        {
            mxCol = EndCol;
        }

        Offset = HiiFindStrPrintBoundary(text,(mxCol - Col));
        if((AdvanceLine) && (Row+i >= gMaxRows))
        {
            ScrollPostScreenLine();
            Row = gMaxRows-1-i;
        }

        if(Offset < EfiStrLen(text))
        {// Printing the string printed next line also
            *String = save;
            save = text[Offset];
            text[Offset] = 0;
            DrawStringWithAttribute( Col , Row+i, (CHAR16*)text, Attrib);
            String = &text[Offset];
        }
        else
            DrawStringWithAttribute( Col , Row+i, (CHAR16*)text, Attrib);
        stColNum = EfiStrLen(text);// Updating the col position based on present string length.
        if ( ( *String = save ) != L'\0' )
        {
            stColNum = 0; Col=0;// Updating the col position to zero in case of \n and \r.
            if ( *String == L'\r' )
            {   String++;
                i--;
            }
            if ( *String == L'\n' )
            {
                String++;
                if ( *(String - 2) == L'\r' )
                    i++;
            }
        }
        else
            break;
        i++;
        // Update the col position based on string processed to display.
        if ( (!AdvanceLine) && ( (Col+Offset) >= mxCol) ) {
            Col =0;
        }
    }
    MemFreePointer( (VOID **)&temp );
    return i;
}
```