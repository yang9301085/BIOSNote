
*AmiCompatibilityPkg\Setup\AmiBiosInfo.sd 

```C
#ifdef MAIN_FORM_SET
    #ifdef FORM_SET_ITEM
        subtitle
            text = STRING_TOKEN(STR_BIOS);
 //J221009+S       
#if (defined OEM_MANUFAC_NAME_SUPPORT && OEM_MANUFAC_NAME_SUPPORT == 1)
        text
          help  = STRING_TOKEN(STR_EMPTY),
          text  = STRING_TOKEN(STR_BIOS_MANUFAC),
          text  = STRING_TOKEN(STR_BIOS_MANUFAC_VALUE);
#endif
//Y231219-S
#if (defined OEM_PRODUCT_NAME_SUPPORT && OEM_PRODUCT_NAME_SUPPORT == 1)
        text
          help  = STRING_TOKEN(STR_EMPTY),
          text  = STRING_TOKEN(STR_SPEC_PRODUCT),
          text  = STRING_TOKEN(STR_SPEC_PRODUCT_VALUE);
#endif

//Y231219-E
#if (defined OEM_BIOS_MODEL_NAME_SUPPORT && OEM_BIOS_MODEL_NAME_SUPPORT == 1)
        text
          help  = STRING_TOKEN(STR_EMPTY),
          text  = STRING_TOKEN(STR_BIOS_MODEL),
          text  = STRING_TOKEN(STR_BIOS_MODEL_VALUE);
#endif

#if (defined OEM_UUID_NUMBER_SUPPORT && OEM_UUID_NUMBER_SUPPORT == 1)
        text
          help  = STRING_TOKEN(STR_EMPTY),
          text  = STRING_TOKEN(STR_UUID),
          text  = STRING_TOKEN(STR_UUID_VALUE);
#endif
#if (defined OEM_SERIAL_NUMBER_SUPPORT && OEM_SERIAL_NUMBER_SUPPORT == 1)
        text
          help  = STRING_TOKEN(STR_EMPTY),
          text  = STRING_TOKEN(STR_SERIAL_NUMBER),
          text  = STRING_TOKEN(STR_SERIAL_NUMBER_VALUE);
#endif
```

* AmiCompatibilityPkg\Setup\Setup.c
```C
#if OEM_MANUFAC_NAME_SUPPORT        //K221009+

    InitString(

        HiiHandle,STRING_TOKEN(STR_BIOS_MANUFAC_VALUE),

    L"%s", STR(BIOS_MANUFAC_VALUE)    

    );

#endif

  

#if OEM_BIOS_MODEL_NAME_SUPPORT     //K221009+    

    InitString(

        HiiHandle,STRING_TOKEN(STR_BIOS_MODEL_VALUE),

    L"%s", STR(BIOS_MODEL_VALUE)    

    );    

#endif

  

#if OEM_PRODUCT_NAME_SUPPORT        //Y231219  

    InitString(

        HiiHandle,STRING_TOKEN(STR_SPEC_PRODUCT_VALUE),

    L"%s", STR(SPEC_PRODUCT_VALUE)    

    );    

#endif

  
  

#if OEM_ENCLOSURE_TYPE_SUPPORT      //Y231219  

    InitString(

        HiiHandle,STRING_TOKEN(STR_OEM_ENCLOSURE_TYPE_VALUE),

    L"%s", STR(ENCLOSURE_TYPE_VALUE)    

    );    

#endif
```

*AmiCompatibilityPkg\Setup\Setup.uni
```C
#string STR_UUID                        #language eng "UUID"
#string STR_UUID_VALUE            #language eng ""
#string STR_SERIAL_NUMBER                        #language eng "Serial Number"
#string STR_SERIAL_NUMBER_VALUE            #language eng ""
//Y231219-S
#string STR_SPEC_PRODUCT                        #language eng "Product Name"
#string STR_SPEC_PRODUCT_VALUE            #language eng ""
#string STR_OEM_ENCLOSURE_TYPE                        #language eng "Enclosure Type"
#string STR_OEM_ENCLOSURE_TYPE_VALUE            #language eng ""
//Y231219-E
```

*OEM\B660M4_PLUS\B660M4_PLUS.sdl
```C
TOKEN
    Name  = "OEM_MANUFAC_NAME_SUPPORT"
    Value  = "1"
    TokenType = Boolean
    TargetMAK = Yes
    TargetH = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
End

TOKEN
    Name  = "BIOS_MANUFAC_VALUE"
    Value  = "MARTEN LP LTD"
    Help  = "Specifies the Board Manufacturer."
    TokenType = Expression
    TargetMAK = Yes
    TargetH = Yes
    TargetEQU = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
    Token = "OEM_MANUFAC_NAME_SUPPORT" "=" "1"
End

TOKEN
    Name  = "OEM_SERIAL_NUMBER_SUPPORT"
    Value  = "1"
    TokenType = Boolean
    TargetMAK = Yes
    TargetH = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
End

TOKEN
    Name  = "OEM_UUID_NUMBER_SUPPORT"
    Value  = "1"
    TokenType = Boolean
    TargetMAK = Yes
    TargetH = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
End

TOKEN
    Name  = "OEM_PRODUCT_NAME_SUPPORT"
    Value  = "1"
    TokenType = Boolean
    TargetMAK = Yes
    TargetH = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
End

TOKEN
    Name  = "SPEC_PRODUCT_VALUE"
    Value  = "B760M4 Plus"
    Help  = "Specifies the Board Manufacturer."
    TokenType = Expression
    TargetMAK = Yes
    TargetH = Yes
    TargetEQU = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
    Token = "OEM_MANUFAC_NAME_SUPPORT" "=" "1"
End

TOKEN
    Name  = "OEM_ENCLOSURE_TYPE_SUPPORT"
    Value  = "1"
    TokenType = Boolean
    TargetMAK = Yes
    TargetH = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
End

TOKEN
    Name  = "ENCLOSURE_TYPE_VALUE"
    Value  = "Desktop Motherboard"
    Help  = "Specifies the Board Manufacturer."
    TokenType = Expression
    TargetMAK = Yes
    TargetH = Yes
    TargetEQU = Yes
    Token = "CUSTOMER_SUPPORT" "=" "1"
    Token = "OEM_ENCLOSURE_TYPE_SUPPORT" "=" "1"
End
```
