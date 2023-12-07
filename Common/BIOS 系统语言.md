```C
TOKEN
    Name = "SUPPORTED_LANGUAGES"
    Value = "$(DEFAULT_LANGUAGE);zh-chs"
    Help = "Semicolon separated list of names of the languages that the firmware supports in RFC 4646 format.\These are human readable languages that the end user will see in Setup.\Use RFC_LANGUAGES token to add configuration languages."
    TokenType = Expression
    TargetMAK = Yes
End

TOKEN
    Name = "DEFAULT_LANGUAGE"
    Value = "en-US"
    Help = "Name of the default system language in RFC4646 format.\"
    TokenType = Expression
    TargetMAK = Yes
End
```