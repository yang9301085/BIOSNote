RegisterNotification是AMITse中核心函数
里面包含了绝大部分有关AMITse的callback，Event，Hook
`AmiTsePkg\EDK\MiniSetup\BootOnly\notify.c`
```C
/**
    It will create a notify event and register a notification.
    @param VOID
    @retval Status
**/
EFI_STATUS RegisterNotification( VOID )
{
	EFI_STATUS 	Status = EFI_UNSUPPORTED;
	UINTN 		i;
	EFI_EVENT 	Event, GopEvent;
	EFI_GUID 	EfiDriverHealthProtocolGuid = EFI_DRIVER_HEALTH_PROTOCOL_GUID;
	VOID 		*Registration = NULL, *GopRegistration = NULL;

	NOTIFICATION_INFO **notify = _gNotifyList;

	for ( i = 0; *notify != NULL; i++, notify++ )
	{
		Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				NotificationFunction,
				*notify,
				&((*notify)->NotifyEvent)
				);

		if ( EFI_ERROR(Status) )
			continue;	

		Status = gBS->RegisterProtocolNotify(
				(*notify)->NotifyGuid,
				(*notify)->NotifyEvent,
				&((*notify)->NotifyRegistration)
				);
			// get any of these events that have occured in the past
		if (!EFI_ERROR (Status))
		{
		    gBS->SignalEvent( (*notify)->NotifyEvent );
		}
	}
	if (IsDriverHealthSupported ())			//Notifying the driver health protocol installation to update the drv health variable in cache
	{
		Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				_DrvHealthNotifyFunction,
				NULL,
				&Event
				);
		if (!EFI_ERROR (Status))
		{
			Status = gBS->RegisterProtocolNotify(
				&EfiDriverHealthProtocolGuid,
				Event,
				&Registration
				);
			if (!EFI_ERROR(Status))
			{
				gBS->SignalEvent (Event);
			}
		}
	}

//Validate the Gop before usage in all the possible cases and also get instance of Gop through notification
	Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				_UpdateGoPNotifyFunction,
				NULL,
				&GopEvent
				);
		if (!EFI_ERROR (Status))
		{
			Status = gBS->RegisterProtocolNotify(
				&gEfiGraphicsOutputProtocolGuid,
				GopEvent,
				&GopRegistration
				);
			if (!EFI_ERROR(Status))
			{
				gBS->SignalEvent (GopEvent);
			}
		}
	return Status;
}
```

```mermaid
graph TD
    classDef startend fill:#F5EBFF,stroke:#BE8FED,stroke-width:2px;
    classDef process fill:#E5F6FF,stroke:#73A6FF,stroke-width:2px;
    classDef decision fill:#FFF6CC,stroke:#FFBC52,stroke-width:2px;

    A([开始]):::startend --> B(初始化变量):::process
    B --> C(设置 notify 指向 _gNotifyList):::process
    C --> D{i < _gNotifyList 长度?}:::decision
    D -- 是 --> E(创建通知事件):::process
    E --> F{事件创建成功?}:::decision
    F -- 否 --> G(跳过当前项，i++ 并指向下一项):::process
    G --> D
    F -- 是 --> H(注册协议通知):::process
    H --> I{协议通知注册成功?}:::decision
    I -- 是 --> J(触发事件):::process
    J --> G
    I -- 否 --> G
    D -- 否 --> K{IsDriverHealthSupported?}:::decision
    K -- 是 --> L(创建驱动健康通知事件):::process
    L --> M{事件创建成功?}:::decision
    M -- 是 --> N(注册驱动健康协议通知):::process
    N --> O{协议通知注册成功?}:::decision
    O -- 是 --> P(触发驱动健康事件):::process
    P --> Q(创建 GOP 通知事件):::process
    M -- 否 --> Q
    O -- 否 --> Q
    K -- 否 --> Q
    Q --> R{事件创建成功?}:::decision
    R -- 是 --> S(注册 GOP 协议通知):::process
    S --> T{协议通知注册成功?}:::decision
    T -- 是 --> U(触发 GOP 事件):::process
    U --> V([返回 Status]):::startend
    R -- 否 --> V
    T -- 否 --> V
```

RegisterNotification通过CreateEvent注册Event，当相应的Event被触发， 就会执行NotificationFunction中对应的动作

